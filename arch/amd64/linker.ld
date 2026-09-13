ENTRY(_start)

SECTIONS {
	. = 2M;

	_kernel_begin = .;

	.boot : ALIGN(8)
	{
		*(.multiboot)
		*(.boot .boot.*)
	}

	.bootdata : ALIGN(8)
	{
		*(.bootdata .bootdata.*)
	}

	.bootbss (NOLOAD) : ALIGN(4K)
	{
		*(.bootbss .bootbss.*)
	}

	. = 0xFFFFFFFF80400000;

	.text : AT(ADDR(.text) - 0xFFFFFFFF80000000) ALIGN(4K)
	{
		*(.text .text.*)
	}

	.rodata : AT(ADDR(.rodata) - 0xFFFFFFFF80000000) ALIGN(4K)
	{
		*(.rodata .rodata.*)
	}

	.data : AT(ADDR(.data) - 0xFFFFFFFF80000000) ALIGN(4K)
	{
  		*(.data .data.*)
	}

	.bss (NOLOAD) : AT(ADDR(.bss) - 0xFFFFFFFF80000000) ALIGN(4K)
	{
		_bss_begin = .;
		*(COMMON)
		*(.bss .bss.*)
		_bss_end = .;
	}

	_kernel_end = .-0xFFFFFFFF80000000;
}
