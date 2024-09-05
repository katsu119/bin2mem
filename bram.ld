MEMORY 
{
	BRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x1800 /* 6kB RAM*/
}

SECTIONS 
{
	.text : {
		. = ALIGN(4);
		* (.text)
		/* * (.*) */
	} > BRAM
	.other : {
		*(.*)
	}
}

