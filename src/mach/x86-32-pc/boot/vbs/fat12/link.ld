SECTIONS
{
	. = 0x7c00;
	.text : { *(.text) *(.text.$) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
