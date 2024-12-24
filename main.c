#include <stdint.h>


// Peripherals

#define RCC_BASE 	0x40023800
#define GPIOE_BASE 	0x40021000

#define FLASH_ACR 	(*(volatile uint32_t *)0x40023C00)

// Register offsets

#define RCC_AHB1ENR 	(*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOE_MODER 	(*(volatile uint32_t *)(GPIOE_BASE + 0x00))
#define GPIOE_ODR 	(*(volatile uint32_t *)(GPIOE_BASE + 0x14))

// Bit positions

#define RCC_AHB1ENR_GPIOEEN 	(1 << 4)

// Clock Initialization
#define RCC_CR 		(*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_PLLCFGR 	(*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR	(*(volatile uint32_t *)(RCC_BASE + 0x08))


#define RCC_CR_HSEON 	(1 << 16)
#define RCC_CR_HSERDY 	(1 << 17)
#define RCC_CR_PLLON	(1 << 24)
#define RCC_CR_PLLRDY	(1 << 25)

#define FLASH_ACR_LATENCY_5WS	(5 << 0)
#define FLASH_ACR_PRFTEN	(1 << 8)
#define FLASH_ACR_ICEN		(1 << 9)
#define FLASH_ACR_DCEN		(1 << 10)

#define RCC_CFGR_SW_PLL		(2 << 0) // PLL selected as system clock
#define RCC_CFGR_SWS_PLL	(2 << 2) // PLL used as the system clock

#define RCC_PLLCFGR_PLLSRC_HSE (1 << 22) // HSE oscillator clock selected as PLL and PLLI2S clock entry

#define PLL_M 16
#define PLL_N 360
#define PLL_P 2
#define PLL_Q 7



// Function prototypes
void delay_ms(uint32_t ms);
void init_gpio(void);
void toggle_led(void);
void clock_init_HSI(void);

int main(void){
	clock_init_HSI();

	init_gpio();

	while(1){
		toggle_led();

		delay_ms(100);
	}

	return 0;
}

void init_gpio(void){
	RCC_AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

	volatile int delay = 18000;
	while (delay--) asm volatile("nop");

	for(int i = 7; i < 14; i++){
	GPIOE_MODER &= ~(3 << (i * 2));
	GPIOE_MODER |= (1 << (i * 2));
	}

}

void toggle_led(void){
	for(int i = 7; i < 14; i++){
		GPIOE_ODR ^= (1 << i);
		delay_ms(50);
	}
}

void delay_ms(uint32_t ms){
	for(uint32_t i = 0; i < ms * 18000; i++){
		asm volatile("nop");
	}
}

void clock_init_HSI(void){
	RCC_CR |= (1 << 0);
	while(!(RCC_CR & (1 << 1)));

	RCC_PLLCFGR = (PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | (PLL_Q << 24));

	RCC_PLLCFGR &= ~(1 << 22);

	RCC_CR |= RCC_CR_PLLON;
	while (!(RCC_CR & RCC_CR_PLLRDY));

	FLASH_ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;
	
	RCC_CFGR &= ~0x3;
	RCC_CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC_CFGR & (0x3 << 2)) != RCC_CFGR_SWS_PLL);
}
