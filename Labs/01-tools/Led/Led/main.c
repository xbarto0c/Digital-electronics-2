/***********************************************************************
 * 
 * Blink a LED and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
/* The preprocessor will process directives that are inserted into the C
 * source code. These directives allow additional actions to be taken on
 * the C source code before it is compiled into object code. Directives
 * are not part of the C language itself.
 *
 * Preprocessor directives begin with a pound (#) symbol and may have 
 * several arguments. Do NOT put a semicolon character at the end of 
 * directives. This is a common mistake.
 */
#define LED_GREEN   PB5 // AVR pin where green LED is connected, nahrad� LED_GREEN PB5
#define SHORT_DELAY 1000 // Delay in milliseconds
#define LONG_DELAY 2000 // Delay in milliseconds
#ifndef F_CPU           // Preprocessor directive allows for conditional
                        // compilation. The #ifndef means "if not defined".
# define F_CPU 16000000 // CPU frequency in Hz required for delay
#endif                  // The #ifndef directive must be closed by #endif
typedef int bool;
#define true 1
#define false 0
/* Includes ----------------------------------------------------------*/
/* Include another C language file into the current file at the location
 * of the #include statement prior to compiling the source code.
 */
#include <util/delay.h> // Functions for busy-wait delay loops
#include <avr/io.h>     // AVR device-specific IO definitions

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN); // posunut� jedni�ky o PB5 (= 5) na pozici p�t�ho bitu, bitov� 'or' s p�vodn�m DDRB
    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
	bool first_run = true;
    PORTB = PORTB & ~(1<<LED_GREEN); // na port PB5 zapsat '1', bitov� 'and' s p�vodn�m DDRB

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        if(first_run) 
		{
			_delay_ms(SHORT_DELAY);
			first_run = !first_run;
		}
		else 
		{
			_delay_ms(LONG_DELAY);
			first_run = !first_run;
		}

        // Invert LED in Data Register
        // PORTB = PORTB xor 0010 0000
        PORTB = PORTB ^ (1<<LED_GREEN);
    }

    // Will never reach this
    return 0;
}

