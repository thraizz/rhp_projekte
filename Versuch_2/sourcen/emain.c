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

long int startposition;
long int endposition;
state_t currstate = Steht;

void Control(int T_NOTAUS, int T2, int ESR, int ESL, int* M_Li, int* M_An, int* M_Re, long int currposition, state_t* currstate){
    if(*currstate != Notaus && T_NOTAUS == 1){
        *currstate = Notaus;
        *M_Li = 0;
        *M_Re = 0;
        *M_An = 0;
    }

    switch(*currstate){
        case Steht:
            if(ESR == 1 && ESL == 0){
                *currstate = StarteRauf;
            }
            else if (ESR == 0 && ESL == 1){
                *currstate = StarteRunter;
            }
            break;
        case StarteRauf:
            if(currposition >= endposition){
                *currstate = FahreRauf_OHNE_OF;
            }
            else if(currposition < endposition){
                *currstate = FahreRauf_MIT_OF;
            }
            *M_Li = 1;
            *M_Re = 0;
            *M_An = 1;
            break;
        case StarteRunter:
            if(currposition > endposition){
                *currstate = FahreRunter_MIT_OF;
            }
            else if (currposition <= endposition){
                *currstate = FahreRunter_OHNE_OF;

            }
            *M_Li = 0;
            *M_Re = 1;
            *M_An = 1;
            break;
        case Notaus:
           if(T2 == 1 && T_NOTAUS == 0){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
           }
           break;
        case FahreRauf_MIT_OF:
            if (currposition<endposition && currposition>startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
            }
            else if (ESR == 1){
                *currstate=StarteRauf;
            }
            break;
        case FahreRauf_OHNE_OF:
            if (currposition<endposition || currposition>startposition){
                *currstate=Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
            }
            else if(ESR == 1){
                *currstate=StarteRauf;
            }
            break;
        case FahreRunter_MIT_OF:
            if(currposition>endposition && currposition<startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
            }
            if(ESL == 1){
                *currstate = StarteRunter;
            }
            break;
        case FahreRunter_OHNE_OF:
            if(currposition > endposition || currposition < startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
                }
            else if(ESL == 1){
                *currstate=StarteRunter;
            }
            break;
        }

        if(*currstate == StarteRauf){
            *M_Li = 1;
            *M_Re = 0;
            *M_An = 1;
            startposition=currposition;
            endposition = (startposition-60000) % 65535;
            if(endposition < 0){
                endposition = endposition + 65535;
            }
        }

        else if(*currstate == StarteRunter){
            *M_Li = 0;
            *M_Re = 1;
            *M_An = 1;
            startposition=currposition;
            endposition = (startposition+60000) % 65535;
        }

}

void emain(void* arg)
{
// VOR DEBUGGING RESTART & FREEZE AN, BEI BREAKPOINT FREEZE AUS
INIT_BM_WITH_REGISTER_UI;


// Aufgabe 1
unsigned short int wert=1, *DigitalValue;
DigitalValue = &wert;
Init(GlobalBoardHandle, 0x9A);
OutputByte(GlobalBoardHandle, PD, 5);
OutputByte(GlobalBoardHandle, PC, 6);
OutputByte(GlobalBoardHandle, PB, 255);
OutputByte(GlobalBoardHandle, PA, 255);
Init(GlobalBoardHandle, 0x9B);
InputByte(GlobalBoardHandle, PD, &wert); //PORT D  lesen, erlaubt
InputByte(GlobalBoardHandle, PC, &wert); //PORT C  lesen, verboten
InputByte(GlobalBoardHandle, PB, &wert); //PORT B  lesen, verboten
InputByte(GlobalBoardHandle, PA, &wert); //PORT A  lesen, verboten

Free(GlobalBoardHandle);

Init(GlobalBoardHandle, 0x9B); //Initialisiere Port A als Input
OutputByte(GlobalBoardHandle, PA, 255); //Schreiben Verboten
Init(GlobalBoardHandle, 0x8B); //Initialisiere Port A als Output
OutputByte(GlobalBoardHandle, PA, 255);//Schreiben erlaubt
Init(GlobalBoardHandle, 0x80); // Schreibe in jeden Port
OutputByte(GlobalBoardHandle, PA, 255);
OutputByte(GlobalBoardHandle, PB, 255);
OutputByte(GlobalBoardHandle, PC, 255);
OutputByte(GlobalBoardHandle, PD, 255);

    // Aufgabe 2
    Free(GlobalBoardHandle);
    Init(GlobalBoardHandle, 0x9A);


    while(1) {

        SYNC_SIM;

        unsigned short int d, c, b, a;
        int T_NOTAUS, T2, ESR, ESL, M_Re, M_Li, M_An;
        long int currposition = 0;

        // Einlesen


        InputByte(GlobalBoardHandle, 0, &a);
        InputByte(GlobalBoardHandle, 1, &b);
        currposition = (b << 8) | a;

        // Taster
        InputByte(GlobalBoardHandle, 2, &c);
        ESR = (c >> BIT_ESR) & 0x1;
        ESL = (c >> BIT_ESL) & 0x1;
        T2 = (c >> BIT_T2) & 0x1;
        T_NOTAUS = (c >> BIT_T1) & 0x1;

        // Bestimme aktion
        Control(T_NOTAUS, T2, ESR, ESL, &M_Li, &M_An, &M_Re, currposition, &currstate);


        // Output...
        d = M_Re << BIT_M_Re| M_Li << BIT_M_Li | M_An;
        OutputByte(GlobalBoardHandle, 3, d);


    }
    Free(GlobalBoardHandle);


}



#define BIT_ESR		3
#define BIT_ESL		2

#define BIT_M_Re	2
#define BIT_M_Li	1
#define BIT_M_An	0


