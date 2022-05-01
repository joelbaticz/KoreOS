SECTIONS
{
	. = 0x0600;
	.text : { *(.text) *(.text.$) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
