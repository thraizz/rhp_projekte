#include "simuc.h"

///////////////////////////////////////////////////////////////////////////////////////
// Bedingte Compilierung zur Bestimmung welches "Beispiel" compiliert werden soll  ///
//
// Es darf immer nur ein "define" aktive, d.h. nicht auskommentiert, sein.
//
#define beispiel_1
//#define beispiel_2
//#define beispiel_2_mit_bugs
//#define beispiel_3
//#define beispiel_4
//#define beispiel_5
//#define beispiel_6
//#define beispiel_7
//#define beispiel_8
//#define beispiel_9
//#define beispiel_10
//#define beispiel_11
//#define beispiel_12
//#define beispiel_13
//#define led_steuerung_ueber_taster
//
///////////////////////////////////////////////////////////////////////////////////////



#ifdef beispiel_1
void emain(void* arg) 
{
	/* Deklarationsteil Anfang*/
	long int		zahl1=2;
	long int		zahl2=4;
	const long int	drei=3;
	long int		ergebnis;
		
	/* Deklarationsteil Ende */

	INIT_BM_WITHOUT_REGISTER_UI;

	/* Ausführungsteil Anfang */
	zahl2 = zahl1 + zahl2;	
	ergebnis = zahl2 * drei;
	// Ausführungsteil Ende
	
	END;
}
#endif // beispiel_1


#ifdef beispiel_2

void emain(void* arg) 
{
	long int zahl1;
	long int zahl2;
	long int ergebnis;
	char string_1[100];
	char string_2[100];

	
	INIT_BM_WITHOUT_REGISTER_UI;
	

	putstring("Bitte 1. Zahl eingeben:\n");
	getstring(string_1);
	zahl1=stringtoi(string_1);


	putstring("Bitte 2. Zahl eingeben:\n");
	getstring(string_1);
	zahl2=stringtoi(string_1);

	ergebnis= zahl1 * zahl2;

	itostring(ergebnis, string_1);

	copystring("Produkt beider Zahlen ist:\n",string_2);
	appendstring(string_1, string_2);
	putstring (string_2); 

	END;
}
#endif // beispiel_2

#ifdef beispiel_2_mit_bugs

void emain(void* arg) 
{
	long int zahl1;
	long int zahl2;
	long int ergebnis;
	char string_1[100]
	char string_2[100];

	
	INIT_BM_WITHOUT_REGISTER_UI;
	

	putstring("Bitte 1. Zahl eingeben:\n");
	getstring(string_1);
	zahl1=stringtoi(string_1);


	putstring("Bitte 2. Zahl eingeben:\n");
	getstring();
	zahl2=stringtoi(string_1);

	ergebnis= zahl1 * zahl2;

	itostring(ergebnis, string_1);

	copystring("Produkt beider Zahlen ist:\n",string_2);
	appendstring(string_1, string_2);
	putstring (string_2); 

	END;
}
#endif // beispiel_2_mit_bugs

#ifdef beispiel_3
void emain(void* arg)
{
	long int	i=4;
	char		string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	while (i>=0)
	{
		itostring(i, string);
		putstring(string);
		wait_for_return();

		i=i-1;
  	}
	END;
}
#endif // beispiel_3


#ifdef beispiel_4
void emain(void* arg)
{
	long int i=-1;
	char		string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	do
	{
		itostring(i, string);
		putstring(string);
		wait_for_return();

		i=i-1;

	} while (i >=0);

	END;
}
#endif // beispiel_4


#ifdef beispiel_5
void emain(void* arg)
{
	long int	i;
	char		string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	for (i=4; i>=0; i=i-1)
	{
		itostring(i, string);
		putstring(string);
		wait_for_return();
	}
	END;
}
#endif // beispiel_5


#ifdef beispiel_6
void emain(void* arg)
{
	long int i;
	char	 string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	putstring("Wert fuer i eingeben:");
	getstring(string);
	i=stringtoi(string);

	if (i >=0)
	{
		putstring("i ist positiv");
	}
	else
	{
		putstring("i ist negativ\n");
	}
	wait_for_return();	
	END;
}
#endif // beispiel_6


#ifdef beispiel_7
void emain(void* arg)
{
	long int i;
	char string [100];

	INIT_BM_WITHOUT_REGISTER_UI;

	for (i=4; i>=0; i=i-1)
	{
		if (i!=2) {
			itostring(i, string);
			putstring(string);
			wait_for_return();
		}
		else
			break;
	}
	END;
}
#endif // beispiel_7


