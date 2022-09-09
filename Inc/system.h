/*
 * system.h
 *
 *  Created on: Apr 7, 2022
 *      Author: vit
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define FREQ_HZ 64000000
#define FREQ_MHZ FREQ_HZ/1000000



volatile inline void delay_us(volatile uint32_t val)
{
	volatile uint32_t delay = val*FREQ_MHZ;
	while(delay--);
}

volatile inline void delay_ms(volatile uint32_t val)
{
	delay_us(val*1000);
}



#endif /* SYSTEM_H_ */
