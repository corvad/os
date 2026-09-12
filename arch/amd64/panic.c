#include <console.h>
#include <panic.h>

void kpanic(const char* err) {
  console_puts("[Kernel Panic] ");
  console_puts(err);
  console_puts("\n");
  __asm__ volatile("cli");
  while (1) {
    __asm__ volatile("hlt");
  }
}