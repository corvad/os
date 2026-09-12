#pragma once
#include <stdint.h>

struct mem_region {
  uint64_t start;
  uint64_t len;
  uint64_t type;
};

struct boot_info {
  uint64_t* mem_regions;
  uint64_t mem_region_len;
};

void kmain(struct boot_info* b);