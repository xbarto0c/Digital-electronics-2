# Lab 5: Jan Bartoň

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/xbarto0c/Digital-electronics-2](https://github.com/xbarto0c/Digital-electronics-2)


### 7-segment library

1. In your words, describe the difference between Common Cathode and Common Anode 7-segment display.
   * CC SSD - pro zobrazení jedné číslice se v displeji nachází 7 LED diod + 1 na desetinnou tečku, katody všech diod jsou spojeny do jedné, aby se ušetřili piny
   * CA SSD - pro zobrazení jedné číslice se v displeji nachází 7 LED diod + 1 na desetinnou tečku, anody všech diod jsou spojeny do jedné, aby se ušetřili piny

2. Code listing with syntax highlighting of two interrupt service routines (`TIMER0_OVF_vect`, `TIMER1_OVF_vect`) from counter application with at least two digits, ie. values from 00 to 59:

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 9999.
 **********************************************************************/
ISR(TIMER1_OVF_vect) // cnt0 - cnt4 act as counters, each of them iterates, when the previous... 
                     // ... one overflows, interrupt1 gets triggered each second
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
```

3. Flowchart figure for function `SEG_clk_2us()` which generates one clock period on `SEG_CLK` pin with a duration of 2&nbsp;us. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![/Labs/05-segment/Flowchart.png](/Labs/05-segment/Flowchart.png)


### Kitchen alarm

Consider a kitchen alarm with a 7-segment display, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![/Labs/05-segment/Schematic.png](/Labs/05-segment/Schematic.png)
