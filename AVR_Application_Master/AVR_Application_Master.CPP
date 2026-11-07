/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */
/*	                                     */
/*    AVR_Application_Master.cpp         */
/*	  									 */
/*                                       */    
/*****************************************/
//-------------------------------------------------------------------------
#include <avr/io.h>					//  look at iom168p.h
#include <avr/interrupt.h> 

//-------------------------------------------------------------------------
#include "AVRI2C.h"
#include "AVRIO1_I2C_Master.h"
#include "AVRKeyboard_NOTAUS.h"
#include "AVRLCD.h"
#include "AVRTimer0.h"
#include "AVRUSART.h"
#include "Schrittmotor.h"
//---------------------------------------------------  HARDWARE -----------
#define LED 0x08					// PORTC Bit 4
#define DDR_LED DDRC
#define PORT_LED PORTC

#define SLA_AVRIO1 0x6C				// SLAVE Adresse von AYRIO1
#define Anzahl_Bytes 2				// Anzahl der Bytes, die pro Block

char enter=0, endschalt=0, meldung=0;
unsigned int drehzahl=0, speed=0;
char eingabe[5]={0,0,0,0,0};
int varein=0;
unsigned char Master_Transmit_Bytes[Anzahl_Bytes];
//--------------------------------------------------- Objects -------------
clsLCD LCD1;
clsKeyboard Keyboard1;
clsUSART COM1;
clsSchrittmotor Stepper(Stop, rechts , 0xFF00); //init
//--------------------------- Application  init ---------------------------

void Application_init (void)
{ 	PORT_LED = PORT_LED & ~LED;	// LED on	
    DDRC = DDRC | LED;		    // Port Bit LED set to Output
    Timer0_init ();				// initialize time Slice System
    COM1.init ();
	I2C_init ();
    LCD1.init ();
    sei ();                     // Enable global Interrupt
}
//--------------------------- Application 10ms ----------------------------
							// Fehlercode 1: Es koennen maximal zweistellige Zahlen eingegeben werden!
							// Fehlercode 2: Geben Sie eine Drehzahl !=0 zwischen -99 und 99 ein!
							// Fehlercode 3: Ungueltige Eingabe!
							// Fehlercode 4: Ein Endschalter wurde betaetigt
