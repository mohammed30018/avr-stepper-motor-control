/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */
/*	                                     */
/*    File AVRI2C_Master_Slave.c         */
/*                                       */
/*    Ansteuerung I2C BUS mit TWI des uC */
/*****************************************/


#include  <avr/io.h>
#include <avr/interrupt.h>
//-------------------------------------------------------------------
#define TimeOut 100
enum {I2C_ok,
      I2C_Timeout,
      I2C_NACKreceived,
      I2C_DAW_NA,
      I2C_Arbitrationlost
     };
//------------------------------------ MASTER -----------------------

//-------------------------------------------------------------------
unsigned char wait_TWINT (void)
{ unsigned int waitcount = 0;
  while ((!(TWCR & (1<<TWINT))) && (waitcount <= TimeOut))
        {waitcount++;};             // warten bis fertig oder TimeOut
  if (waitcount > TimeOut)
      {return (I2C_Timeout);
      }
  if ((TWSR & 0xFC) == 0x20)        // SLA+W has been transmitted;
      {return (I2C_NACKreceived);   // NOT ACK has been received
      }
  if ((TWSR & 0xFC) == 0x30)        // Data has been transmitted;
      {return (I2C_DAW_NA);         // NA
      }
  if ((TWSR & 0xFC) == 0x38)        //  Arbitration lost
      {return (I2C_Arbitrationlost);
      }
     return (I2C_ok);
}
//--------------------------------------------- I2C Start ------------
unsigned char  I2C_start(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN); // Send START condition
  return (wait_TWINT ()); 
} 
//-------------------------------------------- I2C repeated Stop -----
unsigned char I2C_repeated_start(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN); // Send START condition
  return (wait_TWINT ());
} 
//-------------------------------------------- I2C Stop --------------
void I2C_stop (void)
{
  TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO); //  Transmit STOP condition
}
//--------------------------------------------------------------------
void I2C_init (void)
{ TWBR = 4;                    // Bitrate (Teilerfaktor) für Taktperiode I2C
  TWSR = 1;                    // Vorteiler TWPS = 1
}
//--------------------------------------------------------------------
unsigned char I2C_write_8Bit (unsigned char value8)
{
  TWDR = value8;	                            // Adresse
  TWCR = (1<<TWINT) | (1<<TWEN);             // Clear TWINT bit in TWCR to 
  return ( wait_TWINT ());                   // start transmission of byte
}
//---------------------------------------------------------------------
unsigned char I2C_read_8Bit_ACK (void)
{ unsigned char readvalue = 0;
  TWCR = (1<<TWINT)|(1<< TWEA)|(1<<TWEN);	// Empfang starten und ACK senden
  wait_TWINT ();
  readvalue = TWDR;
  return (readvalue);
}
//---------------------------------------------------------------------
unsigned char I2C_read_8Bit_NACK (void)
{ unsigned char readvalue = 0;
  TWCR = (1<<TWINT)|(1<<TWEN);      // Empfang starten und NACK senden
  wait_TWINT ();
  readvalue = TWDR;
  return (readvalue);
}
//----------------------------------------------------------------------
//-------------------------------------- SLAVE -------------------------
#define RxTxBuffer 2       // Anzahl der Bytes für eine Übertragung
volatile unsigned char TWI_DataReceive[RxTxBuffer+1];
volatile unsigned char TWI_DataTransmit[RxTxBuffer+1];
volatile unsigned char TWI_DataTransmitpointer = 0;
volatile unsigned char TWI_DataReveivepointer = 0;
/****************************************************************************
  TWI State codes   !!!!  Quelle Application note AVR311
****************************************************************************/
// General TWI Master status codes                      
#define TWI_START                  0x08  // START has been transmitted  
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Transmitter status codes                      
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver status codes  
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter status codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = "0"); ACK has been received

// TWI Slave Receiver status codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = "0"
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition

//********************************************************************************************************

