#include <stdint.h>

/* Forward declaration of the default handlers */
void Reset_Handler(void);
void Default_Handler(void);

/* Linker script defined symbols */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _etext;
extern uint32_t _estack;

/* Vector table */
__attribute__((section(".isr_vector")))
uint32_t vector_table[] = {
    (uint32_t)&_estack,       /* Initial stack pointer */
    (uint32_t)Reset_Handler,  /* Reset handler */
    (uint32_t)Default_Handler, /* NMI handler */
    (uint32_t)Default_Handler, /* HardFault handler */
    (uint32_t)Default_Handler, /* MemManage handler */
    (uint32_t)Default_Handler, /* BusFault handler */
    (uint32_t)Default_Handler, /* UsageFault handler */
    0, 0, 0, 0,               /* Reserved */
    (uint32_t)Default_Handler, /* SVC handler */
    (uint32_t)Default_Handler, /* DebugMon handler */
    0,                         /* Reserved */
    (uint32_t)Default_Handler, /* PendSV handler */
    (uint32_t)Default_Handler, /* SysTick handler */
    /* may need to add additional interrupt handlers here */
};

/* Reset Handler */
void Reset_Handler(void) {
    /* Copy data section from Flash to RAM */
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero initialize the .bss section */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the main function */
    extern int main(void);
    main();

    /* If main returns, loop forever */
    while (1);
}

/* Default Handler */
void Default_Handler(void) {
    while (1);
}
