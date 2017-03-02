
/* This load file is used when __NO_ROM_DATA__ is not defined 

this file is incomplete and partial, badly adapted to gcc 3.x
from old gcc2.95 erika h8cfgNRD.lds

*/

OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300)
ENTRY(__start)

MEMORY
{
    mem : o = 0x8000, l = 0x6f00
}

SECTIONS
{
    .text : {
	*(.init)
        *(.text)
        *(.rodata)
	*(.vectors)
    } > mem

    .ctors : {
	__CTORS_START = . ;
	*(.ctors)
	__CTORS_END = . ;
    } > mem

    .dtors : {
	__DTORS_START = . ;
	*(.dtors)
	__DTORS_END = . ;
    } > mem

    .data : AT ( __DATA_ROM_START ) 
    {
        __DATA_RAM_START = . ;	
        *(.data)
	*(.tiny)
	__DATA_RAM_END = . ;
    } > mem
    
    .bss : {
/*        __BSS_START = . ; */
        __bss_start = . ;
        *(.bss)
        *(COMMON)
        _end = . ;
/*        __BSS_END = . ; */
    } > mem

/* Initialized data will be loaded in this section */
    .data_rom : {
	__DATA_ROM_SIZE = SIZEOF (.data) ;    
	__DATA_ROM_START = . ;
	. += __DATA_ROM_SIZE ;
	__DATA_ROM_END = . ;

    } > mem 
}
