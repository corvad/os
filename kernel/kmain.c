#include <boot.h>
#include <console.h>
#include <pmm.h>
#include <print.h>
#include <stdint.h>

void kmain(struct boot_info* boot_info) {
  console_puts("[kmain] Initializing...\n");
  pmm_init(boot_info);
  console_puts("[kmain] Initialized\n");
  console_puts("[kmain] pmm_alloc(): ");
  uint64_t addr = pmm_alloc();
  kprint_hex(addr);
  console_putc('\n');
  console_puts("[kmain] pmm_free(): ");
  pmm_free(addr);
  console_putc('\n');
  console_puts("[kmain] pmm_alloc(): ");
  kprint_hex(pmm_alloc());
  console_putc('\n');
  while (1) {
    __asm__ volatile("hlt");
  }
}