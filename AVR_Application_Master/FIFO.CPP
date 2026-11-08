/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*	                                     */
/*    FIFO.cpp					         */
/*	  									 */
/*                                       */
/*****************************************/

#include "FIFO.h"
//--------- Methodendefinitionen FIFO -----------------
clsFIFO::clsFIFO (void)       // Konstruktor
{  this->write_pointer = 0;
   this->read_pointer = 0;
   this->buffersize = buffer_size;   //buffer_size=64
}
//-----------------------------------------------------
bool clsFIFO::isfull (void)  //  buffer full = true	
     { unsigned char temp;
       temp = write_pointer + 1;
       if (temp ==  (buffersize)) 
	   temp = 0; //wenn 
       if (temp == read_pointer)
          return (true);
       else
          return (false);
     }
//-----------------------------------------------------
bool clsFIFO::isempty (void)
     { if (write_pointer == read_pointer)  //wenn der write_pointer gleich dem read_pointer ist steht nichts im FIFO
          { return (true);  // buffer empty = true
          }
       else 
          { return (false); // buffer empty = false
          }
     }
//-----------------------------------------------------
char  clsFIFO::get (void)
     { unsigned char value;
       if  (isempty ())
         { value = buffer[read_pointer];
                //last value again (error)
         }
       else 
         { value =buffer[read_pointer];
           read_pointer = (read_pointer + 1);
           if (read_pointer == (buffersize))
               read_pointer=0; 
         }
       return (value); // return last first added byte 
      }
//-----------------------------------------------------
void clsFIFO::put (char value) // return 0x00 ok
      { buffer[write_pointer]=(char) value;
        write_pointer = (write_pointer + 1 );
        if (write_pointer == (buffersize)) write_pointer=0;  //FIFO-Speicher ist einmal vollgeschrieben der Zeiger wird zurückgesetzt
      }
//-----------------------------------------------------


