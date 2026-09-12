#include "multiboot2.h"

#include <boot.h>
#include <console.h>
#include <panic.h>

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
  console_puts("[multiboot2] Initialized\n");
  kmain(&boot_info);
}