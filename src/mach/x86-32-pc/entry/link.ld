SECTIONS
{
	. = 0xfa00;
	.text : { *(.text) *(.text.$) }
}
