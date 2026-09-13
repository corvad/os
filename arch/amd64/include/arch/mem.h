#pragma once

#include <stdint.h>

#define PAGE_SIZE 4096ULL
#define PAGE_SHIFT 12
#define KERNEL_VMA 0xFFFFFFFF80000000ULL
#define P2V(addr) (((uint64_t)(addr) + KERNEL_VMA))
#define V2P(addr) (((uint64_t)(addr) - KERNEL_VMA))
#define PAGE_ALIGN_UP(addr) \
  (((uint64_t)(addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_DOWN(addr) ((uint64_t)(addr) & ~(PAGE_SIZE - 1))
#define BOOTSTRAP_MAX 0x40000000ULL