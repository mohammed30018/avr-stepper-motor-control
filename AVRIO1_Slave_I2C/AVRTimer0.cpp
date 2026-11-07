/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*     MODULE AVRTimer0.cpp              */
/*	  								  	 */
/*    Timer 0 with 10ms Interrupt        */
/*    LED at PC3 blinks as lifesign      */
/*****************************************/

//------------------------------------------------------------------------------
#include <avr/io.h> 			//  look at iom168p.h
#include <avr/interrupt.h>

#include "AVRApplication.h"

//------------------------------------------------------------------------------

volatile unsigned char clock_T0 = 0, clock_1ms = 0, clock_10ms = 0, clock_100ms = 0;
volatile unsigned char clock_500ms = 0, clock_1s = 0;

//--------------------------------------------------- Timer 0 ------------------

#define T0_reload 131		//  overfllow every  1ms at CPU clock 8 MHZ and clk/64

//---------------------------------------------------  Timer0 init -------------
void Timer0_init (void)     //
{
	// t0_overflow = (256-T0_reload) * Period
	TCCR0B =3;    			// 0 = no clock source  , stopped
	// 1 = clkio/1		, on
	// 2 = clkio/8   	, on
	// 3 = clkio/64   	, on
	// 4 = clkio/256   	, on
	// 5 = clkio/1024	, on
	// 6 = ext clock TO PIN, falling edge
	// 7 = ext clock TO PIN, rising edge

	TCNT0 = T0_reload;		// Timer Count Reload
	TIMSK0 |= (1<<TOIE0);	// Timer 0 Overflow Int Enable
}

//------------------------------------------- Timer 0 Interrupt Servuce Routine -
ISR (TIMER0_OVF_vect)
{
	TCNT0 = T0_reload;		// Timer Count Reload
	clock_T0++;

}

//--------------------------------------------------------------- main
int main (void)

{ 	Application_init ();
	
	while (1)			//-------------------------- for ever ------------------
	{					//------------------------- time slices ----------------
		
		if (clock_T0 > 0)
		{	clock_T0 = 0;
			clock_1ms++;
			Application_1ms ();				// 1 ms
		}
		if (clock_1ms >= 10)
		{	clock_1ms = 0;
			clock_10ms++;
			Application_10ms ();			// 10 ms
		}
		if (clock_10ms >= 10)
		{	clock_10ms = 0;
			clock_100ms++;
			Application_100ms ();			// 100ms
		}
		if (clock_100ms >= 5)
		{	clock_100ms = 0;
			clock_500ms++;
			Application_500ms ();			// 500ms
		}

		if (clock_500ms >= 2)							// 1s
		{	clock_500ms = 0;
			Application_1s ();
		}
		Application_Idle ();  							// IDLE
	}
}

//------------------------------------------------------------- end main -------
