/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

volatile uint8_t cnt0 = 0;	// Counter 0
volatile uint8_t cnt1 = 0;	// Counter 1
volatile uint8_t cnt2 = 0;	// Counter 2
volatile uint8_t cnt3 = 0;	// Counter 3
volatile uint8_t cnt4 = 0;	// Counter 4

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Display decimal counter values on SSD (Seven-segment 
 *           display) when 16-bit Timer/Counter1 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
     SEG_update_shift_regs(3, 0);
	//SEG_update_shift_regs(0b01000001, 0b01110000);
	

    // Configure 16-bit Timer/Counter1 for Decimal counter
    // Set the overflow prescaler to 262 ms and enable interrupt
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
	// Configure 8-bit Timer/Counter0 for switching between positions
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();
	// Set the overflow prescaler to 262 ms and enable interrupt

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

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment decimal counter value and display it on SSD.
 **********************************************************************/
ISR(TIMER1_OVF_vect) // cnt0 - cnt4 act as counters, each of them iterates, when the previous one overflows
{
	cnt0++;
	if(cnt0 > 9) 
	{
		cnt0 = 0;
		cnt1++;
		if(cnt1 > 9)
		{
			cnt1 = 0;
			cnt2++;
			if(cnt2 > 9)
			{
				cnt2 = 0;
				cnt3++;
				if(cnt3 > 9) cnt3 = 0;
			}
		}
	}
}

ISR(TIMER0_OVF_vect)
{
	cnt4++;
	if(cnt4 > 3) cnt4 = 0;
	switch(cnt4)  // each case is for one display position (0-3)
	{
		case 0: SEG_update_shift_regs(cnt0, cnt4);break;
		case 1: SEG_update_shift_regs(cnt1, cnt4);break;
		case 2: SEG_update_shift_regs(cnt2, cnt4);break;
		case 3: SEG_update_shift_regs(cnt3, cnt4);break;
		default: break;
	}
}
