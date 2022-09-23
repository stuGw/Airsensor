#include <stdint.h>
#include "STM32G030.h"
#include "adc.h"
void init(void);
void Default_Handler(void);
void USART2_Handler(void);
void HardFault_Handler(void);
//extern void adc::ADC_Handler(void);

void TIM3_Handler(void);
void RTCIrq_Handler(void);

int main(void);
// The following are 'declared' in the linker script
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;


extern unsigned char  _sidata;//INIT_DATA_VALUES;
extern unsigned char  _sdata;//INIT_DATA_START;
extern unsigned char  _edata;//INIT_DATA_END;
extern unsigned char  _sbss;//BSS_START;
extern unsigned char  _ebss;//BSS_END;

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

typedef void (*fptr)(void);
// the section "vectors" is placed at the beginning of flash 
// by the linker script
#pragma GCC push_options
#pragma GCC optimize ("O0")

const fptr Vectors[] __attribute__((section(".vectors"))) ={
	(fptr)0x20002000, 	/* Top of stack (8k) */ 
	init,   		/* Reset Handler */
	Default_Handler,	/* NMI */
	HardFault_Handler,	/* Hard Fault */
	Default_Handler,	/* MemManage */
	Default_Handler,	/* Reserved  */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */ 
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* SVCall */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* PendSV */
	Default_Handler,	/* SysTick */	
/* External interrupt handlers follow */
	Default_Handler, 	/* 0: WWDG */
	Default_Handler, 	/* 1: PVD */
	RTCIrq_Handler, 	/* 2: RTC */
	Default_Handler, 	/* 3: FLASH */
	Default_Handler, 	/* 4: RCC */
	Default_Handler, 	/* 5: EXTI0_1 */
	Default_Handler, 	/* 6: EXTI2_3 */
	Default_Handler,    /* 7: EXTI4_15 */
	Default_Handler, 	/* 8: Reserved */
	AnalogConverter::DMA_Handler, 	/* 9: DMA_CH1 */
	Default_Handler, 	/* 10: DMA_CH2_3 */
	Default_Handler, 	/* 11: DMA_CH4_5_6_7 */
	Default_Handler, 	/* 12: ADC/COMP */
	Default_Handler, 	/* 13: TIM1_BRK_UP_TRG_COM */
	Default_Handler, 	/* 14: TIM1_CC */
	Default_Handler, 	/* 15: TIM2 */
	Default_Handler,	/* 16: TIM3 */
	Default_Handler, 	/* 17: Reserved */
	Default_Handler, 	/* 18: Reserved */
	Default_Handler, 	/* 19: TIM14 */
	Default_Handler, 	/* 20: Reserved */
	Default_Handler, 	/* 21: TIM16 */
	Default_Handler, 	/* 22: TIM17 */
	Default_Handler, 	/* 23: I2C2 */
	Default_Handler,	/* 24: I2C2 */
	Default_Handler, 	/* 25: SPI1 */
	Default_Handler, 	/* 26: SPI2 */
	Default_Handler, 	/* 27: USART1 */
	USART2_Handler, 	/* 28: USART2 */
	Default_Handler, 	/* 29: USART3/USART4/LPUART1 */
	Default_Handler, 	/* 30: CEC */
	Default_Handler 	/* 31: AES/RNG */
	
};
void initClock()
{
     //return;
    // After reset, CPU clock is set to HSISYS = 16MHz
    
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1<<24);

        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
        // Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
        // inserted into Flash memory interface
        // If the chip is run at 64MHz then 2 wait states are required.
        // SysClock is taken from output R of the PLL.  The R output is VCO/2 so need to aim for 
        // twice the desired value i.e. 128MHz
        // 128 = 16 * 16 / 2 so N = 16; M = 2
        FLASH->ACR &= 0xfffffff0;
        FLASH->ACR |= 2;        
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 8);       
        
        // Set PLL input clock to 16MHz HSI clock        
        RCC->PLLSYSCFGR |= (1<<1);
        RCC->PLLSYSCFGR &= 0xffff80ff; // clear N bits
        RCC->PLLSYSCFGR |= (16 << 8);  // set N = 8;
        RCC->PLLSYSCFGR &= 0xffffff0f; // clear M bits
        RCC->PLLSYSCFGR |= (1 << 4);  // set M = 1 (divide by 2)
        RCC->PLLSYSCFGR |= (1 << 28) + (1 << 29); // enable R output of 128/2
        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void init_array()
{
    // This function calls constructors for global and static objects
    uint32_t count;
    uint32_t i;
    
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i] ();
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i] (); 
}

