 /*****************************************/
 /*    AVR Board1 Application Master      */
 /*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */
                     
 /*	                                      */
 /*		  AVRIO_I2C_Master.cpp		      */
 /*	  									  */
 /*                                       */
 /*****************************************/

 #include "AVRI2C.h"
 //------------------------------------------   Hardware ------------------------
 #define SLA_w 0x00
 #define SLA_r 0x01
 //  Die Adresse SLA   [A7...A0]  , die bei den folgenden Funktionen
 //  übergeben wird, wird um 1 bBit nach links geschoben
 //---------------------------------------  AVRIO1_write -------------------------
 void AVRIO1_write (unsigned char SLA, unsigned char* TWI_Data_Transmit_Bytes)			// write Slave (erst Adresse, danach Daten)
 {	I2C_start ();
	 I2C_write_8Bit (SLA_w | (SLA<<1));
	 I2C_write_8Bit (TWI_Data_Transmit_Bytes[0]);
	 I2C_write_8Bit (TWI_Data_Transmit_Bytes[1]);
	 I2C_stop ();
 }
 //---------------------------------------  AVRIO1_read (SLA,TWI_Data_Bytes) -----
 void AVRIO1_read (unsigned char SLA, unsigned char* TWI_Data_Receive_Bytes)			// read Slave (erst Adresse, danach Daten)
 {   unsigned char index = 0;
	 I2C_start ();
	 I2C_write_8Bit (SLA_r | (SLA<<1));
	 TWI_Data_Receive_Bytes[index] = I2C_read_8Bit_ACK ();
	 index++;
	 TWI_Data_Receive_Bytes[index] = I2C_read_8Bit_NACK ();
	 I2C_stop ();
 }
 //---------------------------------------------------------------