void TWI_Slave_Init ( unsigned char TWI_myAddress)   // [0..0x7F]
{
                          // TWSR: TWI Status 
  TWSR = 0xF8;            // Reset Wert => Prescaler Value = 1
  TWAR = TWI_myAddress<<1;                          // Slave Aresse um 1 Bit nach links schieben
                                                    // für [TWA6..TWA0]
                                                    // Bit 0 = TWGCE = 0 => general call enable 
                                                    // Adresse 0x00 nicht freigegeben.
  TWAMR= 0;                                         // alle Bits von TWI_myAddress sind gültig
  // TWCR Two Wire Control Register
  // Bit 7: TWINT TWI Interrupt Flag 
  // Bit 6: TWEA  TWI Enable Acknoledge Bit
  // Bit 5: TWSTA TWI START Condition Bit
  // Bit 4: TWSTO TWI STOP Condition Bit
  // Bit 3: TWWC  TWI Write Collision Flag
  // Bit 2: TWEN  TWI Enable Bit
  // Bit 1: Res   Reserved
  // Bit 0: TWIE  TWI Interrupt Enable
  TWCR = (1<<TWEN)|                                 // TWI Enable
         (1<<TWIE)|(0<<TWINT)|                      // ENABLE TWI Interupt.
         (1<<TWEA)|                                 // nächstes mal ACK senden
         (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
         (0<<TWWC);                                 // Write Collision
}

//---------------------------------------------------------------------- ISR TWI ---------------------
ISR (TWI_vect)
{  static unsigned char TWSR_temp;                  // nur zum testen...
   TWSR_temp = TWSR;
   switch (TWSR_temp)
    { //-------------------------------------------------------------------- Receiver
      case (TWI_SRX_ADR_ACK):
           { // Own SLA+W has been received ACK has been returned
            TWI_DataReveivepointer = 0;
            TWCR = (1<<TWEN)|                                 // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (1<<TWEA)|                                 // nächstes mal ACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;       
           }
      case (TWI_SRX_ADR_DATA_ACK):
           {// Previously addressed with own SLA+W; data has been received; ACK has been returned
            // TWI_DataReceive ist im TWDR Datenregister
            TWI_DataReceive[TWI_DataReveivepointer] = TWDR;
            TWI_DataReveivepointer++; // if (TWI_DataReveivepointer> RxTxBuffer) =>
                                      // Master sendet zuviele Bytes
            TWCR = (1<<TWEN)|                                 // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (1<<TWEA)|                                 // nächstes mal ACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;
           }
      case (TWI_SRX_ADR_DATA_NACK):
           {// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
            // TWI_value ist im TWDR Datenregister
            TWI_DataReceive[TWI_DataReveivepointer] = TWDR;
            TWI_DataReveivepointer++;  // if (TWI_DataReveivepointer> RxTxBuffer) =>
                                       // Master sendet zuviele Bytes
            TWCR = (1<<TWEN)|                                 // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (0<<TWEA)|                                 // nächstes mal NACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;
           }
      //-------------------------------------------------------------------- Transmitter 
      case (TWI_STX_ADR_ACK):
           {// Own SLA+R has been received; ACK has been returned
            TWI_DataTransmitpointer = 0;
            TWDR = TWI_DataTransmit[TWI_DataTransmitpointer];
            TWI_DataTransmitpointer++;
            TWCR = (1<<TWEN)|                                 // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (1<<TWEA)|                                 // nächstes mal ACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;       
           }
      case (TWI_STX_DATA_ACK):
           {// Data byte in TWDR has been transmitted; ACK has been received
            TWDR = TWI_DataTransmit[TWI_DataTransmitpointer];
            TWI_DataTransmitpointer++;
            TWCR = (1<<TWEN)|                                 // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (0<<TWEA)|                                 // nächstes mal NACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;       
           }
      default:   //------------------------------------------------ unbekannt ---------
           {// Es ist ein unvorhergesehener Fall oder ein Fehler aufgetereten?
            TWCR = (1<<TWEN)|                                // TWI Enable
                   (1<<TWIE)|(1<<TWINT)|                      // ENABLE TWI Interupt. CLEAR Interrupt FLAG
                   (1<<TWEA)|                                 // nächstes mal ACK senden
                   (0<<TWSTA)|(0<<TWSTO)|                     // bei SLAVE immer 0
                   (0<<TWWC);                                 // Write Collision
            break;
           }
    }  // end of switch
}
//--------------------------------------------- Slave receive ----------------------------------------
//--------------------------------------------- get_TWI_ByteA ----------------------------------------
unsigned char TWI_Receive_Byte (unsigned char index)
{     return (TWI_DataReceive[index]);
}
//--------------------------------------------- Slave transmit ---------------------------------------

//--------------------------------------------- put_TWI_ByteC ----------------------------------------   
void TWI_Transmit_Byte (unsigned char index, unsigned char value)
{     TWI_DataTransmit[index] = value;
}
//----------------------------------------------------------------------------------------------------
