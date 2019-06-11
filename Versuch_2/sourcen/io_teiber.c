#include "simuc.h"
#include "io_treiber.h"

typedef struct BHandle_Data {
	BYTE Board_allocated;	// 1=allocated, 0=free
	BYTE Port_A_Direction;	// 1=Output, 0=Input
	BYTE Port_B_Direction;
	BYTE Port_C_Direction;
	BYTE Port_D_Direction;
} BHandle;


BHandle BoardHandle_Data;
DSCB GlobalBoardHandle = &BoardHandle_Data;



BYTE Init(DSCB BoardHandle, unsigned long int Steuerwort) {
    if(Steuerwort<128 || 155<Steuerwort){
        return 1;
    }
    else{
        BoardHandle->Board_allocated=1;
        if((Steuerwort & 1)==1){
            BoardHandle->Port_D_Direction = 0;
        }
        else{
            BoardHandle->Port_D_Direction = 1;
        }
        if((Steuerwort & 8)==1){
            BoardHandle->Port_C_Direction = 0;
        }
        else{
            BoardHandle->Port_C_Direction = 1;
        }
        if((Steuerwort & 2)==1){
            BoardHandle->Port_B_Direction = 0;
        }
        else{
            BoardHandle->Port_B_Direction = 1;
        }
        if((Steuerwort & 16)==1){
            BoardHandle->Port_A_Direction = 0;
        }
        else{
            BoardHandle->Port_A_Direction = 1;
        }


        return 0;
    }
}

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
	 
    if(BoardHandle->Board_allocated==1){
        if(Port==PA && BoardHandle->Port_A_Direction==0){
            DigitalValue = io_in16(IN0);
            DigitalValue = (DigitalValue & 127); //Filtern des oberen Byte
        }

        else if(Port==PB && BoardHandle->Port_B_Direction==0){
            DigitalValue = io_in16(IN0);
            DigitalValue = (DigitalValue & (127<<8));
        }

        else if(Port==PC && BoardHandle->Port_C_Direction==0){
            DigitalValue = io_in16(IN1);
            DigitalValue = (DigitalValue & 127);
        }

        else if(Port==PD && BoardHandle->Port_D_Direction==0){
            DigitalValue = io_in16(IN1);
            DigitalValue = (DigitalValue & (127<<8));
        }

        else{ // Port ungültig oder falsch konfiguriert
            return 2;
        }

        return 0;
    }

    return 1; // Ungültiges BoardHandle
}

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue) {
    short int t;
    if(BoardHandle->Board_allocated==1){
        if(Port==PA){
            if(BoardHandle->Port_A_Direction==0){
                return 3;
            }
            t = (io_in16(IN0) >> 8); //t ist der zu erhaltene obere Byte
            t = t & 127; // Bereinige alles außer oberer Byte der jetzt unten steht
            t = (t << 8) | DigitalValue; // Shifte oberen Byte zurück nach oben, schreibe unteren Byte
            io_out16(OUT0,t); //Schreibe gesamten Inhalt
        }
        else if(Port==PB){
            if(BoardHandle->Port_B_Direction==0){
                return 3;
            }
            t = (io_in16(IN0) & 127); //t ist der zu erhaltene untere Byte, bereinige oberen Byte
            t = t  | (DigitalValue << 8); // Shifte oberen Byte zurück nach oben, schreibe unteren Byte
            io_out16(OUT0,t); //Schreibe gesamten Inhalt
        }
        else if(Port==PC){
            if(BoardHandle->Port_C_Direction==0){
                return 3;
            }
            t = (io_in16(IN1) >> 8);
            t = t & 127;
            t = (t << 8) | DigitalValue;
            io_out16(OUT1,t)
        }
        else if(Port==PD){
            if(BoardHandle->Port_D_Direction==0){
                return 3;
            }
            t = (io_in16(IN1) & 127);
            t = t  | (DigitalValue << 8);
            io_out16(OUT1,t);
        }
        else{
            return 2;
        }
        return 0;
    }
    return 1;
}

BYTE Free(DSCB BoardHandle) {

    if(BoardHandle->Board_allocated==1){
        ~BoardHandle;
        return 0;
    }
    return 1;
}

