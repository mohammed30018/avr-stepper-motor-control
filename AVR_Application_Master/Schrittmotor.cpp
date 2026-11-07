/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    Schrittmotor.cpp			         */
/*	  									 */
/*                                       */
/*****************************************/

#include <avr/io.h>
#include <avr/interrupt.h> 
#include "Schrittmotor.h"
extern clsSchrittmotor Stepper;
//-----------------------------------------------------------------------
volatile unsigned char Timer_Stepper_OVF = 0;  // Semaphore 
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#include "AVR8574.h"
//-------------------------------------------------  HARDWARE -----------
// Adresse I2C Parallelport für Motor und S1 und S2
#define PCF8574_Motor 3
// Belegung der Bits:
// PM7      PM6      PM5      PM4      PM3      PM2      PM1      PM0
// L3       L4       EN34     S2       L1       L2       EN12     S1

// Enable = 1 -> L1-L4 aktiv, sonst hochohmig
// S1 und S2 muss mit 1 beschrieben werden,
// damit der Zustand des Schalters gelesen werden kann
//-----------------------------------------------------------------------
// Ansteuerung der Spulen des Motors im Halbschritt-Verfahren
const unsigned char Phase [8] = { 0xA0, 0x20, 0x60, 0x40,
                                  0x50, 0x10, 0x90, 0x80 };
// ------------------------------ Funktionen zur Motorsteuerung ---------
void SetMotorPort (unsigned char step)  // Hardwarezugriff NUR hier 
{  unsigned char PM_out = 0xFF;
   PM_out = PM_out & ( (Phase [step] | 0x3F) &
                       ((Phase [step]>>2) | 0xF3));
   PCF8574_writebyte (PCF8574_Motor,PM_out);
}
//--------------------------------  Endschalter S2 und S1 ---------------
unsigned char lese_Endschalter (void)       //  S1 geschlossen: Bit 0 = 1
{  unsigned char PM_in,temp=0;              //  S2 geschlossen: Bit 1 = 1
   PM_in = PCF8574_readbyte (PCF8574_Motor);
   temp = ( ((~PM_in) & 0x01) | (((~PM_in)>>3) & 0x02) );
   return (temp);
}
//-----------------------------------------------------------------------
//---------- Definitionen der Methoden von der Klasse Schrittmotor ------
//------------------------------------------ Konstruktor ----------------
clsSchrittmotor::clsSchrittmotor(unsigned char RUN, unsigned char DIR,
                           unsigned int RELOAD)    
{//--------------- LED --------------------------------------------------
 LED_aktiv_PORT &= ~LED_aktiv_PIN;   // blinkt bei ISR
 LED_aktiv_DDR |= LED_aktiv_PIN;     // fuer Schrittmotor
 //--------------- Eigenschaften ----------------------------------------
 this->run = RUN;
 this->direction = DIR;
 this->reload = RELOAD;
 //this->step = step;
 //---------------------       init Timer 1 -----------------------------
    TCCR1A = 0;	// Timer 1, Compare and Waveform, egal
    TCCR1B = 5; // 0 = no clock source  , stopped
                // 1 = clkio/1, on
				// 2 = clkio/8, on
				// 3 = clkio/64   	, on
				// 4 = clkio/256   	, on
				// 5 = clkio/1024	, on
				// 6 = ext clock T1 PIN, falling edge
                // 7 = ext clock T1 PIN, rising edge
    TCCR1C = 0; // wird hier nicht benötigt
    TIMSK1  = (1<<TOIE1);	// Timer Mask Register 1
                            // Timer overflow interrupt enable
	TCNT1 = this->reload;         // Startwert, Timer zählt aufwärts
//------------------------------------------------------------------
}
//--------------------------- Interrupt Service Routine ------------
ISR (TIMER1_OVF_vect)
{	LED_aktiv_PORT ^= LED_aktiv_PIN;
	TCNT1 = Stepper.reload;         // Startwert, Timer zählt aufwärts
	Timer_Stepper_OVF = 1;
}
//------------------------------------------------------------------
void clsSchrittmotor::set_speed (unsigned int s)
{this->reload = s;
}
//------------------------------------------------------------------
void clsSchrittmotor::set_direction (unsigned char value)
{this->direction = value;
}
unsigned char clsSchrittmotor::get_direction()
{
	return(this->direction);
}
//------------------------------------------------------------------
void clsSchrittmotor::start (void)
{
 this->run = Run;
}
//------------------------------------------------------------------
void clsSchrittmotor::stop (void)
{
	this->run = Stop;
}
void clsSchrittmotor::off (void)
{
	this->run = Off;
}
void clsSchrittmotor::NOT_AUS(void)
{
	this->run = notaus;
}
unsigned char clsSchrittmotor::get_run()
{
	return(Stepper.run);
}
//---------------------------------- Destruktor --------------------
clsSchrittmotor::~clsSchrittmotor()
{
}
//------------------------------------------------------------------
void clsSchrittmotor::do_step(void)
{
 if (Stepper.run == Run)// die ursprüngliche Step wird aus geführt 
    {if (Stepper.direction == links && lese_Endschalter() != 0x01)
			{Stepper.step--;
			 Stepper.step &= 0x07;
			}
	 if (Stepper.direction == rechts && lese_Endschalter() != 0x02)
			{Stepper.step++;
			 Stepper.step &= 0x07;
			}
	SetMotorPort (Stepper.step);
	}
	if (Stepper.run == Off) PCF8574_writebyte (PCF8574_Motor,0x11);		// 0001 0001 alle Spulen aus; Enable=0 -> hochohmig; S1 und S2 = 1 muss beschrieben werden
	if (Stepper.run == notaus) PCF8574_writebyte (PCF8574_Motor,0x33);		// 0011 0011 Spule L3 L4 kurzgeschlossen und L1 L2 kurzgeschlossen; S1 und S2 = 1 muss beschrieben werden
}
//------------------------------------------------------------------