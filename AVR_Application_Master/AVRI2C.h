/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*    Ansteuerung I2C BUS mit TWI des uC */
/*                                       */
/*****************************************/


void I2C_init  (void);
void I2C_start (void);
void I2C_repeated_start (void);
void I2C_stop (void);
void I2C_write_8Bit (unsigned char value8);
unsigned char I2C_read_8Bit_ACK (void);
unsigned char I2C_read_8Bit_NACK (void);
/*********************************************/
