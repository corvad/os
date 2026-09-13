#include "pmm.h"

#include <arch/mem.h>
#include <boot.h>
#include <console.h>
#include <panic.h>
#include <print.h>
#include <stdint.h>

static uint64_t* bitmap;
static uint64_t bitmap_len;
static uint64_t total_pages;

void pmm_init(struct boot_info* boot_info) {
  console_puts("[pmm] Initializing...\n");
  uint64_t max_end = 0;
  for (uint64_t i = 0; i < boot_info->mem_region_len; i++) {
    if (boot_info->mem_regions[i].end > max_end) {
      max_end = boot_info->mem_regions[i].end;
    }
  }
  if (max_end == 0) {
    kpanic("[pmm] No memory available\n");
  }

  total_pages = max_end >> PAGE_SHIFT;
  console_puts("[pmm] Total Pages: ");
  kprint_hex(total_pages);
  console_putc('\n');
  bitmap_len = (total_pages + 63) / 64;
  console_puts("[pmm] Bitmap Length: ");
  kprint_hex(bitmap_len);
  console_putc('\n');
  uint64_t possible_bitmap = pmm_find_bitmap_region(boot_info, bitmap_len * 8);
  if (possible_bitmap == 0) {
    kpanic("[pmm] No memory available for the pmm bitmap\n");
  }
  bitmap = (uint64_t*)P2V(possible_bitmap);
  console_puts("[pmm] Bitmap Addr: ");
  kprint_hex((uint64_t)possible_bitmap);
  console_putc('\n');
  console_puts("[pmm] Bitmap Virtual Addr: ");
  kprint_hex((uint64_t)bitmap);
  console_putc('\n');
  console_puts("[pmm] Zeroing Bitmap...\n");
  for (uint64_t i = 0; i < bitmap_len; i++) {
    bitmap[i] = 0xFFFFFFFFFFFFFFFFULL;
  }

  for (uint64_t i = 0; i < boot_info->mem_region_len; i++) {
    uint64_t begin = PAGE_ALIGN_UP(boot_info->mem_regions[i].begin);
    uint64_t end = PAGE_ALIGN_DOWN(boot_info->mem_regions[i].end);
    for (uint64_t addr = begin; addr + PAGE_SIZE <= end; addr += PAGE_SIZE) {
      uint64_t page = addr >> PAGE_SHIFT;
      if (page >= total_pages) {
        break;
      }
      if ((bitmap[page / 64] & (1ULL << (page % 64))) != 0) {
        bitmap[page / 64] = bitmap[page / 64] & ~(1ULL << (page % 64));
      }
    }
  }
  pmm_reserve(0, boot_info->kernel_end);
  pmm_reserve(possible_bitmap, possible_bitmap + bitmap_len * 8);
  if (max_end > BOOTSTRAP_MAX) {
    pmm_reserve(BOOTSTRAP_MAX, max_end);
  }
  console_puts("[pmm] Initialized\n");
}

void pmm_reserve(uint64_t begin, uint64_t end) {
  begin = PAGE_ALIGN_DOWN(begin);
  end = PAGE_ALIGN_UP(end);
  for (uint64_t addr = begin; addr < end; addr += PAGE_SIZE) {
    uint64_t page = addr >> PAGE_SHIFT;
    if (page >= total_pages) {
      break;
    }
    bitmap[page / 64] = bitmap[page / 64] | (1ULL << (page % 64));
  }
}

uint64_t pmm_find_bitmap_region(struct boot_info* boot_info, uint64_t size) {
  for (uint64_t i = 0; i < boot_info->mem_region_len; i++) {
    uint64_t begin = boot_info->mem_regions[i].begin;
    uint64_t end = boot_info->mem_regions[i].end;

    if (begin < boot_info->kernel_end) {
      begin = boot_info->kernel_end;
    }

    if (end > BOOTSTRAP_MAX) {
      end = BOOTSTRAP_MAX;
    }

    begin = PAGE_ALIGN_UP(begin);

    if (end > begin && end - begin >= size) {
      return begin;
    }
  }
  return 0;
}

uint64_t pmm_alloc(void) {
  for (uint64_t i = 0; i < bitmap_len; i++) {
    if (bitmap[i] == 0xFFFFFFFFFFFFFFFFULL) {
      continue;
    }

    uint64_t free_page = __builtin_ctzll(~bitmap[i]);
    uint64_t page = i * 64 + free_page;

    bitmap[i] = bitmap[i] | (1ULL << free_page);

    uint64_t addr = page << PAGE_SHIFT;

    uint64_t* page_addr = (uint64_t*)P2V(addr);
    for (uint64_t j = 0; j < PAGE_SIZE / 8; j++) {
      page_addr[j] = 0;
    }
    return addr;
  }

  return 0;
}

void pmm_free(uint64_t page_addr) {
  if (page_addr % PAGE_SIZE != 0) {
    kpanic("[pmm] Freeing a non-aligned page\n");
  }

  uint64_t page = page_addr >> PAGE_SHIFT;

  if (page >= total_pages) {
    kpanic("[pmm] Freeing an out-of-bounds page\n");
  }

  if ((bitmap[page / 64] & (1ULL << (page % 64))) == 0) {
    kpanic("[pmm] Page already free\n");
  }

  bitmap[page / 64] = bitmap[page / 64] & ~(1ULL << (page % 64));
}