MULTIBOOT2_MAGIC EQU 0xE85250D6
MULTIBOOT2_ARCH EQU 0

BITS 32

SECTION .multiboot
ALIGN 8

multiboot2_start:
	dd MULTIBOOT2_MAGIC
	dd MULTIBOOT2_ARCH
	dd multiboot2_end - multiboot2_start
	dd -(MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH + (multiboot2_end - multiboot2_start))
	dw 0
	dw 0
	dd 8
multiboot2_end:	 	

SECTION .bootdata
ALIGN 8
gdt:
	dq 0x00
	dq 0x00AF9A000000FFFF
	dq 0x00AF92000000FFFF
gdt_end:
gdtptr:
	dw gdt_end - gdt - 1
	dd gdt

SECTION .boot exec
GLOBAL _start

_start:
	; disable interrupts
	cli

	; save multiboot2 addressses
	mov [multiboot2_magic], eax
	mov [multiboot2_info], ebx

	; setup stack
	mov esp, stack_top

	; zero page tables
	mov edi, pml4
	mov ecx, 4096*4
	xor eax, eax
	rep stosb

	; setup pml4
	mov eax, pdpt
	or eax, 0x03
	mov [pml4], eax
	mov [pml4 + 4], 0
	mov [pml4 + 511 * 8], eax
	mov [pml4 + 511 * 8 + 4], 0

	; setup pdpt
	mov eax, pd
	or eax, 0x03
	mov [pdpt], eax
	mov [pdpt + 4], 0
	mov [pdpt + 510 * 8], eax
	mov [pdpt + 510 * 8 + 4], 0

	; setup pd
	xor ecx, ecx
.pd_loop:
	mov eax, ecx
	shl eax, 21
	or eax, 0x83
	mov [pd + ecx * 8], eax
	mov [pd + ecx * 8 + 4], 0
	inc ecx
	cmp ecx, 512
	jne .pd_loop

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; load pml4
	mov eax, pml4
	mov cr3, eax

	; enable long mode
	mov ecx, 0xc0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	; load gdt
	lgdt [gdtptr]

	; 64-bit jump
	jmp 0x08:_start64

SECTION .bootbss nobits alloc write

ALIGNB 4096
pml4:
	resb 4096
ALIGNB 4096
pdpt:
	resb 4096
ALIGNB 4096
pd:
	resb 4096
ALIGNB 4096
pt:
	resb 4096

ALIGNB 16
multiboot2_magic:
	resb 4
multiboot2_info:
	resb 4

ALIGNB 16
stack_bottom:
	resb 16384
stack_top:

SECTION .boot exec
BITS 64
EXTERN multiboot2_init
EXTERN _bss_begin
EXTERN _bss_end

_start64:
	cli
	
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; setup stack
	mov rsp, stack_top

	; zero .bss
	mov rdi, _bss_begin
	mov rcx, _bss_end
	sub rcx, rdi
	xor eax, eax
	rep stosb

	; call multiboot2_init
	mov rax, multiboot2_init
	mov edi, [rel multiboot2_magic]
	mov esi, [rel multiboot2_info]
	jmp rax