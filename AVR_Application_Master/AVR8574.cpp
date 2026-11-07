/*     FHDAVR Parallelport PCF 8574		 */
/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    AVRI2C.cpp						 */
/*	  									 */
/*                                       */
/*****************************************/

#include "AVRI2C.h"
//------------------------------   Hardware ---------------------

#define PCF8574_Base_adr_w 0x40
#define PCF8574_Base_adr_r 0x41
//   PCF8574 user_adress  (A2,A1,A0)

//---------------------------------------  PCF8574_writebyte ----
void PCF8574_writebyte (unsigned char adress, unsigned char value)
{	I2C_start ();
    I2C_write_8Bit (PCF8574_Base_adr_w|((adress&0x07)<<1));				// schreiben der Adressierung, danach schreiben der I/O 
    I2C_write_8Bit (value);
	I2C_stop ();
}
//---------------------------------------  PCF8574_readbyte ----
unsigned char PCF8574_readbyte (unsigned char adress)
{	unsigned char temp;
    I2C_start ();
    I2C_write_8Bit (PCF8574_Base_adr_r|((adress&0x07)<<1));				// lesen der Adressierung, danach schreiben der I/O 
    temp = I2C_read_8Bit_NACK ();
	I2C_stop ();
	return (temp);

}
//---------------------------------------------------------------





