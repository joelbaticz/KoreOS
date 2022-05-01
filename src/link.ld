SECTIONS
{
	. = 0x10000;

	/* Code */
	.text :
	{
		__TEXT_START__ = . ;
		*(.text)
		*(.data)
		*(.rdata)
		__TEXT_END__ = . ;
	}

  	.bss :
  	{
    		__BSS_START__ = . ;
    		*(.bss)
    		*(COMMON)
    		__BSS_END__ = . ;
  	}

	/DISCARD/ :
	{
		*(*)
	}

}