#ifdef beispiel_8
void emain(void* arg)
{
	long int i;
	char string [100];

	INIT_BM_WITHOUT_REGISTER_UI;

	for (i=4; i>=0; i=i-1)
	{
		if (i!=2) {
			itostring(i, string);
			putstring(string);
			wait_for_return();
		}
		else
			continue;
	}
	END;
}
#endif // beispiel_8


#ifdef beispiel_9
void emain(void* arg)
{
	long int i=4;
	char string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	putstring("Wert fuer i eingeben:");	
	getstring(string);
	i=stringtoi(string);
	                 	

	switch( i )
		{
			case 1: putstring( "Eins"); wait_for_return(); break;
			case 2: putstring( "Zwei"); wait_for_return(); break;
			case 3: putstring( "Drei"); wait_for_return(); break;
			case 4: putstring( "Vier"); wait_for_return(); break;
			case 5: putstring( "Fuenf"); wait_for_return(); break;
			default: putstring( "Kleiner Eins oder groesser Fuenf");
		}
	END;
}
#endif // beispiel_9


#ifdef beispiel_10
void emain(void* arg)
{
	long int i=4;
	char string[100];

	INIT_BM_WITHOUT_REGISTER_UI;

	putstring("Wert fuer i eingeben:");	
	getstring(string);
	i=stringtoi(string);
		                 	

	switch( i )
 	{
		case 1: putstring( "Eins"); wait_for_return();
 		case 2: putstring( "Zwei"); wait_for_return();
 		case 3: putstring( "Drei"); wait_for_return();
 		case 4: putstring( "Vier"); wait_for_return();
 		case 5: putstring( "Fuenf"); wait_for_return();
 		default: putstring( "Kleiner Eins oder groesser Fuenf");
 	}
	END;
}
#endif // beispiel_10


#ifdef beispiel_11
void emain(void* arg)
{
	char string_1[100];
	char string_2[100];
	long int messwerte [10];  			// Meldet ein Feld von
										// 10 Integerwerten an.

	long int tabelle[3] = {1, 2, 3}; 	// Meldet ein Feld von 
	 				         			// 3 Integerwerten an und weist dem
				             			// 1. Feldelement den Wert 1, dem
				             			// 2. Feldelement den Wert 2 und dem
			             	 			// 3. Feldelement den Wert 3 zu.
	INIT_BM_WITHOUT_REGISTER_UI;


	messwerte [0] = tabelle[2]; 		// Weist dem 1. Feldelement von messwerte 
					             		// den Wert des 3. Feldelementes von tabelle zu.

	messwerte [1] = messwerte [0]; 		// Liest den Wert des 1.
				               			// Feldelementes und weist diesen Wert
				               			// dem 2. Feldelement zu.


	copystring("messwerte[0]:",string_2);
	itostring(messwerte[0], string_1);
	appendstring(string_1, string_2);
	putstring(string_2);
	wait_for_return();

	copystring("messwerte[1]:",string_2);
	itostring(messwerte[1], string_1);
	appendstring(string_1, string_2);
	putstring(string_2);
	wait_for_return();

	END;
}
#endif // beispiel_11




#ifdef beispiel_12

#define AUSGABE
#define MAXANZAHL 30

void emain(void* arg)
{
	int feld[MAXANZAHL];
	char string_1[100];
	char string_2[100];
	int i;

	INIT_BM_WITHOUT_REGISTER_UI;

	for (i=0; i<MAXANZAHL; i++)
	{
		putstring("Bitte eine Zahl eingeben:\n");
		getstring(string_1);
		feld[i]=stringtoi(string_1);

#ifdef AUSGABE
		itostring(feld[i], string_1);
		copystring("Eingegeben wurde die Zahl ",string_2);
		appendstring(string_1, string_2);
		putstring (string_2); 
		wait_for_return();
#endif
	}
	END;
}

#endif // beispiel_12


#ifdef beispiel_13

void emain(void* arg)
{
	typedef enum {MO, DI, MI, DO, FR, SA, SO} wochentag;
	wochentag heute=MO;

	INIT_BM_WITHOUT_REGISTER_UI;

	// ...
	// Hier wird irgendwie der aktuelle Wochentag ermittelt
	// und der entsprechende Enumerations-Wert der Variablen
	// heute zugewiesen.
	// ...
	
	switch( heute )
		{
			case MO:	
				putstring( "Gehe zur FH!"); 
				wait_for_return(); break;
			case DI: putstring( "Gehe zur FH!"); wait_for_return(); break;
			case MI: putstring( "Gehe zur FH!"); wait_for_return(); break;
			case DO: putstring( "Gehe zur FH!"); wait_for_return(); break;
			case FR: putstring( "Zuhause lernen"); wait_for_return(); break;
			case SA: putstring( "Erholen"); wait_for_return(); break;
			case SO: putstring( "Erholen"); wait_for_return(); break;
			default: putstring( "Den Wochtag gib es nicht!");
		}
	END;
}

