﻿#include "simuc.h"
#include "user_conf.h"

///////////////////////////////////////////////////////////////////////////////////////
// Bedingte Compilierung 
// Es darf immer nur ein "define" aktive, d.h. nicht auskommentiert, sein.
//
#define V3_Aufgabe_1
//#define V3_Aufgabe_2_und_3
//#define nachrichtenempfang_ueber_ports
//#define timer_als_taktsignalgenerator
//
///////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned char hh;
    unsigned char mm;
    unsigned char ss;
} uhrzeit;

uhrzeit akt_zeit;
uhrzeit hoch_zeit;
uhrzeit runter_zeit;

#ifdef V3_Aufgabe_1

// Einige Defines zum leichteren Wiederfinden
#define BIT_POS_IST_OBEN	0
#define BIT_POS_IST_UNTEN	1
#define BIT_POS_NACH_OBEN	4
#define BIT_POS_NACH_UNTEN	5
#define BIT_POS_FAHRE_NACH_OBEN	9
#define BIT_POS_MOTOR_AN	8
#define BIT_POS_FAHRE_NACH_UNTEN	10

typedef enum {hoch, runter, steht} STATE;	// Datentyp für den Zustand das Automaten.
											// Vergleichbar mit "TYPE .... IS" in VHDL.
typedef unsigned short int USHORT;			// Eigener Datentyp zur Abkuerzung

#define ZT_MAXW  1  //20
#define SS_MAXW 60  //60
#define MM_MAXW  2  //60
#define HH_MAXW  1  //24

void timer1_init()
{
    /*
     * Zur Berechnung der Werte fuer den Prescaler und den Compare-Match:
     * Bei einer Frequenz von 4 Mhz zaehlt der Timer mit einem Takt von 0,25us.
     * Er kann mit seinen 65536 moeglichen Zaehlstaenden eine maximale Interrupt-Periode
     * von 65536 * 0,25us = 16384us realisieren.
     * Dies ist zu schnell. => Der Zaheler muss mittels des Prescalers langsamer eingestellt werden.
     * Die ideale Untersetzung waere 50000us / 16384us = 3,0517.
     * Da es diese Unterssetzung (Prescaler-Wert) nicht gibt waehlen wir den naechst groesseren Wert also 8.
     * Der Zaehler zaehlt jetzt mit einem Takt vom 2us. => Die Interrupts haben bei
     * einem Compare-Match-Wert von 65535 eine Periode von 131072 us.Der Compare-Match-Wert
     * muss auf 50000us/131072us*65536us = 25000 eingestellt werden.
    **/
    unsigned short buf = 0;
    // TCRA1
    // Clock Source auf intern/8 Prescaler
    // Timer Modus Clear-Timer-On-Compare-Match
    buf = (1 << PS11) | (1 << TM10);
    io_out16(TCRA1, buf);
    // TCRB1
    // Counter Enable
    buf = (1 << CE1);
    io_out16(TCRB1, buf);
    // TIMR1
    // Compare Match Interrupt enable
    buf = (1 << OCIE1);
    io_out16(TIMR1, buf);
    // CMPA1
    // Compare Match Register auf ...
    buf = 25000; //25000;
    io_out16(CMPA1, buf);
}

//Interrupt Service Routine
void timer1_oco1_isr()
{
    static int zt = 0;
    //unsigned char stringbuf[100];
    unsigned short int buf;
    zt++;
    if(zt == ZT_MAXW){
        zt = 0;
        akt_zeit.ss++;
    }
    if(akt_zeit.ss == SS_MAXW){
        akt_zeit.ss = 0;
        akt_zeit.mm++;
    }
    if(akt_zeit.mm == MM_MAXW){
        akt_zeit.mm = 0;
        akt_zeit.hh++;
    }
    if(akt_zeit.hh == HH_MAXW){
        akt_zeit.hh = 0;
    }
    /*sprintf(stringbuf, "zt=%d Uhrzeit: %d:%d:%d\n", zt, akt_zeit.hh, akt_zeit.mm, akt_zeit.ss);
    putstring(stringbuf); // Bei der Simulation des SimuC fuehrt der Aufruf von putstring() innerhalb einer ISR
    // ggf. zu einem Dead-Lock . Dies liegt aber nur am Simulator.
    // Zuruecksetzen des Interrupt-Flags*/
    buf = io_in16(TIFR1);
    buf = buf & ~(1 << OCIF1);
    io_out16(TIFR1, buf);
}

