/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*	  FHDAVR Parallelport PCF 8574		 */
/*                                       */
/*****************************************/
      
/*	  MODULE FHD8574.h				           */

/*    Deklarationen für PCF8574                */
/*    Ansteuerung I2C BUS mit TWI des uC       */
/*    --------------------------------------   */


void PCF8574_writebyte (unsigned char adress, unsigned char value);
unsigned char PCF8574_readbyte (unsigned char adress);
