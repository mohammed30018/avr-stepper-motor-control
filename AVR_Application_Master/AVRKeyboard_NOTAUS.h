/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*    AVRKEXBOARD.H                      */
/*                                       */
/*****************************************/

//------------------------------------------------------------------------------------
class clsKeyboard
{
private: unsigned char 	oldscancode; 
private: unsigned char 	pressed;
private: unsigned char  keys[4];        		 // Puffer für Tasten in ASCII
public:  clsKeyboard (void);                     // Konstruktor
public: unsigned char Keyboard_gedrueckt (void);
};
//------------------------------------------------------------------------------------