void steuerungsfunktion(USHORT ist_oben,
						USHORT ist_unten,
						USHORT nach_oben,
						USHORT nach_unten,
						USHORT nach_oben_wegen_zeit,
						USHORT nach_unten_wegen_zeit,
						USHORT* p_fahre_nach_oben,
						USHORT* p_fahre_nach_unten,
						STATE* p_state){
	//! Das Rollo soll hoch bzw runter wenn Befehl oder Zeitbedingung vorliegt
	nach_oben = nach_oben | nach_oben_wegen_zeit;
	nach_unten = nach_unten | nach_unten_wegen_zeit;

	// 5.)	switch-case-Konstrukt zur "Verwaltung" der Zustaende
	switch (*p_state) {

		// 6.)	Ein CASE je Zustand
		case steht:

			// 7a.)  .... Ausgabesignale bestimmen 
			*p_fahre_nach_unten=0; *p_fahre_nach_oben=0; 

			// 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
			//         Ein IF je Pfeil
			if (  (ist_unten == 0) && (nach_unten == 1) && (nach_oben == 0)) {
				*p_state = runter; // Wechsel in den Zustand "runter"
			}
			if (  (ist_oben == 0) && (nach_oben == 1) ){
				*p_state = hoch;  // Wechsel in den Zustand "hoch"
			}
			
			// Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert. 
			if ( !(    ((ist_unten == 0) && (nach_unten == 1) && (nach_oben == 0)) 
				    || ((ist_oben == 0) && (nach_oben == 1)) )      ) {
				*p_state = steht; // Bleibe im Zustand "steht"
			} 
			break;

		case runter:

			// 7a.)  Ausgabesignale bestimmen 
			*p_fahre_nach_unten=1; *p_fahre_nach_oben=0; 


			// 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
			if (ist_unten == 1) {
				*p_state = steht; // Wechsel in den Zustand "steht"
			}

			// Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert. 
			if (ist_unten == 0) {
				*p_state = runter; // Bleibe im Zustand "runter"
			}

			break;

		case hoch:

			// 7a.)  Ausgabesignale bestimmen 
			*p_fahre_nach_unten=0; *p_fahre_nach_oben=1; 

			// 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
			if (ist_oben == 1){
				*p_state = steht; // Wechsel in den Zustand "steht"
			}

			// Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert. 
			if (ist_oben == 0){
				*p_state = hoch; // Bleibe im Zustand "hoch"
			}
			break;

		// 9.) nicht erlaubte Zustaende "abfangen"
		default:
			//! standard ist stillstand, also weder fahrt nach unten noch nach oben
			*p_fahre_nach_unten=0; *p_fahre_nach_oben=0;
			*p_state = runter;
			break;

	} // end switch	
} // end steuerungsfunktion()


int is_time_equal(uhrzeit* zeit1, uhrzeit* zeit2){
	if (zeit1 == zeit2){
		return 1;
	}
	if(zeit1->hh == zeit2->hh && zeit1->mm == zeit2->mm && zeit1->ss == zeit2->ss){
		return 1;
	}
	return 0;
}


