/********************************************/
/*    AVR Board1 Application Master			*/
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*											*/
/*    CPU ATMEGA8/168                       */
/*    MODULE AVRI2C.cpp                     */
/*    Ansteuerung I2C BUS mit TWI des uC    */
/********************************************/

#include  <avr/io.h>
#define TimeOut 20  
//-------------------------- TWI  Two Wire Interface I2C -----------------------
void wait_TWINT (unsigned char errorcode)
{unsigned int waitcount = 0;
 while ((!(TWCR & (1<<TWINT))) && (waitcount <= TimeOut))
	{waitcount++;};             // warten bis fertig oder TimeOut
}
//------------------------------------------------------- I2C Start ------------
void I2C_start(void)
{	
 TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN); // Send START condition
 wait_TWINT ('S');                        
}   
//------------------------------------------------------- I2C repeated Stop ----
 void I2C_repeated_start(void)
{
 TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN); // Send START condition
 wait_TWINT ('R');                        
}                                            
//------------------------------------------------------- I2C Stop --------------
void I2C_stop (void)
{
 TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO); // Transmit STOP condition
}
//------------------------------------------------------- I2C_init --------------
void I2C_init (void)
{TWBR = 1; 				// Bitrate (Teilerfaktor) für 5us Taktperiode I2C
 TWSR = 1;  				// Vorteiler TWPS = 1
}
//------------------------------------------------------- I2C_write_8Bit --------
void I2C_write_8Bit (unsigned char value8)
{
 TWDR = value8;	    					 // Adresse
 TWCR = (1<<TWINT) | (1<<TWEN);			 // Clear TWINT bit in TWCR to 
 wait_TWINT ('W');						 // start transmission of byte
}
//------------------------------------------------------- I2C_read_8Bit_ACK -----
unsigned char I2C_read_8Bit_ACK (void)
{unsigned char readvalue = 0;
 TWCR = (1<<TWINT)|(1<< TWEA)|(1<<TWEN);	// Empfang starten und ACK senden
 wait_TWINT ('A');
 readvalue = TWDR;
 return (readvalue);
}
//------------------------------------------------------- I2C_read_8Bit_NACK ----
unsigned char I2C_read_8Bit_NACK (void)
{unsigned char readvalue = 0;
 TWCR = (1<<TWINT)|(1<<TWEN);			// Empfang starten und NACK senden
 wait_TWINT ('N');
 readvalue = TWDR;
 return (readvalue);
}
//-------------------------------------------------------------------------------
