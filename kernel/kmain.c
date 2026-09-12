#include <boot.h>
#include <console.h>
#include <stdint.h>

void kmain(struct boot_info* b) {
  console_puts("[kmain] Initializing...\n");
  console_puts("[kmain] Boot info:\n");
  console_puts("[kmain] Memory regions:\n");
  console_puts("[kmain] Total RAM:\n");
  console_puts("[kmain] Initialized\n");
  console_puts("[kmain] Hello World!\n");
  while (1) {
    __asm__ volatile("hlt");
  }
}