void emain(void* arg) {
	STATE cstate;		// Variable für den Zustand das Automaten.

	// Variablen für die Eingabesignale. Eine Variable fuer jedes Signal.
	USHORT		nach_oben;  
	USHORT		nach_unten;
	USHORT		ist_oben;
	USHORT		ist_unten;

	// Variablen für die Ausgabesignale. Eine Variable fuer jedes Signal.
	USHORT		fahre_nach_oben;
	USHORT		fahre_nach_unten;
	
	USHORT		input, output, last_output;


	INIT_BM_WITH_REGISTER_UI;	// Nur fuer Simulation


	// 1.)	Hardware konfigurieren
	io_out16(DIR1, 0xFF00); // Ausgang: Bits 15 bis 8   Eingang: Bits 7 bis 0 
	
	// Definiere zeiten
	akt_zeit.hh = 0;
    akt_zeit.mm = 0;
    akt_zeit.ss = 0;
    hoch_zeit.hh =  0;
    hoch_zeit.mm =  1;
    hoch_zeit.ss = 55;
    runter_zeit.hh =  0;
    runter_zeit.mm =  1;
    runter_zeit.ss =  0;
    unsigned short buf = 0;
	// PIC Interrupt deaktivieren
    buf = io_in16(PICC);
    buf = buf & ~(1 << PICE);
    io_out16(PICC, buf);

    // Timer 1 initialisieren
    timer1_init();

    // ISR registrieren
    setInterruptHandler(IVN_OC1, timer1_oco1_isr);

	// PCI Interrupt aktivieren
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);	
	//##########################################################################
	// 2.)	 Definition des Startzustandes. Entspricht dem asynchronen Reset in VHDL.
	cstate = runter;

	// 3.) Unendliche Schleife. Ein Schleifendurchlauf entspricht einem Zyklus des Automaten
	while (1) { 
	
		SYNC_SIM; // Nur fuer Simulation

		// 4.)	Einlesen der Eingabesignale einmal je Zyklus
		input = io_in16(IN1);

		// extrahieren von "ist_oben" (BIT_POS_IST_OBEN)
		ist_oben = (input >> BIT_POS_IST_OBEN) & 0x01;

		// extrahieren von "ist_unten" (BIT_POS_IST_UNTEN)
		ist_unten = (input >> BIT_POS_IST_UNTEN) & 0x01;

		// extrahieren von "nach_oben" (BIT_POS_NACH_OBEN)
		nach_oben = (input >> BIT_POS_NACH_OBEN) & 0x01;

		// extrahieren von "nach_unten" (BIT_POS_NACH_UNTEN)
		nach_unten = (input >> BIT_POS_NACH_UNTEN) & 0x01;
		
		// Aufruf der Steuerungsfunktion
		//##########################################################################
        steuerungsfunktion(ist_oben, ist_unten, nach_oben, nach_unten,
                           is_time_equal(&akt_zeit, &hoch_zeit), is_time_equal(&akt_zeit, &runter_zeit),
                           &fahre_nach_oben, &fahre_nach_unten, &cstate);
		//##########################################################################


		// 7b.) Ausgabesignale ausgeben
		output=fahre_nach_unten<<BIT_POS_FAHRE_NACH_UNTEN;  
		output=output | (fahre_nach_oben<< BIT_POS_FAHRE_NACH_OBEN); 


		// Nur wirklich ausgeben wenn notwendig
		// Optimierung mittels bedigter Portausgabe
		if (last_output != output) {
			output=output | (1<< BIT_POS_MOTOR_AN);   // Nur fuer Bandmodell
			io_out16(OUT1, io_in16(OUT1) & 0x00FF);
			io_out16(OUT1, io_in16(OUT1) |  output);
			last_output = output;
		}
	} // end while
} // end main
#endif // V3_Aufgabe_1



#ifdef V3_Aufgabe_2_und_3
#include "user_conf.h"

// Vorgegebene globale Variablen und Makros
// Diese sind zum Teil auch in den Loesungen zu den relevanten Uebungsaufgaben schon vorhanden
#define MAX_MESSAGE_SIZE 100

// Hier muessen noch weitere globale Variablen und Makros eingefuegt werden
// ...
#define STARTBYTE	35
#define ENDBYTE		0

typedef enum {b_akt_zeit='A', b_hoch_zeit, b_runter_zeit}  Befehlskennung;
typedef enum {warte_auf_start_byte, warte_auf_end_byte}    STATE_N;

unsigned char        nachricht[MAX_MESSAGE_SIZE], flag_ready=1;
unsigned long int    byte_counter;
uhrzeit              akt_zeit, hoch_zeit, runter_zeit;
STATE_N              comstate = warte_auf_start_byte;

