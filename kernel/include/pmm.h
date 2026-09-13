#pragma once
#include <boot.h>
#include <stdint.h>

void pmm_init(struct boot_info* boot_info);
uint64_t pmm_find_bitmap_region(struct boot_info* boot_info, uint64_t size);
uint64_t pmm_alloc(void);
void pmm_free(uint64_t addr);
void pmm_reserve(uint64_t begin, uint64_t end);