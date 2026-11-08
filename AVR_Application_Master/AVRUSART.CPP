/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Mohamed                  */
/*	  Datum 09.07.2026                    */
/*	                                     */
/*    AVRUSART.cpp				         */
/*	  									 */
/*                                       */
/*****************************************/

//***********************   avrstudio includes ***************
#include <avr/io.h> 		// Ports, Registers definitions
#include <avr/interrupt.h>  // Interrupts definitions
#include <stdbool.h>        // bool
#include "FIFO.h"
#include "AVRUSART.h"
#include "Schrittmotor.h"
//******************************************************************   RS232 **************
clsFIFO FIFO_RX;  // FIFO objects      vom Computer zum Micro
clsFIFO FIFO_TX;  // FIFO objects      vom Mikro zum Computer
extern clsSchrittmotor Stepper;
//-----------------------------------------------------------------------------------------
void clsUSART::init (void)
{
    DDRD = DDRD & 0xFE;   					    // PD0 RX -> INPUT
	DDRD = DDRD |0x02;                          // PD1 EX -> OUTPUT
	UBRR0H = 0; 								// UART Controller
//	UBRR0L = 51;								// Set baud rate 51 9600 Baud at 8MHz
	UBRR0L = 25;								// Set baud rate 25 19299 Baud at 8MHz
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);				// Enable receiver and transmitter
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);			// Set frame format: 8data, 2stop bit
	UCSR0B = UCSR0B | (1 << UDRIE0);			// Enable Data Register empty interrupt!
	UCSR0B = UCSR0B | (1 << RXCIE0);			// Enable Receive complete Interrupt

}
//----------------------------------- elementary access to send data register--------------
void clsUSART::COM_TX (unsigned char value)
{	while ( !( UCSR0A & (1<<UDRE0)) ); 	// Wait for empty transmit buffer
	UDR0 = value;						// Put data into buffer, sends the data
}
//--------------------------------------------------------------   write char to buffer --- 
void clsUSART::send (unsigned char value)
	{ while (FIFO_TX.isfull())
	   {	UCSR0B = UCSR0B | (1 << UDRIE0);	// Enable Data Register empty interrupt!
	   }
	  cli ();	// interrupt sperren
	 FIFO_TX.put (value);
      UCSR0B = UCSR0B | (1 << UDRIE0);	// Enable Data Register empty interrupt!
	  sei ();	//interrupt freigeben
    }
//--------------------------------------------------------------   write String to buffer -
void clsUSART::send_String (const char* Text)
	{while (*Text != 0)
			{ send (*Text++);
			}
	}
void clsUSART::Drehzahl1_Ausgabe (char Zahl)						// Terminal Ausgabe für Drehzahl (0-9)
	{ 
		send_String("Die Drehzahl: ");
		send(Zahl);
		send_String(" wurde gesetzt und die Drehrichtung ist ");
		if (Stepper.get_direction()==links) send_String("links.\n");
		else send_String("rechts.\n");
		send(0x0D);  
		
	}

void clsUSART::Drehzahl2_Ausgabe (char Zahl1, char Zahl2)			// Terminal Ausgabe für Drehzahl (10-99)
	{
		send_String("Die Drehzahl: ");
		send(Zahl1);
		send(Zahl2);
		send_String(" wurde gesetzt und die Drehrichtung ist ");
		if (Stepper.get_direction()==links) send_String("links.\n");
		else send_String("rechts.\n");
		send(0x0D);  
	}

void clsUSART::Fehler_Ausgabe (char Fehler)			
	// Terminal Ausgabe für Fehler
	// Fehlercode 1: Es koennen maximal zweistellige Zahlen eingegeben werden!
	// Fehlercode 2: Geben Sie eine Drehzahl !=0 zwischen -99 und 99 ein! 
	// Fehlercode 3: Ungueltige Eingabe!
	// Fehlercode 4: Ein Endschalter wurde betaetigt!
{
	switch(Fehler)						
	{
		case 1:
		{
			send_String("Es koennen maximal zweistellige Zahlen eingegeben werden!\n");
		} 
		break;
		case 2:
		{
			send_String("Geben Sie eine Drehzahl !=0 zwischen -99 und 99 ein! \"?\" fuer Hilfe\n");
		}
		break;
		case 3:
		{
			send_String("Ungueltige Eingabe! \"?\" fuer Hilfe\n");
		} 
		break;
		case 4:
		{
			send_String("Ein Endschalter wurde betaetigt!\n");
		}
		break;
	}
send(0x0D);
}
//-------------------------------------------------------------- Zeichen da? --------------
bool clsUSART::rec_buffer_empty (void)                       //  Char in buffer?
	{return (FIFO_RX.isempty ());
	}
//-------------------------------------------------------------- Zeichen lesen ------------
unsigned char clsUSART::reveive ()                           // get char
    {return FIFO_RX.get();
	}
//--------------------------------------------------------------  Interruptroutinen -------

ISR(USART_RX_vect)                       			//  USARTD0 Receive Complete Interrupt
{   FIFO_RX.put (UDR0);
}

ISR(USART_UDRE_vect)								// USART Data Register Empty Interupt
{   if ( FIFO_TX.isempty ())				
         {	UCSR0B = UCSR0B & ~(1 << UDRIE0);		// if no more byte in FIFO
		 }                             				// Disable Data Register Empty Interrupt 	
    else
         {clsUSART::COM_TX (FIFO_TX.get());         // Transmit next Byte
		 }
}

ISR(USART_TX_vect)									// Transmit Complete Interrupt
{   while (1);    									    // not used
}
//---------------------------------------------------------- that´s it for RS232 ----------