void do_param(unsigned char* msg, uhrzeit* akt, uhrzeit* hoch, uhrzeit* runter ) {
    uhrzeit* ziel;
	// Abhängig von der 2. Stelle (A, B oder C) wird die aktuelle/hoch/runter Zeit verändert
	// Dies geschieht über setzen eines Pointers auf die richtige Referenz
    switch (msg[1]) {
		case b_akt_zeit:    ziel = akt;      break;
		case b_hoch_zeit:   ziel = hoch;     break;
		case b_runter_zeit: ziel = runter;   break;
    }

    // Zehnerstelle muss mit 10 multipliziert werden
	// und immer abzüglich des Endbits
	// 3. und 4. Stelle ist die Stunde
    ziel->hh = ((((msg[2])-'0') * 10) + ((msg[3])-'0'));
	// 5. und 6. Stelle ist die Minute
    ziel->mm = (((msg[4]) -'0') * 10) + ((msg[5])-'0');
	// 7. und 8. Stelle ist die Sekunde
    ziel->ss = (((msg[6]) -'0') * 10) + ((msg[7])-'0');
}


void steuerungsfunktion(unsigned char        byte_received,
                        unsigned long int*   byte_zaehler,
                        unsigned char*       empfangene_nachricht,
                        unsigned char*       ready,
                        STATE_N*             state){
    switch (*state) {
		case warte_auf_start_byte:
			if (byte_received == STARTBYTE) {
				*byte_zaehler = 0;
				empfangene_nachricht[*byte_zaehler] = byte_received;
				(*byte_zaehler)++;
				*state = warte_auf_end_byte;
			}
			break;

		case warte_auf_end_byte:
			// Erneutes Startbyte bricht Übertragung ab und triggert erneutes Lesen
			if (byte_received == STARTBYTE) {
				*byte_zaehler = 0;
				empfangene_nachricht[*byte_zaehler]=byte_received;
				(*byte_zaehler)++;
				break;
			}

			// Nachricht vollständig empfangen, warte auf neue Übertragung
			else if (byte_received == ENDBYTE) {
				// Etwas tun am Uebergang.
				empfangene_nachricht[*byte_zaehler] = byte_received;
				(*byte_zaehler)++;
				*ready = 1;
				*state = warte_auf_start_byte;
				break;
			}

			// Breche wg. zu langer Nachricht ab und warte auf neue Übertragung
			else if (*byte_zaehler == MAX_MESSAGE_SIZE-1) {
				*state = warte_auf_start_byte;
				break;
			}

			// Wenn hier angelangt: Kein besonderer byte; speichern und Zähler erhöhen, weiter warten
			empfangene_nachricht[*byte_zaehler]=byte_received;
			(*byte_zaehler)++;
			break;

		default: // Ungültiger Zustandswechsel, aktuelles Lesen abbrechen und auf erneutes Startbyte warten
			*state = warte_auf_start_byte;
	} // end switch
}

#pragma region slave
void init_spi2(){
	//! Enable SPI2 and SPI2 Interrupts
	io_out8(SPCR2, ((1<<SPIE2)|(1<<SPE2)));
}

void spi_isr() {
    unsigned char byte_received;

    // Warte auf Uebtragungsende.
    while (!(io_in8(SPSR2) & (1<<SPIF2)));

    // Daten lesen
    byte_received = io_in8(SPDR2);

    // Aufruf der Steuerungsfunktion
    steuerungsfunktion(byte_received, &byte_counter, &(nachricht[0]), &flag_ready, &comstate);
	// Gemäß A3b2, ruft die isr nach dem empfang do_param auf
    if (flag_ready==1) do_param(&(nachricht[0]), &akt_zeit, &hoch_zeit, &runter_zeit );
}

