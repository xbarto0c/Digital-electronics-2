# Lab 8: Jan Bartoň

Link to this file in your GitHub repository:

[https://github.com/xbarto0c/Digital-electronics-2/edit/main/Labs/08-i2c/Assignment.md](https://github.com/xbarto0c/Digital-electronics-2/edit/main/Labs/08-i2c/Assignment.md)

### Arduino Uno pinout

1. In the picture of the Arduino Uno board, mark the pins that can be used for the following functions:
   * PWM generators from Timer0, Timer1, Timer2 - marked with blue colour
   * analog channels for ADC - marked with orange colour
   * UART pins - marked with red colour
   * I2C pins - marked with green colour
   * SPI pins - marked with yellow colour
   * external interrupt pins INT0, INT1 - marked with purple colour

   ![/Labs/08-i2c/arduino_uno.png](/Labs/08-i2c/arduino_uno.png)

### I2C

1. Code listing of Timer1 overflow interrupt service routine for scanning I2C devices and rendering a clear table on the UART.

```c
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
    case STATE_WAIT:	// wait after the scan finished, before running another one
	_delay_ms(10000);
	uart_puts("\033[2J\033[H");
	state = STATE_IDLE;
    default:
        state = STATE_IDLE;
        break;
    }
}
```

2. (Hand-drawn) picture of I2C signals when reading checksum (only 1 byte) from DHT12 sensor. Indicate which specific moments control the data line master and which slave.

   ![/Labs/08-i2c/I2C_comm.jpg](/Labs/08-i2c/I2C_comm.jpg)

### Meteo station

Consider an application for temperature and humidity measurement and display. Use combine sensor DHT12, real time clock DS3231, LCD, and one LED. Application display time in hours:minutes:seconds at LCD, measures both temperature and humidity values once per minut, display both values on LCD, and when the temperature is too high, the LED starts blinking.

1. FSM state diagram picture of meteo station. The image can be drawn on a computer or by hand. Concise name of individual states and describe the transitions between them.

   ![/Labs/08-i2c/thermometer.png](/Labs/08-i2c/thermometer.png)
