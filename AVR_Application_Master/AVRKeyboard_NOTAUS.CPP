/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    AVRKeyboard_NOTAUS.H               */
/*	  									 */
/*                                       */
/*****************************************/

#include <avr/io.h>
#include "AVRKeyboard_NOTAUS.h"
/*Hardware keyboard:
		Zeilen   B2,B1,B0  Access with Decoder 
		Spalten  D4 (left),D5,D6(right)
*/
//------------------------------------------  line Decoder ------------------------------

const unsigned char Zeilencode[4] = {6,4,2,0};

//------------------------------ Keyboard Konstuktor ------------------------------------

clsKeyboard::clsKeyboard (void)
	{	oldscancode = 0; 
		pressed = 0;
	}
//----------------------   Tastatur scan 3 Spalten, 4 Zeilen ----------------------------

unsigned char clsKeyboard::Keyboard_gedrueckt (void)
{
	volatile unsigned char spalte, scancode, zcount, scount, matrixcounter;

	DDRB = DDRB | 0x07;
	DDRD = DDRD & 0x0F;
	PORTD = PORTD | 0xF0;

	matrixcounter = 1;
	scancode = 0;

	for (zcount = 0; zcount <= 3; zcount++)
	{
		spalte = 0x10;
		PORTB = (PORTB & 0xF8) | Zeilencode[zcount];
		for (scount = 0; scount <= 2; scount++)
		{
			if ((~PIND & spalte) > 0) { scancode = matrixcounter; }
			matrixcounter++;
			spalte = spalte << 1;
		}
	}

	if (scancode == 0)
	{
		oldscancode = 0;
		pressed = 0;
		return (0);
	}
	else
	{
		if (scancode != oldscancode)
		{
			oldscancode = scancode;
			pressed = 0;
			return (0);
		}
		else if (!pressed)
		{
			pressed = 1;
			return (1);
		}
	}
	return (0);
}