void emain(void* arg) {
	char string[300];
    unsigned short buf = 0;
	 
	INIT_BM_WITH_REGISTER_UI;

	// Hier die notwendigen Initialisierungen einfuegen.
	// ...
    akt_zeit.hh =  0;
    akt_zeit.mm =  0;
    akt_zeit.ss =  0;
    hoch_zeit.hh =  0;
    hoch_zeit.mm =  0;
    hoch_zeit.ss =  0;
    runter_zeit.hh =  0;
    runter_zeit.mm =  0;
    runter_zeit.ss =  0;

	//! Gleiche Initialisierung wie bei service interrupt routine,
	//! statt timer wird aber spi2 initialisiert und der entsprechende Interrupthandler gewählt
	// Zur Sicherheit vor Initialisierung den Interupt des PIC generell deaktivieren
    buf = io_in16(PICC);
    buf = buf &  ~(1 << PICE);
    io_out16(PICC, buf);

	// SPI2 initalisieren
    init_spi2();

	// Spi ISR registrieren
    setInterruptHandler(IVN_SPI2, spi_isr);

	// Interrupt des PIC jetzt zulassen
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);


	while(1) {
        #ifndef USER_PROG_2
		putstring("Sie haben USER_PROG_2 nicht definiert\n");
        #endif
		if(flag_ready==1){
			putstring((char*)nachricht);
			putstring("\n");

			sprintf(string, "Akt:%d:%d:%d  Hoch:%d:%d:%d  Runter:%d:%d:%d\n", akt_zeit.hh,akt_zeit.mm,akt_zeit.ss, hoch_zeit.hh,hoch_zeit.mm,hoch_zeit.ss, runter_zeit.hh,runter_zeit.mm,runter_zeit.ss);
			putstring(string);
			flag_ready = 0;
		}	
	}
		
}
#pragma endregion

#pragma region master
void init_spi1() {
	// Enable SPI Interrupt, SPI, MASTER SELECT
    io_out8(SPCR1, ((1<<SPIE1)|(1<<SPE1)|(1<<MSTR1)));
}


void spi_send_data(unsigned char data) {
	// Sende daten
    io_out(SPDR1, data);
	// Warte auf ende der übertragung, also bis spi fertig gesetzt
	while(!(io_in8(SPSR1)&(1<<SPIF1)));
}

void emain_sender(void* arg) {
    unsigned char i;
    unsigned char* param;
	unsigned char parametriere_akt_zeit[]		=	"#A000005";
	unsigned char parametriere_hoch_zeit[]		=	"#B000105";
	unsigned char parametriere_runter_zeit[]	=	"#C000159";
    param = parametriere_runter_zeit;

	init_spi1();

	while(1) { 
		i=0;
		do  {
            spi_send_data(param[i]);
			ms_wait(10);
			i++;
        } while(i<9);
	}
}
#pragma endregion

#endif //V3_Aufgabe_2_und_3


#ifdef nachrichtenempfang_ueber_ports
// Sinnvoll zu nutzende Makros
#define COM_SIGNAl_PIN				0		// Pin ueber den der Interrupts ausgeloest wird
#define COM_DATA_IN_REGISTER		IN0		// Register ueber den das Byte eingelesen wird
#define MAX_MESSAGE_SIZE			100		// Maximale Laenge einer Nachricht
#define STARTBYTE					0xFE	// Wert des Start-Bytes
#define ENDBYTE						0xEF	// Wert des Ende-Bytes


typedef enum {warte_auf_start_byte, warte_auf_end_byte} STATE_N;


// Globale Variablen fuer die ISR
unsigned char		nachricht[MAX_MESSAGE_SIZE];
unsigned char		flag_ready;
STATE_N				comstate=warte_auf_start_byte;
unsigned long int	byte_counter;



void init_gpio_0_1()
{
	unsigned short hilfe = 0;
	
	// ### PORT 1
	// Interrupt fuer Bit 0 von PORT1 enable
	hilfe = io_in16(EIE1) | (1 << COM_SIGNAl_PIN);
	io_out16(EIE1, hilfe);
	
	// Das Bit 0 von PORT 1 ist Eingang
	hilfe = io_in16(DIR1) & ~(1 << COM_SIGNAl_PIN);
	io_out16(DIR1, hilfe);


	// ### PORT 0
	// Die unter 8 Bit von PORT0 sind Eingang
	hilfe = io_in16(DIR0) & 0xFF00;

	io_out16(DIR1, hilfe);		
}


