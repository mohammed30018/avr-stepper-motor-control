/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    AVRLCD.cpp				         */
/*	  									 */
/*                                       */
/*****************************************/ 

#include  <avr/io.h>
#include "AVRLCD.H"
//--------------------------   some defines and the free characterset ----------------
#include "LCDset.h"

#define LCD_Delay 20	// needed for basic access, depended of the CPU clock speed

//--------------------------   low level Hardware-Anschluss --------------------------

#define LCD_DATAin PIND			    /* Daten vom LCD-Modul HighNibble */
#define LCD_DATAout PORTD	 	    /* Daten zum LCD-Modul HighNibble */
#define LCD_DataDin DDRD &= 0x0F    /* Datendirection vom LCD-Modul HighNibble */
#define LCD_DataDout DDRD |= ~0x0F  /* Datendirection zum LCD-Modul HighNibble */


#define LCD_Enable PORTB |= 0x01   		/* B.0 = Enable = 1 */
#define LCD_Disable PORTB &= ~0x01		/* B.0 = Enable = 0 */
#define LCD_RS_Data PORTB |= 0x04	  	/* B.2 = RS = 1 Register select Daten */
#define LCD_RS_Control PORTB &= ~0x04 	/* B.2 = RS = 0 Register select Control */
#define LCD_RW_Read PORTB |= 0x02		/* B.1 = R/W = 1 Read  */
#define LCD_RW_Write PORTB &= ~0x02		/* B.1 = R/W = 0 Write  */

void LCD_init_Hardware (void)			// set directions of the Port bits
{
	DDRB = DDRB | 0x01;	  // PORTB Bit 0 Output   (EN)
	DDRB = DDRB | 0x02;	  // PORTB Bit 1 Output   (R/W)
	DDRB = DDRB | 0x04;	  // PORTB Bit 2 Output   (RS)
	DDRD = DDRD | 0xF0;	  // PORTD BIT 7,6,5,4  Output (D7,D6,D5,D4)
}
//--------------------------  Modullokale Vaiablen -----------------------------


unsigned char z;

//------------------------------------------------------------------------------
const char HEXTAB [16] = {'0','1','2','3','4','5','6','7',
                          '8','9','A','B','C','D','E','F'};

//------------------------------------- Zeitverzögerung ------------------

void delay (unsigned int zeit)
	{unsigned int a;
	 for (;zeit > 0 ; zeit--)
	       a=zeit+a	;
        }


//---------------------------   Zugriff auf das Display ------------------------
    

unsigned char read_control (void)  

	{  unsigned char z;
	   LCD_DataDin;
	   LCD_RS_Control;
	   LCD_RW_Read;
	   LCD_Enable;    // HighNibble in z
	   z =  LCD_DATAin;
	   LCD_Disable;
	   LCD_Enable;    // LowNibble in z
	   z =  (( z & 0x0F0 ) | ((LCD_DATAin /16) & 0x0F));
	   LCD_Disable;
	   delay (LCD_Delay);
	   return (z);

	}

void write_control (unsigned char value)

	{  unsigned char z;
	   LCD_DataDout;
	   LCD_RS_Control;
	   LCD_RW_Write;	
       z    = ((LCD_DATAin & 0x0F) | (value & 0x0F0));  		//HighNibble
	   LCD_DATAout = z;							
	   LCD_Enable;												// Impuls
	   LCD_Disable;
       z   = ((LCD_DATAin & 0x0F) | ((value * 16) & 0x0F0)); 	// Low Nibble
	   LCD_DATAout = z;
	   LCD_Enable;												// Impuls
	   LCD_Disable;
	   delay (LCD_Delay);
	}


void clsLCD::write_data (unsigned char value)    // value 8 Bit

	{  LCD_DataDout;
	   LCD_RS_Data;
	   LCD_RW_Write;
       LCD_DATAout = ((LCD_DATAin & 0x0F) | (value & 0x0F0));  		 //HighNibble
	   LCD_Enable;													// Impuls
	   LCD_Disable;
       LCD_DATAout = ((LCD_DATAin & 0x0F) | ((value * 16) & 0x0F0)); // Low Nibble
	   LCD_Enable;													 // Impuls
	   LCD_Disable;
	   delay (LCD_Delay);
	}



//------------------------------------- INIT_LCD -------------------------

void clsLCD::init (void)
	{LCD_init_Hardware ();
     LCD_Disable;
//-------------------------------------------------------------------------
	// nach dem Einschalten mindestens 10 ms warten
	delay (10000);
 	write_control (Function_Set); 	// Procedere für RESET LCD
	delay (4100);
	write_control (Function_Set); 	// 
	delay (164);
	write_control (Function_Set); 	//
	delay (80);
	
 	write_control (Cursor_OFF); 	//
	delay (80);

 	write_control (Clear_Display); 	//
	delay (1640);

 	write_control (Entry_Mode_Set); 
	delay (80);  
   
}


//--------------------------- Cursorsteuerung  Position ------------------------

void clsLCD::gotoxy (unsigned char x , unsigned char y)       // nur zweizeilige Displays //

	{
	if (y == 0) 	write_control  ( Set_DD_RAM_address |  x);
    if (y == 1)     write_control  ( Set_DD_RAM_address | 0x40 | x);
	if (y == 2)     write_control  ( (Set_DD_RAM_address | x) + 20);
	if (y == 3)     write_control  ( (Set_DD_RAM_address | 0x40 | x) + 20 );
        }

//--------------------------- Cursorsteuerung  ON/OFF/BLINKEN ------------------


void clsLCD::Cursor (unsigned char ON, unsigned char BLINK)

{
	if (ON && BLINK)
	write_control (Cursor_BLINK);
	else if (ON && !BLINK)
	write_control (Cursor_ON);
	else
	write_control (Cursor_OFF);
}

//--------------------------- Clearscreen --------------------------------------
void clsLCD::Clearscreen (void)

{
	while ( read_control () & busy ) {}; // Hier Endlosschleife, wenn
	// Display nicht fertig wird
	write_control (Clear_Display); 	//
	delay (1650);
	while ( read_control () & busy ) {}; // Hier Endlosschleife, wenn
	// Display nicht fertig wird
	write_control (Clear_Display); 	//
	delay (1650);
}

// ------------------------ Zeichenkette ausgeben ------------------------------

void clsLCD::PrintString (const char *Zeichenkette)

	{	while (*Zeichenkette != 0)
			{ write_data (*Zeichenkette++);
			}
	}

   
//------------------------- Byte als Dezimalzahl ausgeben ---------------------

void clsLCD::PrintZahl8 (unsigned char zahl)
	{   unsigned char i;
	    unsigned char tempstring[6];
	    unsigned char schonzahl = 0;
	    for (i = 3; i> 0 ; i--)     // Umwandeln einer Zahl in einen String
		{ tempstring [i] = ((zahl % 10) | 0x30);
                  zahl = zahl / 10;
		}
	    for (i = 1; i <= 2; i++)	// führende Nullen durch BLANK ersetzen
		{	if ((tempstring [i] == 0x30) && (schonzahl == 0) && (i<=3)) 
				 tempstring [i] = 0x20;	//Leerzeichen
				 else
				 schonzahl = 1;

                }
	    for (i = 1; i <= 3; i++)    // Ausgabe
		{ write_data (tempstring [i]);
                }
	}
//--------------------------------------------------------------------------------  end -----

