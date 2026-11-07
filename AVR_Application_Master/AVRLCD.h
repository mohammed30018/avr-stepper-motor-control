/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*									     */
/*    AVRLCD.H                           */
/*    4 Bit Datbus                       */
/*                                       */
/*****************************************/


class clsLCD
{
public: void init (void);                                // initialisiert LCD
public: void PrintString (const char *Zeichenkette);     // Gibt eine Textkonstante aus, Ende mit 0:
public: void Cursor (unsigned char ON, unsigned char BLINK);
public: void Clearscreen (void);
public: void gotoxy (unsigned char x , unsigned char y); // x Position, y Position, (0,0) links oben
public: void write_data (unsigned char value);           // Byte in das Datenregister schreiben
public: void PrintZahl8 (unsigned char zahl) ;           // 1 Byte -> dezimal 3 Stellen, führende 0 unterdrückt
private: void set_CG_RAM (void);
};