void steuerungsfunktion(unsigned char 		byte_received,
						unsigned long int* 	byte_zaehler,
						unsigned char* 		empfangene_nachricht,
						unsigned char*		ready, STATE_N* state)
{
	
	switch (*state) {

	case warte_auf_start_byte:
		// Uebergang nach warte_auf_end_byte
		if ( byte_received == STARTBYTE) {
			// Etwas tun am Uebergang.
			*byte_zaehler=0;
			empfangene_nachricht[*byte_zaehler]=byte_received;
			*byte_zaehler=*byte_zaehler+1;

			// Zustandswechsel
			*state = warte_auf_end_byte; 
		}
		break;

	case warte_auf_end_byte:	
		// Uebergang nach warte_auf_start_byte
		if (byte_received == ENDBYTE)  {
			// Etwas tun am Uebergang.
			empfangene_nachricht[*byte_zaehler]=byte_received;
			*byte_zaehler=*byte_zaehler+1;

			*ready=1;

			// Zustandwechsel
			*state = warte_auf_start_byte; 
		}

		if (*byte_zaehler == MAX_MESSAGE_SIZE-1) {
			// Die Nachricht ist zu lang und kann dahr nicht gueltig sein!
			// Zustandwechsel
			*state = warte_auf_start_byte; 
		}

		// Uebergang auf sich selbst nur damit etwas getan wird.
		if (byte_received == STARTBYTE) {	
			// Etwas tun am Uebergang.
			*byte_zaehler=0;
			empfangene_nachricht[*byte_zaehler]=byte_received;
			*byte_zaehler=*byte_zaehler+1;

			// Zustandwechsel
			*state = warte_auf_end_byte;	// Ist ueberfluessing dient aber hoffentlich 
									// dem Verstaendnis
		}

		// Uebergang auf sich selbst nur damit etwas getan wird.
		if ((byte_received != STARTBYTE) && (byte_received != ENDBYTE) 
			&& (*byte_zaehler < MAX_MESSAGE_SIZE-1)) {		
			// Etwas tun am Uebergang.
			empfangene_nachricht[*byte_zaehler]=byte_received;
			*byte_zaehler=*byte_zaehler+1;

			// Zustandwechsel
			*state = warte_auf_end_byte;	// Ist ueberfluessing dient aber hoffentlich 
									// dem Verstaendnis
		}

		break;

		// 9.) nicht erlaubte Zustaende "abfangen"
		default:
			*state = warte_auf_start_byte;
	} // switch

}



void ISR_EXT_INT0()
{
	unsigned short int hilfe = 0;
	unsigned char buf;

	// Einlesen des Datenbytes
	buf=(unsigned char) (io_in16(COM_DATA_IN_REGISTER) & 0x00FF);

	// Aufruf der Steuerungsfunktion
	steuerungsfunktion(buf, &byte_counter, &(nachricht[0]), &flag_ready, &comstate);

	// Zureucksetzen des Interrupt-Flags
	hilfe = io_in16(EIF1) & ~(1<<COM_SIGNAl_PIN);
	io_out16(EIF1, hilfe);
	return;
}


void emain(void* arg)
{
	 unsigned short int buf;

	 INIT_REGISTER_UI_WITHOUT_BM	

	 // Zur Sicherheit vor Initialisierung den Interrupt des PIC generell deaktivieren 
	 buf = io_in16(PICC); 
	 buf = buf &  ~(1 << PICE); 
	 io_out16(PICC, buf); 

	 // Initialisieren der Ports
	 init_gpio_0_1();

	 // Registrieren der ISRs in der Interupt-Vektor-Tabelle
	 setInterruptHandler(IVN_EI100, ISR_EXT_INT0);
	
	 // Interrupt des PIV jetzt zulassen.
	 buf = buf | (1 << PICE); 
	 io_out16(PICC, buf); 
	

	 while(1)
	 {
		
	 }
	
}
#endif // nachrichtenempfang_ueber_ports


#ifdef timer_als_taktsignalgenerator

