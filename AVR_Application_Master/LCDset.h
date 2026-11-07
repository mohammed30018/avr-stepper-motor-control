/***********************************************/
/*    FHDAVR  LCDset                           */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	  CPU ATMEGA8/168                          */
/*    MODULE LCDset.h                          */
/*	     		            				   */
/*    settings LCD                             */ 
/*                                             */    
/*                                             */
/***********************************************/
//-----------Cursor-------------------------------------------------------------

void LCD_Cursor (unsigned char ON, unsigned char BLINK);
#define CON 	1
#define COFF 	0
#define CBLINK 	1
#define CSTATIC 0
//------------------------- Function_Set ---------------------------------------
// #define Function_Set 	0x38	// für 8 Bit Zugriff
#define Function_Set 	0x28	/* für 4 Bit Zugriff    */
	
	//	0	0	1	DL	N	F	x	x

	//				^: 0  =	4 Bit   ; 1 = 8 Bit
	//					^:  N=0 ->  eine Zeile 
	//				            N=1 ->  mehrere Zeilen 
	//						^> F=0 5*7 Punkte
	//						   F=1 5*10 Punkte
	//							(nur bei einer Zeile)

//----------------------   Cursorsteuerung Register Display ON/OFF -------------

#define Cursor_ON 		0x0E	/* nicht blinkend */
#define Cursor_OFF		0x0C	/* Anzeige an, kein Cursor */
#define Cursor_BLINK	0x0F	/* Anzeige an, blinkender Cursor */

	//	0 	0	0	0 	1	D	C	B
	//						Display on/off
	//							Cursor
	//								Blinken

//-----------------------   Clear Display --------------------------------------

#define Clear_Display	0x01    /* löschen und Home */

//-------------------------- HOME ----------------------------------------------

#define Cursor_HOME 	0x02    /* Cursor nach links oben, Anzeige bleibt erhalten */

//-------------------------- Entry_Mode_Set ------------------------------------

#define Entry_Mode_Set 	0x06	/* Cursor inkrementieren */

//--------------------------  Busyflag BF --------------------------------------

#define busy		0x80

//--------------------------  Cursorposition setzen ----------------------------

#define Set_DD_RAM_address	0x80

//--------------------------  Character Generator Adresse setzen ---------------

#define Set_CG_RAM_address	0x40

//---------------------------  free Symbols ----------------------



const unsigned char CG_Set[8*8] =
	{	0b00000000,			// 0 Ausrufezeichen
		0b00011111,
		0b00001110,
		0b00000100,	
		0b00000100,
		0b00000000,
		0b00000100,
		0b00000000,
		
		0b00101010,			// 1 Streifen
		0b00101010,
		0b00101010,
		0b00101010,	
		0b00101010,
		0b00101010,
		0b00101010,
		0b00101010,
				
		0b00000000,			// 2 Raute
		0b00000100,
		0b00001110,	
		0b00011111,
		0b00001110,
		0b00000100,
		0b00000000,
		0b00000000,

		0b00000000,			// 3 
		0b00111111,
		0b00011110,	
		0b00001100,
		0b00011110,
		0b00111111,
		0b00000000,
		0b00000000,
		
	    0b01010101,			// 4 andere Streifen
		0b01010101,
		0b01010101,
		0b01010101,	
		0b01010101,
		0b01010101,
		0b01010101,
		0b01010101,
		
		0b00101010,			// 5 andere Streifen
		0b00101010,
		0b00101010,
		0b00101010,	
		0b00101010,
		0b00101010,
		0b00101010,
		0b00101010,
				
		0b00011111,			// 6 andere Raute
		0b00011011,
		0b00010001,	
		0b00000000,
		0b00010001,
		0b00011011,
		0b00011111,
		0b00000000,

		0b00000000,			// 3 
		0b00111111,
		0b00011110,	
		0b00001100,
		0b00011110,
		0b00111111,
		0b00000000,
		0b00000000,	
	};

