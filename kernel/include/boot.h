#pragma once
#include <stdint.h>

#define BOOT_MAX_MEM_REGIONS 128
#define BOOT_MAX_CMDLINE_LEN 256

struct mem_region {
  uint64_t begin;
  uint64_t end;
};

struct boot_info {
  uint64_t kernel_begin;
  uint64_t kernel_end;

  struct mem_region mem_regions[BOOT_MAX_MEM_REGIONS];
  uint64_t mem_region_len;

  char cmdline[BOOT_MAX_CMDLINE_LEN];
};

void kmain(struct boot_info* boot_info);