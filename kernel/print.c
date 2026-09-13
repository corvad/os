#include <console.h>
#include <print.h>

void kprint_hex(uint64_t value) {
  static const char digits[] = "0123456789ABCDEF";
  console_puts("0x");
  for (int i = 15; i >= 0; i--) {
    char digit = (value >> (i * 4)) & 0xF;
    console_putc(digits[digit]);
  }
}