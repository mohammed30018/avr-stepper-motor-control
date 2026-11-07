/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*    MODULE AVRLED_Matrix.h	         */
/*    4 x Siebensegment					 */
/*										 */
/*****************************************/

//--------------------------------------------------
void initLED_Matrix (void);

void LED_mux (void);  // Diese Funktion aufrufen
// zum Multiplexen
// der Ziffern

void LEDMatrix_Printhex (unsigned int value);
void LEDMatrix_Printuint (unsigned int value);
void LEDMatrix_Print_err(unsigned char errornumber);
void LEDMatrix_PrintDrehzahl (unsigned int value);