// Globale Variablen zur Kommunikation mit der ISR
unsigned long int zt=0, hh=0, mm=0, ss=0;

void timer1_init()
{
	/*	Zur Berechnung der Werte fuer den Prescaler und den Compare-Match:
		Bei einer Frequenz von 4 Mhz zaehlt der Timer mit einem Takt von 0,25us.
		Er kann mit seinen 65536 moeglichen Zaehlstaenden eine maximale Interrupt-Periode von 65536 * 0,25us = 16384us realisieren.
		Dies ist zu schnell. => Der Zaheler muss mittels des Prescalers langsamer eingestellt werden.
		Die ideale Untersetzung waere  50000us / 16384us = 3,0517.
		Da es diese Unterssetzung (Prescaler-Wert) nicht gibt waehlen wir den naechst groesseren Wert also 8.
		Der Zaehler zaehlt jetzt mit einem Takt vom 2us. => Die Interrupts haben bei einem Compare-Match-Wert von 65535 
		eine Periode von 131072 us.Der Compare-Match-Wert muss auf 50000us/131072us*65536us = 25000 eingestellt werden.
	*/

	unsigned short buf = 0;
	
	// TCRA1
	// Clock Source auf intern/8 Prescaler
	// Timer Modus Clear-Timer-On-Compare-Match
	buf = (1 << PS11) | (1 << TM10);
	io_out16(TCRA1, buf);

	// TCRB1
	// Counter Enable
	buf = (1 <<CE1);
	io_out16(TCRB1, buf);

	// TIMR1
	// Compare Match Interrupt enable
	buf = (1 << OCIE1);
	io_out16(TIMR1, buf);

	// CMPA1
	// Compare Match Register auf ...
	buf = 5000;
	io_out16(CMPA1, buf);
} 

//! Vorgegebene Makros haben nicht gut funktioniert daher eigene Werte
#define ZT_MAXW	3	//20
#define SS_MAXW	3	//60
#define MM_MAXW	3	//60
#define HH_MAXW	3	//24

//	Interrupt Service Routine
void timer1_oco1_isr()
{
	unsigned char stringbuf[100];
	unsigned short int buf;

	zt++;

	if(zt==ZT_MAXW){
		zt=0;
		ss++;
	}

	if(ss==SS_MAXW){
		ss=0;
		mm++;
	}

	if(mm==MM_MAXW){
		mm=0;
		hh++;
	}
	if(hh==HH_MAXW){
		hh=0;
	}

	sprintf(stringbuf,"zt=%d  Uhrzeit: %d:%d:%d\n",zt,hh,mm,ss);
	// Achtung die Verwendung von putstring() kann in der Simulationsumgebung
	// zu einem Deadlock fuehren, d.h. alles bleibt einfach stehen.
	// Dahr ist dies hier (zur Sicherheit) auskommentiert.
	// putstring(stringbuf);

	// Zuruecksetzen des Interrupt-Flags
	buf = io_in16(TIFR1);
	buf = buf & ~(1 << OCIF1);
	io_out16(TIFR1, buf);

}


void emain(void* arg)
{
	unsigned short buf = 0;
	unsigned char stringbuf[100];

	INIT_BM_WITH_REGISTER_UI; // Nur zur Simulation

	// Zur Sicherheit vor Initialisierung den Interupt des PIC generell deaktivieren
	buf = io_in16(PICC);
	buf = buf &  ~(1 << PICE);
	io_out16(PICC, buf);

	// Timer 1 initialisieren
	timer1_init();

	// ISR registrieren
	setInterruptHandler(IVN_OC1, timer1_oco1_isr);

	// Uhrzeiterfragen ohne weitere Ueberpruefung
	putstring("Bitte die aktuelle Uhrzeit im Format hh:mm:ss eingeben\n");
	getstring(stringbuf);
	sscanf(stringbuf,"%d:%d:%d",&hh, &mm, &ss);

	// Interrupt des PIC jetzt zulassen
	buf = buf | (1 << PICE);
	io_out16(PICC, buf);

	while(1);	
}
#endif // timer_als_taktsignalgenerator
