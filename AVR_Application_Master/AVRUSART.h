/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*    AVRUSART.h						 */
/*                                       */
/*****************************************/

#include "FIFO.h"
/*----------------------------------------------------*/
class clsUSART
{ 
  public: void init (void);
  public: void send (unsigned char value);
  public: void send_String (const char* Text);
  public: bool rec_buffer_empty (void);
  public: unsigned char reveive (); 
  public: void Drehzahl1_Ausgabe (char Zahl);
  public: void Drehzahl2_Ausgabe (char Zahl1, char Zahl2);
  public: void Fehler_Ausgabe (char Fehler);
  public: static void COM_TX (unsigned char value); 
};
/*----------------------------------------------------*/
