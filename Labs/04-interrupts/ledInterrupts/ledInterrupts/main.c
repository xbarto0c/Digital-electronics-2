/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3
#define LED_D4  PB2

//volatile int glob_var = 0; // když ji chci mìnit v rámci ISR
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED on the Multi-function shield using 
             the internal 8- or 16-bit Timer/Counter.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configuration of LED(s) at port B
    GPIO_config_output(&DDRB, LED_D1);
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_config_output(&DDRB, LED_D4);
    GPIO_write_high(&PORTB, LED_D1);
	GPIO_write_high(&PORTB, LED_D2);
	GPIO_write_high(&PORTB, LED_D3);
	GPIO_write_high(&PORTB, LED_D4);

    // Configuration of 16-bit Timer/Counter1 for LED blinking
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

void killAllLeds()
{
	GPIO_write_high(&PORTB, LED_D1);
	GPIO_write_high(&PORTB, LED_D2);
	GPIO_write_high(&PORTB, LED_D3);
	GPIO_write_high(&PORTB, LED_D4);
}
/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Toggle D1 LED on Multi-function shield.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
	static int glob_var = 0; // definuje se pouze jednou
	static int decrement = 1;
	if(glob_var % 3 == 0 ) decrement ^= 1;
	if(decrement) glob_var--;
	else glob_var++;
	switch(glob_var)
	{
		case 0: 
			{
				killAllLeds();
				GPIO_toggle(&PORTB, LED_D1);break;
			}
		case 1:
			{
				killAllLeds();
				GPIO_toggle(&PORTB, LED_D2);break;
			}
		case 2:
			{
				killAllLeds();
				GPIO_toggle(&PORTB, LED_D3);break;
			}
		case 3:
			{
				killAllLeds();
				GPIO_toggle(&PORTB, LED_D4);break;
			}
		default: killAllLeds();
	}
    
}


