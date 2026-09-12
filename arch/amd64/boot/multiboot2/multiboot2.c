#include "multiboot2.h"

#include <boot.h>
#include <console.h>

struct boot_info boot_info;

void multiboot2_init(uint32_t multiboot2_magic, uint32_t multiboot2_info) {
  console_init();
  console_puts("[multiboot2_init] Started multiboot2 validation...\n");

  console_puts("[multiboot2_init] Validation done, handing off to kmain\n");
  kmain(&boot_info);
}