/*****************************************/
/*    AVR Board1 Application Master      */
/*	   Aly, Al-Dhubri ,                  */
/*	  Datum 09.07.2026                    */

/*	                                     */
/*									     */
/*    Schrittmotor.h					 */
/*                                       */
/*****************************************/
//-----------------------------------------
// PD3 activitiy
#define LED_aktiv_PIN 0x08
#define LED_aktiv_DDR DDRD
#define LED_aktiv_PORT PORTD
#ifndef __SCHRITTMOTOR_H__
#define __SCHRITTMOTOR_H__
//-----------------------------------------
enum RUN { Stop,
           Run, Off, notaus};
enum Richtung{ links,
	           rechts};
unsigned char lese_Endschalter (void);
//-----------------------------------------
class clsSchrittmotor
{private:  unsigned char direction;
 private:  unsigned char run;
 private:  unsigned char step;
 public:   unsigned int reload;
 public:
	clsSchrittmotor(unsigned char run,
	                unsigned char dir,
	                unsigned int reload);
	~clsSchrittmotor();
	void set_speed (unsigned int);
	void set_direction (unsigned char value);
	unsigned char get_direction();// Die aktuelle Richtung, Die im Attribut direction gespeichert ist,wird zurückgegeben.
	unsigned char get_run();//Die aktuelle betriebsmodus, der im Attribut run gespeichert ist, wird zurückgegeben 
	void start (void);
	void stop (void);
	void off (void);//Attribut run auf den Wert Off gesetzt 
	void NOT_AUS (void);
	void do_step(void);
}; //clsSchrittmotor
//-----------------------------------------
#endif //__SCHRITTMOTOR_H__
//-----------------------------------------