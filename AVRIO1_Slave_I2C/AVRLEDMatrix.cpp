/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    MODULE AVRLED_Matrix.cpp           */
/*    4 x Siebensegment					 */
/*										 */
/*****************************************/
//--------------------------------------------------------------------------
#include <avr/io.h>
//--------------------------------------------------------------------------
// Segmente an PORT D Bits [7..0]: (dp, g, f, e, d, c, b, a ) (0 = on)
// Ziffern an Port C (PC3, PC2, PC1, PC0) (1 = on)
// Segmenttabelle: Umsetzung von Indexeintrag auf Symbole 0-F, '-', ' ', 'h'
const unsigned char segmenttab[32] =
{0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, // 0..7
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, // 8..F
	0xBF, 0xFF, 0x8B                                // - BLANK h
};
unsigned char ziffernspeicher[4]; // enthält den Anzeigewert für eine Ziffer
//--------------------------------------------------------------------------
void initLED_Matrix (void)
{   DDRD = 0xFF;         // Segmente aus
	DDRC = DDRC | 0x0F;  // Ziffern aus
	ziffernspeicher[3] = 0x10;	 // es wird kurz ---- angezeigt
	ziffernspeicher[2] = 0x10;
	ziffernspeicher[1] = 0x10;
	ziffernspeicher[0] = 0x10;
}
//----------------------------------- Multiplexen der Ziffern -------------
void LED_mux (void)
{ static unsigned char digit = 0;           // Speicher für aktuelle Ziffer
	PORTD = 0x0FF;                            // Segmente aus
	PORTC = (PORTC&0xF0)| (1<<digit);         // nächste Ziffer aktivieren
	PORTD = segmenttab[ziffernspeicher[digit]]; // Segmente setzen
	digit++;                                  // für nächste Stelle
	digit = digit & 0x03;
}
//------------------------------------ Anzeige eines Fehlercodes ----------
void LEDMatrix_Print_err(unsigned char errornumber)
{ziffernspeicher[3] = 0x0E;      //'E'
	ziffernspeicher[2] = 17;        //BLANK
	ziffernspeicher[1] = (errornumber>>4);	     // High nibble errornumber
	ziffernspeicher[0] = (errornumber & 0x0F); // Low nibble errornumber
}
//--------------------- Umrechnung von value in eine Anzeige als HEX-Zahl -
void LEDMatrix_Printhex (unsigned int value)
{ for ( unsigned char digit = 0; digit <= 3; digit ++)
	{ziffernspeicher[digit] =(unsigned char)  value &  0xF;
		value = value >> 4;
	}
}
//---------------- Umrechnung von value in eine Anzeige als Dezimal-Zahl --
void LEDMatrix_Printuint (unsigned int value)
{ if (value > 9999)
	{LEDMatrix_Print_err(0x0F);
	}
	else
	{for ( unsigned char digit = 0; digit <= 3; digit ++)
		{ziffernspeicher[digit] =(unsigned char)(value % 10);
			value = value / 10;
		}
	}
}
void LEDMatrix_PrintDrehzahl(unsigned int value)//Schaltet die erste stelle der LED Matrix auf Blank
{
	ziffernspeicher[3]=17;
	if ((value & 0x80)==0x80)//Überbrüfung das achte Bit des Übergebene int.
	{
		ziffernspeicher[2] = 16;
	}
	else
	{
		ziffernspeicher[2] = 17;
	}
	value &= 0x7F;
	if (value > 99)
	{LEDMatrix_Print_err(0x0F);
	}
	else
	{for ( unsigned char digit = 0; digit <= 1; digit ++)
		{ziffernspeicher[digit] =(unsigned char)(value % 10);
			value = value / 10;
		}
	}
}
//-------------------------------------------------------------------------
