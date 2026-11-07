/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */
/*	                                     */
/*    File AVRI2C_Master_Slave.h         */
/*                                       */
/*    Ansteuerung I2C BUS mit TWI des uC */
/*****************************************/



//-------------------------------------    I2C MASTER ----------------------
void I2C_init  (void);
unsigned char I2C_start (void);
void I2C_repeated_start (void);
void I2C_stop (void);
unsigned char  I2C_write_8Bit (unsigned char value8);
unsigned char I2C_read_8Bit_ACK (void);
unsigned char I2C_read_8Bit_NACK (void);
//-------------------------------------    I2C SLAVE ------------------------
//--- Wenn der Master einen Block anfordert, wird ein Block mit
//--- der Anzahl RxTxBuffer  Bytes an den Master gesendet
//---------------------------------------------------------------------------
//-- jedes Byte mit einem [index] kann einzeln gelesen und geschrieben werden
//--  !! Die Bytes zueinander sind nicht konststent !!
void TWI_Slave_Init ( unsigned char TWI_myAddress);
unsigned char TWI_Receive_Byte (unsigned char index);
void TWI_Transmit_Byte (unsigned char index, unsigned char value);
//---------------------------------------------------------------------------