void Application_10ms ()
{   

// REPLACE WITH THIS (fixed):
// Schalter gedrückt ? Motor stoppen
// Switch pressed ? stop motor
if (lese_Endschalter()==0x0001 && Stepper.get_direction()==links && Stepper.get_run()!=notaus)
{
    Stepper.stop();
    endschalt = 1;
}
if (lese_Endschalter()==0x0002 && Stepper.get_direction()==rechts && Stepper.get_run()!=notaus)
{
    Stepper.stop();
    endschalt = 2;
}

// Schalter losgelassen ? Motor wieder starten
// Switch released ? restart motor
if (lese_Endschalter()==0 && endschalt!=0 && Stepper.get_run()!=notaus && drehzahl!=0)
{
    Stepper.start();
    endschalt = 0;
    meldung = 0;
}		// Endschalter rechts (S2)

	unsigned char in_char;
	if (!COM1.rec_buffer_empty())			 //wenn der Buffer nicht leer ist wird ausgeführt rec_buffer_empty()=1 wenn Buffer leer
	{ in_char = COM1.reveive ();			 //Buffer auslesen
		if (in_char==13) enter=1;			 //Abfrage auf Enter
		else 
		{
			eingabe[varein]=in_char;		//Char-Array schreiben
			varein++;						//Stelle des Char-Array schieben
		}
	}
	
	if(enter==1 && Stepper.get_run()==notaus)		//Abfrage auf NOT-AUS 
	{
		COM1.send_String("NOT-AUS betaetigt!\n");
		COM1.send(0x0D);
		enter=0;							// rücksetzen der Variablen
		varein=0;
		eingabe[0]=0;
		eingabe[1]=0;
		eingabe[2]=0;
		eingabe[3]=0;
		eingabe[4]=0;
	}
	if(enter==1 && Stepper.get_run()!=notaus)			// Enter wurde gedrückt
	{
		
		switch(eingabe[0])								//Switch auf erste Stelle des Char-Array
		{
			case 'd':
			case 'D': 
				if (eingabe[1]=='-')					//Negative Drehzahl eingeben
				{
					if (eingabe[4]>47 && eingabe[4]<58)	
					{
						COM1.Fehler_Ausgabe(1);
						break;
					}
					if (eingabe[2]==0)					// keine Zahl eingegeben 
					{
						COM1.Fehler_Ausgabe(2);
						break;
					}
					else       
					{                                  // Motordrehrichtung Links und Eingaben alle i.O. d/D-xx
						Stepper.set_direction(links);
						if (eingabe[3]>47 && eingabe[3]<58)		//zweistellige Zahl
						{
							drehzahl=(eingabe[2]-48)*10+(eingabe[3]-48);
							COM1.Drehzahl2_Ausgabe(eingabe[2], eingabe[3]);
						}
						else
						{   
							drehzahl=(eingabe[2]-48);			// einstellige Zahl
							COM1.Drehzahl1_Ausgabe(eingabe[2]);
						}
						if (drehzahl==0) Stepper.stop();		// Drehzahl = 0 -> Motor steht
						else speed=65535-(9766/drehzahl); //9765625=60000000000/(48*128*1000)
						Stepper.set_speed(speed);
					}
					if (endschalt==2) endschalt=0, meldung=0;	// Endschaltmeldungen rücksetzen nach erfolgreicher Änderung der Drehrichtung
					break;
				}
				if (eingabe[1]=='+')					//positive Drehzahl eingeben mit +
				{
					if (eingabe[4]>47 && eingabe[4]<58)
					{
						COM1.Fehler_Ausgabe(1);
						break;
					}
					if (eingabe[2]==0)			// keine Zahl eingegeben 
					{
						COM1.Fehler_Ausgabe(2);
						break;
					}
					else
					{
						Stepper.set_direction(rechts);
						if (eingabe[3]>47 && eingabe[3]<58)			// zweistellige Zahl einlesen
						{
							drehzahl=(eingabe[2]-48)*10+(eingabe[3]-48);
							COM1.Drehzahl2_Ausgabe(eingabe[2], eingabe[3]);
						}
						else
						{
							drehzahl=(eingabe[2]-48);				// einstellige Zahl einlesen
							COM1.Drehzahl1_Ausgabe(eingabe[2]);
						}
						if (drehzahl==0) Stepper.stop();
						else speed=65535-(9766/drehzahl); //9766=60000000/(48*128*drehzahl)
						Stepper.set_speed(speed);
					}
					if (endschalt==1) endschalt=0, meldung=0;
					break;
				}
				if ((eingabe[1]>47 && eingabe[1]<58) || eingabe[1]==0)		//positive Drehzahl eingeben ohne Vz
				{
					if (eingabe[3]>47 && eingabe[3]<58)
					{
						COM1.Fehler_Ausgabe(1);
							// Fehlercode 1: Es koennen maximal zweistellige Zahlen eingegeben werden!
							// Fehlercode 2: Geben Sie eine Drehzahl !=0 zwischen -99 und 99 ein!
							// Fehlercode 3: Ungueltige Eingabe!
							// Fehlercode 4: Ein Endschalter wurde betaetigt
						break;
					}
					if (eingabe[1]==0)
					{
						COM1.Fehler_Ausgabe(2);
						break;
					}
					else
					{						
						Stepper.set_direction(rechts);
						if (eingabe[2]>47 && eingabe[2]<58)
						{
							drehzahl=(eingabe[1]-48)*10+(eingabe[2]-48);
							COM1.Drehzahl2_Ausgabe(eingabe[1], eingabe[2]);
						}
						else
						{
							drehzahl=(eingabe[1]-48);
							COM1.Drehzahl1_Ausgabe(eingabe[1]);
						}
						if (drehzahl==0) Stepper.stop();
						else speed=65535-(9766/drehzahl); //9766=60000000/(48*128*drehzahl)
						Stepper.set_speed(speed);
					}
					if (endschalt==1) endschalt=0, meldung=0;
					break;
				}
				if (eingabe[1]!='-' && eingabe[1]!='+' && !(eingabe[1]>47 && eingabe[1]<58))
				{
					COM1.Fehler_Ausgabe(1);
				}
			break;

			case 's':
			case 'S': 
				if (eingabe[1]==0) 
				{
					Stepper.stop();
					COM1.send_String("Motor steht.\n");
					COM1.send(0x0D);
				}
				else
				{
					COM1.Fehler_Ausgabe(3);
				}
			break;

			case 'r':
			case 'R': 
				if(eingabe[1]==0)
				{
					if(drehzahl!=0)
					{
						if((endschalt!=1 && Stepper.get_direction()==links) || (endschalt!=2 && Stepper.get_direction()==rechts))
						{
							Stepper.start();
							COM1.send_String("Motor laeuft.\n");
							COM1.send(0x0D);
			
						}
						else
						{
							COM1.Fehler_Ausgabe(4);
						}
					}
					else
					{
						COM1.Fehler_Ausgabe(2);
					}
				}
				else
				{
					COM1.Fehler_Ausgabe(3);
				}
			break;

			case 'o':
			case 'O': 
				if(eingabe[1]==0)
				{
					Stepper.off();
					COM1.send_String("Motor stromlos\n");
					COM1.send(0x0D);
				}
				else
				{
					COM1.Fehler_Ausgabe(3);
				}
			break;
			
			case '?':
				if(eingabe[1]==0)
				{
					COM1.send_String("D/dxxx(Enter)  Drehzahl(-)xx in Umdrehungen/Minute");
					COM1.send (0x0D);
					COM1.send_String("S/s(Enter)     Stopp");
					COM1.send (0x0D);
					COM1.send_String("R/r(Enter)     Run");
					COM1.send (0x0D);
					COM1.send_String("O/o(Enter)     Off (stromlos)\n");
					COM1.send (0x0D);
				}
				else
				{
					COM1.Fehler_Ausgabe(3);
				}
			break;
			
			default:
				COM1.Fehler_Ausgabe(3);
			break;
		}
		enter=0;				// Rücksetzen aller Variablen
		varein=0;
		eingabe[0]=0;
		eingabe[1]=0;
		eingabe[2]=0;
		eingabe[3]=0;
		eingabe[4]=0;
	}
}
//--------------------------- Application 100ms ---------------------------
void Application_100ms ()
{   
	if (Stepper.get_run()==notaus)						// Abfrage NOT-AUS LCD
	{
		LCD1.gotoxy(0,0);
		LCD1.PrintString("    NOT-AUS     ");
		LCD1.gotoxy(0,1);
		LCD1.PrintString("   betaetigt    ");
		if (meldung==0 || (meldung==1 && endschalt!=0))
		{
			COM1.send_String("NOT-AUS betaetigt\n");
			COM1.send(0x0D);
			meldung=2;
		}
	}
	if (endschalt==1 && Stepper.get_run()!=notaus)
	{						
		LCD1.gotoxy(0,0);
		LCD1.PrintString("Endschalter links");
		LCD1.gotoxy(0,1);
		LCD1.PrintString("   betaetigt    ");
		if (meldung==0)							// Einmalmeldung am Terminal 
		{
			COM1.send_String("Endschalter links betaetigt\n");
			COM1.send(0x0D);
			meldung=1;
		}
	}
	if (endschalt==2  && Stepper.get_run()!=notaus)
	{
		LCD1.gotoxy(0,0);
		LCD1.PrintString("Endschalter rechts");
		LCD1.gotoxy(0,1);
		LCD1.PrintString("   betaetigt    ");
		if (meldung==0)
		{
			COM1.send_String("Endschalter rechts betaetigt\n");
			COM1.send(0x0D);
			meldung=1;
		}
	}
	if (endschalt==0 && Stepper.get_run()!=notaus)			// Zustandsanzeige LCD
	{
		LCD1.gotoxy(0,0);
		LCD1.PrintString("Zustand: ");
		if (Stepper.get_run() == Run) LCD1.PrintString("Run    ");
		if (Stepper.get_run() == Stop) LCD1.PrintString("Stopp   ");
		if (Stepper.get_run() == Off) LCD1.PrintString("Off     ");
		LCD1.gotoxy(0,1);
		LCD1.PrintString("Soll:");
		LCD1.PrintZahl8(drehzahl);
		LCD1.PrintString(" U/min");
		if(Stepper.get_direction()==links && drehzahl!=0) LCD1.PrintString(" L");
		if(Stepper.get_direction()==rechts && drehzahl!=0) LCD1.PrintString(" R");
		if(drehzahl==0) LCD1.PrintString("  ");
	}
	

	if (Stepper.get_run()==Run)									// Übergabe Drehzal über I2C-Bus für LED-Matrix
	{	
		if (Stepper.get_direction()==links) 	drehzahl |= 0x80;	// Vorzeichen für Drehzahl wird hinzugefügt. Erstes Bit wird "1" gesetzt
		Master_Transmit_Bytes[0] =   (char)(drehzahl &0xFF);  // Unteres Byte 8 Bit
		Master_Transmit_Bytes[1] =    (char)(drehzahl>>8);    // die restl. 2 Bits	
		drehzahl &= 0x7F;										// Vorzeichen für Drehzahl wird entfernt. Erstes Bit wird "0" gesetzt
	}
	else
	{
		Master_Transmit_Bytes[0] =   (char)(0);  // Unteres Byte 8 Bit
		Master_Transmit_Bytes[1] =    (char)(0);    // die restl. 2 Bits
	}
	AVRIO1_write (SLA_AVRIO1,Master_Transmit_Bytes);// 2 Bytes schreiben
}
//--------------------------- Application 500ms ---------------------------
void Application_500ms ()
{   PORT_LED ^= LED;  // LED toggle
	
}
//--------------------------- Application 500ms ---------------------------
void Application_1s (void)
{	

}
//--------------------------- Application IDLE ----------------------------

extern unsigned char Timer_Stepper_OVF;
void Application_Idle (void)
{
	// REPLACE WITH THIS (fixed):
	if (Keyboard1.Keyboard_gedrueckt() == 1)
	{
		if (Stepper.get_run() == notaus)
		{
			Stepper.start();   // resume with same speed & direction
			meldung = 0;
		}
		else
		{
			Stepper.NOT_AUS(); // trigger emergency stop
		}
	}
	
	if (Timer_Stepper_OVF)											// Aufruf Do-Step
	{  
		Timer_Stepper_OVF = 0;
		Stepper.do_step();
	}
}

