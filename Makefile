ASM	 := nasm
ASMFLAGS := -f bin

PROJDIRS := boot

ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.asm")
BINFILES := $(patsubst %.asm,%.bin,$(ASMFILES))

SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

.PHONY: all fmt clean

all: $(BINFILES)

%.bin: %.asm Makefile
	@$(ASM) $(ASMFLAGS) $< -o $@

clean:
	-@$(RM) $(wildcard $(BINFILES))