void initWakePin()
{
	RCC->IOPENR |= (1<<0);
	GPIOA->MODER |= 0x00100000;//PA10
	GPIOA->MODER &= ~0x00200000;//PA10
}
void initI2C2GPIO()
{
	RCC->IOPENR |= (1<<0);//enable port B
		//af opendrain
		GPIOA->MODER |= 0x02800000;//PB8 & PB9 set MODE1
		GPIOA->MODER &= ~0x01400000;//PB8 & PB9 reset MODE0
		GPIOA->OTYPER |= 0x00001800;
		GPIOA->OSPEEDR |= 0x0ff00000; //maxspeed
		GPIOA->PUPDR |= 0x01400000;
		GPIOA->AFRH |= 0x00066000; //af6 to b8 b9
	//debug
	//output opendrain
	//RCC->IOPENR |= (1<<1);//enable port B
	//af opendrain
	//GPIOB->MODER |= 0x000a0000;//PB8 & PB9 set MODE1
	//GPIOB->MODER &= ~0x00050000;//PB8 & PB9 reset MODE0
	//GPIOB->OTYPER |= 0x00000300;
	//GPIOB->OSPEEDR |= 0x000f0000; //maxspeed
	//GPIOB->AFRH |= 0x00000066; //af6 to b8 b9

	//work
	//output opendrain
	//RCC->IOPENR |= (1<<1);//enable port B
	//af opendrain
	//GPIOB->MODER |= 0x0000a000;//PB6& PB7 set MODE1
	//GPIOB->MODER &= ~0x00005000;//PB6 & PB7 reset MODE0
	//GPIOB->OTYPER |= 0x000000c0;
	//GPIOB->OSPEEDR |= 0x0000f000; //maxspeed
	//GPIOB->AFRL |= 0x66000000; //af6 to b7 b6
}


void offI2C2()
{
	I2C2->CR1 &= ~0x00000001;
	while((I2C2->CR1 & 0x00000001)!=0);
}

void initI2C2()
{	RCC->APBENR1 |= 0x00400000;//en 12c1 clocking
	initI2C2GPIO();

	offI2C2();
	I2C2->TIMINGR |= 0x10707dbc;//0x50330309;//TIMINGR conf example 400kHz 48Mhz rm738
	I2C2->CR1 |= 0x00000001;//enable
}


void initializeHw()
{
	//initSysTick();
	//initLedPin();
	//initBeeperPin();
	initI2C2();
	initWakePin();
   // initMemWPPin();
	//initHD44780GPIOMode8();//init gpio
	//initSPI2Pins();
	//initSPI2();
	//initEXTIPins();
}
void init()
{
// do global/static data initialization
	unsigned char *src;
	unsigned char *dest;
	unsigned len;
    initClock();
    src= &_sidata;//INIT_DATA_VALUES;
    dest= &_sdata;//INIT_DATA_START;
    len= &_edata-&_sdata;
	while (len--)
		*dest++ = *src++;
// zero out the uninitialized global/static variables
	dest = &_sbss;
	len = &_ebss - &_sbss;
	while (len--)
		*dest++=0;
    init_array();
    initializeHw();
	main();
}

void Default_Handler()
{
	while(1);
}
void HardFault_Handler()
{
	while(1);
}

#pragma GCC pop_options