// Hier die Steuerungsfunktion definieren
// ...

long int startposition;
long int endposition;
state_t currstate = Steht;

void Control(int T_NOTAUS, int T2, int ESR, int ESL, int* M_Li, int* M_An, int* M_Re, long int currposition, state_t* currstate){
    if(*currstate != Notaus && T_NOTAUS == 1){
        *currstate = Notaus;
        *M_Li = 0;
        *M_Re = 0;
        *M_An = 0;
    }

    switch(*currstate){
        case Steht:
            if(ESR == 1 && ESL == 0){
                *currstate = StarteRauf;
            }
            else if (ESR == 0 && ESL == 1){
                *currstate = StarteRunter;
            }
            break;
        case StarteRauf:
            if(currposition >= endposition){
                *currstate = FahreRauf_OHNE_OF;
            }
            else if(currposition < endposition){
                *currstate = FahreRauf_MIT_OF;
            }
			*M_Li = 1;
            *M_Re = 0;
            *M_An = 1;
            break;
        case StarteRunter:
            if(currposition > endposition){
                *currstate = FahreRunter_MIT_OF;
            }
            else if (currposition <= endposition){
                *currstate = FahreRunter_OHNE_OF;

            }
			*M_Li = 0;
            *M_Re = 1;
            *M_An = 1;
            break;
        case Notaus:
           if(T2 == 1 && T_NOTAUS == 0){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
           }
           break;
        case FahreRauf_MIT_OF:
            if (currposition<endposition && currposition>startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
            }
            else if (ESR == 1){
                *currstate=StarteRauf;
            }
            break;
        case FahreRauf_OHNE_OF:
            if (currposition<endposition || currposition>startposition){
                *currstate=Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
            }
            else if(ESR == 1){
                *currstate=StarteRauf;
            }
            break;
        case FahreRunter_MIT_OF:
            if(currposition>endposition && currposition<startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
				break;
            }
            if(ESL == 1){
                *currstate = StarteRunter;
            }
            break;
        case FahreRunter_OHNE_OF:
            if(currposition > endposition || currposition < startposition){
                *currstate = Steht;
                *M_Li = 0;
                *M_Re = 0;
                *M_An = 0;
                break;
                }
            else if(ESL == 1){
                *currstate=StarteRunter;
            }
            break;
        }

        if(*currstate == StarteRauf){:
            *M_Li = 1;
            *M_Re = 0;
            *M_An = 1;
            startposition=currposition;
            endposition = (startposition-60000) % 65535;
            if(endposition < 0){
                endposition = endposition + 65535;
            }
        }

        else if(*currstate == StarteRunter){
            *M_Li = 0;
            *M_Re = 1;
            *M_An = 1;
            startposition=currposition;
            endposition = (startposition+60000) % 65535;
        }
    }

void emain(void* arg) 
{
    // VOR DEBUGGING RESTART & FREEZE AN, BEI BREAKPOINT FREEZE AUS
	INIT_BM_WITH_REGISTER_UI; 

	
	// Aufgabe 1
    unsigned short int wert=1, *DigitalValue;
    DigitalValue = &wert;
    Init(GlobalBoardHandle, 0x9A);
    OutputByte(GlobalBoardHandle, PD, 5);
    OutputByte(GlobalBoardHandle, PC, 6);
    OutputByte(GlobalBoardHandle, PB, 255);
    OutputByte(GlobalBoardHandle, PA, 255);
    Init(GlobalBoardHandle, 0x9B);
    InputByte(GlobalBoardHandle, PD, &wert); //PORT D  lesen, erlaubt
    InputByte(GlobalBoardHandle, PC, &wert); //PORT C  lesen, verboten
    InputByte(GlobalBoardHandle, PB, &wert); //PORT B  lesen, verboten
    InputByte(GlobalBoardHandle, PA, &wert); //PORT A  lesen, verboten

    Free(GlobalBoardHandle);

    Init(GlobalBoardHandle, 0x9B); //Initialisiere Port A als Input
    OutputByte(GlobalBoardHandle, PA, 255); //Schreiben Verboten
    Init(GlobalBoardHandle, 0x8B); //Initialisiere Port A als Output
    OutputByte(GlobalBoardHandle, PA, 255);//Schreiben erlaubt
    Init(GlobalBoardHandle, 0x80); // Schreibe in jeden Port
    OutputByte(GlobalBoardHandle, PA, 255);
    OutputByte(GlobalBoardHandle, PB, 255);
    OutputByte(GlobalBoardHandle, PC, 255);
    OutputByte(GlobalBoardHandle, PD, 255);
   

	// Aufgabe 2
	Free(GlobalBoardHandle);
    Init(GlobalBoardHandle, 0x9A);

    
	while(1) {

		SYNC_SIM; 

        unsigned short int d, c, b, a;
        int i, T_NOTAUS, T2, ESR, ESL, M_Re, M_Li, M_An;
        long int currposition = 0;

		// Einlesen


		InputByte(GlobalBoardHandle, 0, &a);
        InputByte(GlobalBoardHandle, 1, &b);
		currposition = (b << 8) | a;

		// Taster
		InputByte(GlobalBoardHandle, 2, &c);
		ESR = (c >> 3) & 0x1;
        ESL = (c >> 2) & 0x1;
        T2 = (c >> 5) & 0x1;
        T_NOTAUS = (c >> 4) & 0x1;
		
		// Bestimme aktion
        Control(T_NOTAUS, T2, ESR, ESL, &M_Re, &M_Li, &M_An, currposition, &currstate);


		// Output...
        d = M_Re << 2| M_Li << 1| M_An;
        OutputByte(GlobalBoardHandle, 3, d);


    }
    Free(GlobalBoardHandle);

	
}

  
