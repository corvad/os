#include <console.h>
#include <stdint.h>

#define SERIAL0 0x3F8

char in(uint16_t port) {
  char r;
  __asm__ volatile("inb %1, %0" : "=a"(r) : "Nd"(port));
  return r;
}

void out(uint16_t port, char c) {
  __asm__ volatile("outb %0, %1" : : "a"(c), "Nd"(port));
}

void console_init() {
  // init console
  out(SERIAL0 + 1, 0x00);
  out(SERIAL0 + 3, 0x80);
  out(SERIAL0 + 0, 0x01);
  out(SERIAL0 + 1, 0x00);
  out(SERIAL0 + 3, 0x03);
  out(SERIAL0 + 2, 0xC7);
  out(SERIAL0 + 4, 0x0B);
  out(SERIAL0 + 4, 0x0F);
}

void console_putc(char c) {
  while ((in(SERIAL0 + 5) & 0x20) == 0);
  out(SERIAL0, c);
}

void console_puts(const char* s) {
  while (*s != '\0') {
    console_putc(*s++);
  }
}