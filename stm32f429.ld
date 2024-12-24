/* Define memory regions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2M     /* Flash memory */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 256K  /* SRAM */
}

/* Top of the stack */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* Initial stack pointer */

/* Entry point */
ENTRY(Reset_Handler)

/* Sections */
SECTIONS
{
    /* Vector table in FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Interrupt vector table */
        . = ALIGN(4);
    } > FLASH

    /* Code and read-only data in FLASH */
    .text :
    {
	. = ALIGN(4);
        *(.text*)   /* Code */
        *(.rodata*) /* Read-only data */
	. = ALIGN(4);
	_etext = .; /* Mark end of .text section */
    } > FLASH

    /* Initialized data in RAM, loaded from FLASH */
    .data : 
    {
        _sdata = .;            /* Start of .data section in RAM */
        *(.data*)              /* Initialized data */
        _edata = .;            /* End of .data section */
    } > RAM AT > FLASH

    /* Uninitialized data (zero-initialized) in RAM */
    .bss :
    {
        _sbss = .;             /* Start of .bss section */
        *(.bss*)               /* Uninitialized data */
        *(COMMON)
        _ebss = .;             /* End of .bss section */
    } > RAM

    /* Stack */
    .stack :
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + 0x4000;        /* 16KB stack */
        _estack = .;           /* End of stack */
    } > RAM

    /* Heap */
    .heap :
    {
        _sheap = .;
        . = . + 0x4000;        /* 16KB heap */
        _eheap = .;
    } > RAM
}

/* Symbols used in the startup code */
PROVIDE(__stack = _estack);
