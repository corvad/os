ENTRY(_start)

LMA = 2M;
VMA = 0xFFFFFFFF80000000;

SECTIONS {
	. = LMA;

	.boot : ALIGN(8)
	{
		*(.multiboot)
		*(.boot)
	}

	.bootdata : ALIGN(8)
	{
		*(.bootdata)
	}

	.bootbss (NOLOAD) : ALIGN(4K)
	{
		*(.bootbss)
	}

	. += VMA;

	.text : AT(ADDR(.text) - VMA) ALIGN(4K)
	{
		*(.text)
	}

	.rodata : AT(ADDR(.rodata) - VMA) ALIGN(4K)
	{
		*(.rodata)
	}

	.data : AT(ADDR(.data) - VMA) ALIGN(4K)
	{
  		*(.data)
	}

	.bss (NOLOAD) : AT(ADDR(.bss) - VMA) ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
