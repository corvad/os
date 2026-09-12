ARCH	 := amd64

ifeq ($(shell uname -s),Darwin)
CC	 := /opt/homebrew/opt/llvm/bin/clang
LD       := /opt/homebrew/bin/ld.lld
MKRESCUE := /opt/homebrew/bin/x86_64-elf-grub-mkrescue
else
CC	 := clang
LD	 := ld.lld
MKRESCUE := grub-mkrescue
endif

LDFLAGS  := -nostdlib -T arch/$(ARCH)/linker.ld
ASM	 := nasm
ASMFLAGS := -f elf64
CFLAGS   := --target=x86_64-elf -ffreestanding -mcmodel=kernel -mno-sse -mno-mmx -fno-builtin -O2 -nostdlib -fno-stack-protector -fno-pie -mno-red-zone -Wall -Wextra -std=c11 -Iarch/$(ARCH)/include -Ikernel/include

PROJDIRS := arch kernel
KERNEL   := os.elf

ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.asm")
ASMOBJS  := $(patsubst %.asm,%.o,$(ASMFILES))

SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

.PHONY: all fmt clean

all: $(KERNEL)

$(KERNEL): $(ASMOBJS) $(OBJFILES) arch/$(ARCH)/linker.ld
	@$(LD) $(LDFLAGS) -o $@ $(ASMOBJS) $(OBJFILES)

%.o: %.asm Makefile
	@$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c $(HDRFILES) Makefile
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: iso

iso: $(KERNEL)
	@cp $(KERNEL) iso/boot
	@$(MKRESCUE) -o os.iso iso

clean:
	-@$(RM) $(wildcard $(KERNEL) $(ASMOBJS) $(OBJFILES) os.iso iso/boot/$(KERNEL))
