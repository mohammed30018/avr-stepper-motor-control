/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    File AVRI2C_Master_Slave.cpp       */
/*                                       */
/*										 */
/*****************************************/


//-------------------------------------------------------------------------
#include <avr/io.h> //  siehe iom168p.h
#include <avr/interrupt.h>
//-------------------------------------------------------------------------
#include "AVRTimer0.h"
#include "AVRI2C_Master_Slave.h"
#include "AVRLEDMatrix.h"
//---------------------------------------------------  HARDWARE -----------
#define LEDBit_AVRIO1 0x04               // PORTB Bit 2
#define LED_PORT_AVRIO1 PORTB
#define LED_DDR_AVRIO1 DDRB
//-------------------------------------------------------------------------
#define SLA 0x6C                         // SLAVE-Adresse AVRIO1

//--------------------------- Application  init ---------------------------
void Application_init (void)
{       LED_PORT_AVRIO1 &= ~LEDBit_AVRIO1;    // LED on
	LED_DDR_AVRIO1 |= LEDBit_AVRIO1;      // Port Bit -> Output
	Timer0_init ();                       // initialize time Slice System
	TWI_Slave_Init (SLA);
	initLED_Matrix ();
	sei ();                               // Enable global Interrupt
}
//--------------------------- Application 1ms -----------------------------
void Application_1ms ()
{  LED_mux ();//Sie multiplext die LED Anzeige 
}
//--------------------------- Application 10ms ----------------------------
void Application_10ms ()
{
}
//--------------------------- Application 100ms ---------------------------
void Application_100ms ()
{
	unsigned int in = 0;
	in |= TWI_Receive_Byte(0);// Daten auslesen vom Master über TWI_Receive_Byte und an die LEDMatrix_printdrehzahl übergeben 
	in |= (TWI_Receive_Byte(1) << 8);
	LEDMatrix_PrintDrehzahl(in);
}
//--------------------------- Application 500ms ---------------------------
void Application_500ms ()
{
	LED_PORT_AVRIO1 ^= LEDBit_AVRIO1;// LED Lebenzeichen 
}
//--------------------------- Application 500ms ---------------------------
void Application_1s (void)
{
}
//--------------------------- Application IDLE ----------------------------

void Application_Idle (void)
{
}
//------------------------------------------------------- ENDE ------------


