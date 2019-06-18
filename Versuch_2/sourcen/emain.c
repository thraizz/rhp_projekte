#include "simuc.h"
#include "io_treiber.h"

// Die Verwendung der folgenden Zustandsnamen ist verbindlich
typedef enum{Steht,StarteRunter,StarteRauf,Notaus,FahreRauf_MIT_OF,FahreRauf_OHNE_OF,FahreRunter_MIT_OF,FahreRunter_OHNE_OF} state_t;

// Die folgenden Defines muessen sinnvoll genutzt werden
#define BIT_T2		5
#define BIT_T1		4
#define BIT_ESR		3
#define BIT_ESL		2

#define BIT_M_Re	2
#define BIT_M_Li	1
#define BIT_M_An	0


// Hier die Steuerungsfunktion definieren
// ...

void emain(void* arg) 
{
	// Hier alle benoetigten Variablen deklarieren
	// ...

    // VOR DEBUGGING RESTART & FREEZE AN, BEI BREAKPOINT FREEZE AUS
	INIT_BM_WITH_REGISTER_UI; // Hier unbedingt einen Break-Point setzen !!!

	
	// Hier die Treiberfunktionen aufrufen und testen (Aufgabe 1)
	// ...
    unsigned short int wert=1, *DigitalValue;
    DigitalValue = &wert;
    Init(GlobalBoardHandle,0x9A);
    OutputByte(GlobalBoardHandle,PD,5);
    OutputByte(GlobalBoardHandle,PC,6);
    OutputByte(GlobalBoardHandle,PB,255);
    OutputByte(GlobalBoardHandle,PA,255);
    Init(GlobalBoardHandle, 0x9B);
    InputByte(GlobalBoardHandle,PD,&wert); //PORT D  auslesen
    printf("Wert in DigitalValue: ");
    printf("%i",wert);
    printf("\n");
    InputByte(GlobalBoardHandle,PC,&wert); //PORT D  auslesen
    printf("Wert in DigitalValue: ");
    printf("%i",wert);
    printf("\n");
    InputByte(GlobalBoardHandle,PB,&wert); //PORT D  auslesen
    printf("Wert in DigitalValue: ");
    printf("%i",wert);
    printf("\n");
    InputByte(GlobalBoardHandle,PA,&wert); //PORT D  auslesen
    printf("Wert in DigitalValue: ");
    printf("%i",wert);
    printf("\n");

    Free(GlobalBoardHandle);

    Init(GlobalBoardHandle, 0x9B);
    OutputByte(GlobalBoardHandle, PA, 255);
    Init(GlobalBoardHandle, 0x8B);
    OutputByte(GlobalBoardHandle, PA, 255);
    Init(GlobalBoardHandle, 0x80);
    OutputByte(GlobalBoardHandle, PD, 255);
    Free(GlobalBoardHandle);
    exit(0);



    /** Ab hier beginnt die Endlosschleife fuer den Automaten (Aufgabe 2)
	while(1) {

		SYNC_SIM; 

		// Hier die Eingabesignale einlesen
		// ...

		

		// Hier die Steuerungsfunktion aufrufen
		// ...




		// Hier die Ausgabesignale ausgeben
		// ...


    }
    **/
    // while(1)..

	
}

  
