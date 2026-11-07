/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*    FIFO.h							 */
/*                                       */
/*****************************************/

#ifndef _FIFO_H_
#define _FIFO_H_
#include <stdlib.h>
#define buffer_size 64
//------------------- FIFO ----------------------------
class clsFIFO                  // Klassendefinition
{                              // Eigenschaften
   public: unsigned char write_pointer; 
   public: unsigned char read_pointer;
   public: char buffersize;
   public: char buffer[buffer_size];
                               // Konstuktor
   public: clsFIFO (void);                    
                               // Methoden
   public: bool isfull (void); //  buffer voll = true
   public: bool isempty (void);//  buffer leer = true
   public: char get (void);    //  return byte
   public: void put (char value);  // return 0x00 ok
};      // end class FIFO
//------------------- FIFO ----------------------------
#endif

