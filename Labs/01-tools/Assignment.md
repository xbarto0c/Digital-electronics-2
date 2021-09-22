# Lab 1: Jan Bartoň

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/xbarto0c/Digital-electronics-2](https://github.com/xbarto0c/Digital-electronics-2)


### Blink example

1. What is the meaning of the following binary operators in C?
   * `|`
   * `&`
   * `^`
   * `~`
   * `<<`
   * `>>`

2. Complete truth table with operators: `|`, `&`, `^`, `~`

| **b** | **a** |**b or a** | **b and a** | **b xor a** | **not b** |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 0 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 0 | 1 | 1 |
| 1 | 0 | 1 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 |


### Morse code

1. Listing of C code with syntax highlighting which repeats one "dot" and one "comma" on a LED:

```c
#define LED_GREEN   PB5 
#define SHORT_DELAY 1000
#ifndef F_CPU                                 
# define F_CPU 16000000
#endif                 
#include <util/delay.h>
#include <avr/io.h>    

int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);
    
    // definice datového typu bool, tj. proměnná obsahující buď hodnotu '1' - pravda, nebo '0' - nepravda
    typedef int bool;
    #define true 1
    #define false 0
    bool odd_run = true;
    
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        if(odd_run) // pokud cyklem probíhá po (2n+1)-té (lichý počet iterací), blikne dioda krátce, pokud po 2n-té (sudý počet iterací), blikne dioda dlouze
		{
			_delay_ms(500);
			odd_run = !odd_run;
		}
		else 
		{
			_delay_ms(1000);
			odd_run = !odd_run;
		}

        // Invert LED in Data Register
        // PORTB = PORTB xor 0010 0000
        PORTB = PORTB ^ (1<<LED_GREEN);
    }


    // Will never reach this
    return 0;
}
```


2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Wiring diagram](/Labs/01-tools/Wiring_diagram_morse.jpg)
