/***********************************************************************
 * 
 * The I2C bus scanner detects the addresses of the modules that are 
 * connected to the SDA and SCL signals. A simple description of FSM is 
 * used.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 8000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <util/delay.h>
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include "twi.h"            // TWI library for AVR-GCC

/* Variables ---------------------------------------------------------*/
typedef enum {              // FSM declaration
    STATE_IDLE = 1,
    STATE_SEND,
    STATE_ACK,
	STATE_WAIT
} state_t;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and send I2C (TWI) address every 33 ms.
 *           Send information about scanning process to UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize I2C (TWI)
    twi_init();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure 16-bit Timer/Counter1 to update FSM
    // Set prescaler to 33 ms and enable interrupt
    //TIM1_overflow_33ms();
    //TIM1_overflow_interrupt_enable();
	TIM1_overflow_33ms();
	TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("Scan I2C-bus for slave devices:\033[1E");

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
 * Purpose:  Update Finite State Machine and test I2C slave addresses 
 *           between 8 and 119.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static state_t state = STATE_IDLE;  // Current state of the FSM
    static uint8_t addr = 7;            // I2C slave address
    uint8_t result = 1;                 // ACK result from the bus
    char uart_string[2] = "00"; // String for converting numbers by itoa()
	static uint8_t initialised = 0;
	static uint8_t row_counter = 0;
	static uint8_t dev_counter = 0;
	if(!initialised)
	{
		uart_puts("      ");
		for(int i = 0; i <= 15; i++)
		{
			uart_puts(".");
			itoa(i,uart_string,16);
			uart_puts(uart_string);
			uart_puts(" ");
		}
		uart_puts("\033[1E");
		uart_puts("0x0.: RA RA RA RA RA RA RA RA ");
		initialised = !initialised;
	}
    // FSM
    switch (state)
    {
    // Increment I2C slave address
    case STATE_IDLE:
        addr++;
        // If slave address is between 8 and 119 then move to SEND state
		if (addr >= 8 && addr <= 119) 
		{
			state = STATE_SEND;
		}
		else 
		{
			addr = 7;
			uart_puts("RA RA RA RA RA RA RA RA");
			uart_puts("\033[1ENumber of detected devices: ");
			uart_puts(itoa(dev_counter,uart_string,10));
			uart_puts("\033[1E\033[1E");
			state = STATE_WAIT;
			dev_counter = 0;
			initialised = !initialised; 
		}
		if(addr % 16 == 0)
		{
			row_counter++;
			if(row_counter >= 8) 
			{
				row_counter = 1;
			}
			uart_puts("\033[1E");
			uart_puts("0x");
			uart_puts(itoa(row_counter,uart_string,10));
			uart_puts(".:");
			uart_puts(" ");
		}
        break;
    
    // Transmit I2C slave address and get result
    case STATE_SEND:
        // I2C address frame:
        // +------------------------+------------+
        // |      from Master       | from Slave |
        // +------------------------+------------+
        // | 7  6  5  4  3  2  1  0 |     ACK    |
        // |a6 a5 a4 a3 a2 a1 a0 R/W|   result   |
        // +------------------------+------------+
        result = twi_start((addr<<1) + TWI_WRITE);
        twi_stop();
        /* Test result from I2C bus. If it is 0 then move to ACK state, 
         * otherwise move to IDLE */
		if (result) 
		{
			state = STATE_IDLE;
			uart_puts("-- ");
		}
		else state = STATE_ACK;
        break;

    // A module connected to the bus was found
    case STATE_ACK:
        // Send info about active I2C slave to UART and move to IDLE
		uart_puts(itoa(addr,uart_string,16));
		uart_puts(" ");
		dev_counter++;
		state = STATE_IDLE;
        break;

    // If something unexpected happens then move to IDLE
	case STATE_WAIT:
		_delay_ms(10000);
		uart_puts("\033[2J\033[H");
		state = STATE_IDLE;
    default:
        state = STATE_IDLE;
        break;
    }
}
ISR(TIMER0_OVF_vect)
{
	static uint8_t addr = 92;
	static uint8_t data = 0;
	static char temp_buffer[3] = "000";
	twi_start((addr<<1) + TWI_WRITE);
	twi_write(0x02);
	twi_stop();
	twi_start((addr<<1) + TWI_READ);
	data = twi_read_ack();
	twi_stop();
	uart_puts(itoa(data,temp_buffer,10));
}