#endif // beispiel_13



#ifdef led_steuerung_ueber_taster

void emain(void* arg) 
{
	unsigned short int T1, T2;
	unsigned short int temp, help;

	INIT_BM_WITH_REGISTER_UI;
	//INIT_REGISTER_UI_WITHOUT_BM


	// ->  Dies folgenden Zeilen sind nicht Teil der Loesung bewirken aber, dass das Bandmodell bei Tastendruck laeuft
	io_out(DIR1, 1<<8);		// Nur zur Ueberpruefung und damit der Motor des Bandmodells eingeschaltet ist
							// ACHTUNG: Ein Schreiben (io_out) auf ein Portbit wird ignoriert, wenn dieses als Eingang konfiguriert ist. 
							//			Daher MUSS diese Zeile vor der folgenden Zeile stehen.

	io_out(OUT1, (io_in(OUT1)) | (1<<8));	// Damit der Motor vom Bandmodell auf Tastendruck rechts oder links herum laeuft.
											// wird das M_an-Bit (bit 8) hier gesetzt.
	// <-#######


	// Initialisierung gemaess Unterpunkt b)
	temp = (1 << 5) | (1 << 4); // temp mit dem Bitmuster 0x0030 
								// initialisieren bei dem nur Bit 5 							
								// und Bit 4 gesetzt (1) sind

	temp = ~temp;				// Alle Bits von temp negieren 								
								// temp enthält nun das
								// Bitmuster 0xFFCF

	io_out (DIR1, ( io_in(DIR1) & temp) );	// Nur Bit 5 und 4 den
											// Wert 0 ( Lesen ) geben und
											// alle anderen Bits unverändert
											// lassen.

	temp = (1 << 10) | (1 << 9);	
	io_out (DIR1, ( io_in(DIR1) |  temp) ); // Nur Bit 10 und 9 
											// auf 1 (Schreiben) setzen. Alle
											// anderen Bits unverändert
											// lassen.

	while(1) {

		SYNC_SIM;

		temp = io_in(IN1);		// Einlesen von Port 1	

		temp = temp >> 4;	// So oft nach rechts shiften
							// bis das Bit 4 an der Bitstelle 0 steht.

		T1 = temp & 0x01;	// Zur Sicherheit alle Bits, 
							// bis auf Bit0 auf 0 setzen 
							// und das Ergebnis T1 zuweisen.
		/*	Das Bit für den Taster T1 ist nun eingelesen und 
		die Variablen T1 hat genau den logischen Wert dieses
		Bits, d.h.
		T1 = 0 wenn Bit 4 = 0 , also wenn Taster T1 betätigt.
		T1 = 1 wenn Bit 4 = 1 , also wenn Taster T1 nicht betätigt.
		Für T2 machen wir nun das Gleiche.
		*/
		temp = in(IN1);		// Einlesen von Port 1	

		temp = temp >> 5;	// So oft nach rechts shiften
							// bis das Bit 5 an der Bitstelle 0 steht.

		T2 = temp & 0x01;	// Alle Bits, bis auf Bit0 auf 0 setzen 
							// und das Ergebnis T2 zuweisen.

		if(T1 == 1) {		
			temp = 1 << 9;	// Berechnung des Bitmusters 0x0200 						
							// bei dem nur das Bit 9 gesetzt ist.

			// Bit 9 an Port 1 auf '1' setzen.
			help = io_in(OUT1); 			// Aktuellen Ausgabewert einlesen ...
			help = help |  temp;			// ... mit temp "verodern"...
			io_out(OUT1, ( help | temp) );	 // ... und wieder ausgeben.
			
			// Dies waere die kuerzere Varianten um Bit 9 zu setzen
			// io_out (OUT1, ( io_in(OUT1) |  temp) );
		}
		else {
			temp = 1 << 9;	// Berechnung des Bitmusters 0x0200 

			temp = ~temp;	// Das Bitmuster wird bitweise
							// invertiert, d.h. nun ist Bit 9 das
							// einzige Bit das '0' ist.

			// Bit 9 an Port 1 wird zurueck gesetzt auf '0
			io_out (OUT1, ( io_in(OUT1) & temp) );
			
		}

		if(T2 == 1) {		
			temp = 1 << 10;
			io_out (OUT1, ( io_in(OUT1) |  temp) );
		}		
		else {
			temp = 1 << 10;
			temp = ~temp;
			io_out (OUT1, ( io_in(OUT1) & temp) );
		}
	}		

	
	END;
}
#endif // led_steuerung_ueber_taster
