/* 1.The Firmware's Entry Point */
/* Here, the entry point is the function named Reset_Handler that will be implemented in the startup file. */
ENTRY(Reset_Handler)

/* 2.Available Memory Definitions */
/* Our script specifies two memory regions: FLASH and SRAM. */
MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
	SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

/* Symbol Creation for The End of The SRAM Memory Region */
/* This symbol is used to initialize the stack pointer. */
/* The stack grows downward (from high memory to low memory addresses in most architectures), so
   setting the stack pointer to the end of SRAM ensures that it starts at the maximum available address,
   utilizing the SRAM space efficiently for stack operations. */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);
 
/* 3.Required Heap and Stack Size in Bytes */
__max_heap_size = 0x200;    /* for dynamic memory allocation */
__max_stack_size = 0x400;   /* for function call management */

/* 4.Output Sections Definitions */
SECTIONS
{
    /* The First Section of Output File */
	.text :
	{
	  . = ALIGN(4);        /* align the start of the .text section on a 4-byte boundary */
	  *(.isr_vector_tbl)   /* merge all .isr_vector_tbl sections from the input files into the current location in the .text section */
	  *(.text)             /* merge all .text sections of input files into the current location in the .text section */
	  *(.rodata)           /* merge all .rodata sections of input files into the current location in the .text section */
	  . = ALIGN(4);        /* ensure that the end of the section is aligned to a 4-byte boundary */
	 _etext = .;           /* create a global symbol to hold end of text section, which is used as a pointer in the startup file */
	} > FLASH              /* place the .text section in the FLASH memory segment */
	    

    /* The Second Section of Output File */
	.data :
	{
	 . = ALIGN(4);        /* align the start of the .data section on a 4-byte boundary */
	_sdata = .;           /* create a global symbol to hold start of data section, which is used as a pointer in the startup file */
	  *(.data)            /* merge all .data sections of input files into the current location in the .data section */
	 . = ALIGN(4);        /* ensure that the end of the section is aligned to a 4-byte boundary */
	_edata = .;           /* create a global symbol to hold end of data section, which is used as a pointer in the startup file */
	} > SRAM AT> FLASH    /* VMA (location of section at run-time) = SRAM and LMA (location of initial values of section) = FLASH */
	
	
	/* The Third Section of Output File */
	.bss :
	{
	 . = ALIGN(4);        /* align the start of the .bss section on a 4-byte boundary */
	_sbss = .;            /* create a global symbol to hold start of bss section, which is used as a pointer in the startup file */
	*(.bss)               /* merge all .bss sections of input files into the current location in the .bss section */
	 . = ALIGN(4);        /* ensure that the end of the section is aligned to a 4-byte boundary */
	_ebss = .;            /* create a global symbol to hold end of bss section, which is used as a pointer in the startup file */
	} > SRAM              /* place the .bss section in the SRAM memory segment */
}