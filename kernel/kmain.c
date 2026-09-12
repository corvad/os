#include <boot.h>
#include <console.h>
#include <stdint.h>

void kmain(struct boot_info* b) {
  console_puts("[kmain] Init\n");
  while (1) {
    __asm__ volatile("hlt");
  }
}