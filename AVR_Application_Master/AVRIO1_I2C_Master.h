/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */
 /*									     */
/*    Ansteuerung I2C BUS mit TWI des uC */
/*                                       */
/*****************************************/


void AVRIO1_write (unsigned char SLA, unsigned char* TWI_Data_Transmit_Bytes);
void AVRIO1_read (unsigned char SLA, unsigned char* TWI_Data_Receive_Bytes);

