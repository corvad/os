#include "multiboot2.h"

#include <arch/mem.h>
#include <boot.h>
#include <console.h>
#include <panic.h>
#include <print.h>

struct boot_info boot_info;

void multiboot2_init(uint32_t multiboot2_magic, uint32_t multiboot2_info) {
  console_init();
  console_puts("[multiboot2] Initializing...\n");
  console_puts("[multiboot2] Validating bootloader magic number... ");
  if (multiboot2_magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
    console_puts("OK\n");
  } else {
    console_puts("FAIL\n");
    kpanic("[multiboot2] Invalid bootloader magic number");
  }

  console_puts("[multiboot2] Parsing boot information... \n");
  struct multiboot_tag* tag = (struct multiboot_tag*)(P2V(multiboot2_info) + 8);

  boot_info.mem_region_len = 0;
  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
      case MULTIBOOT_TAG_TYPE_MMAP: {
        struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;
        uint32_t num_entries =
            (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;
        for (uint32_t i = 0; i < num_entries; i++) {
          struct multiboot_mmap_entry* entry =
              (struct multiboot_mmap_entry*)((uint8_t*)mmap->entries +
                                             (multiboot_uint32_t)
                                                     mmap->entry_size *
                                                 i);
          console_puts("[multiboot2] Memory Region: ");
          kprint_hex(entry->addr);
          console_putc(' ');
          kprint_hex(entry->len);
          console_putc(' ');
          kprint_hex(entry->type);
          console_putc('\n');
          if (boot_info.mem_region_len == BOOT_MAX_MEM_REGIONS) {
            kpanic("[multiboot2] Too many memory regions\n");
          }
          if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
            boot_info.mem_regions[boot_info.mem_region_len].begin = entry->addr;
            boot_info.mem_regions[boot_info.mem_region_len].end =
                entry->addr + entry->len;
            boot_info.mem_region_len++;
          }
        }
      } break;
    }
    tag = (struct multiboot_tag*)((uint8_t*)tag +
                                  (uint32_t)((tag->size + 7) / 8) * 8);
  }

  extern uint8_t _kernel_begin;
  extern uint8_t _kernel_end;
  boot_info.kernel_begin = (uint64_t)&_kernel_begin;
  console_puts("[multiboot2] Kernel Begin: ");
  kprint_hex(boot_info.kernel_begin);
  console_puts("\n");
  boot_info.kernel_end = (uint64_t)&_kernel_end;
  console_puts("[multiboot2] Kernel End: ");
  kprint_hex(boot_info.kernel_end);

  console_puts("\n");
  console_puts("[multiboot2] Initialized\n");
  kmain(&boot_info);
}