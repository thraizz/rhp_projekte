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
            io_out16(DIR1, (io_in16(DIR1)&0xff00));
        }
        else{
            BoardHandle->Port_D_Direction = 1;
            io_out16(DIR1, (io_in16(DIR1)|0xff00));
        }
        if((Steuerwort & 8)==8){
            BoardHandle->Port_C_Direction = 0;
            io_out16(DIR1,(io_in16(DIR1)&0xff00));
        }
        else{
            BoardHandle->Port_C_Direction = 1;
            io_out16(DIR1,(io_in16(DIR1)|0x00ff));
        }
        if((Steuerwort & 2)==2){
            BoardHandle->Port_B_Direction = 0;
            io_out16(DIR0, (io_in16(DIR0)&0xff00));
        }
        else{
            BoardHandle->Port_B_Direction = 1;
            io_out16(DIR0, (io_in16(DIR0)|0xff00));
        }
        if((Steuerwort & 16)==16){
            BoardHandle->Port_A_Direction = 0;
            io_out16(DIR0,(io_in16(DIR0)&0x0000));
        }
        else{
            BoardHandle->Port_A_Direction = 1;
            io_out16(DIR0,(io_in16(DIR0)|0x00ff));
        }


        return 0;
    }
}

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
    short int t;
    if(BoardHandle->Board_allocated==1){
        if(Port==PA && BoardHandle->Port_A_Direction==0){
            t = io_in16(OUT0);
            t = (t & 0xFF);
            *DigitalValue = t; //Filtern des oberen Byte
        }

        else if(Port==PB && BoardHandle->Port_B_Direction==0){
            t = io_in16(OUT0);
            t = (t & 0xFF00);
            *DigitalValue = t>>8;
        }

        else if(Port==PC && BoardHandle->Port_C_Direction==0){
            t = io_in16(OUT1);
            t = (t & 0xFF);
            *DigitalValue = t;
        }

        else if(Port==PD && BoardHandle->Port_D_Direction==0){
            t = io_in16(OUT1);
            t = (t & 0xFF00);
            *DigitalValue = t>>8;
        }

        else{ // Port ungültig oder falsch konfiguriert
            return 2;
        }

        return 0;
    }

    return 1; // Ungültiges BoardHandle
}

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue) {
    unsigned short int t;
    if(BoardHandle->Board_allocated==1){
        if(Port==PA){
            if(BoardHandle->Port_A_Direction==0){
                return 3;
            }
            t = (io_in16(OUT0) & 0xFF00); //t ist der zu erhaltene obere Byte
            t = t | (0xFF & DigitalValue); // Shifte oberen Byte zurück nach oben, schreibe unteren Byte
            io_out16(OUT0,t); //Schreibe gesamten Inhalt
        }
        else if(Port==PB){
            if(BoardHandle->Port_B_Direction==0){
                return 3;
            }
            t = (io_in16(OUT0) & 0xFF); //t ist der zu erhaltene untere Byte, bereinige oberen Byte
            t = t  | (0xFF00 & (DigitalValue << 8)); // Shifte oberen Byte zurück nach oben, schreibe unteren Byte
            io_out16(OUT0,t); //Schreibe gesamten Inhalt
        }
        else if(Port==PC){
            if(BoardHandle->Port_C_Direction==0){
                return 3;
            }
            t = (io_in16(OUT1) & 0xFF00);
            t = t | (0xFF & DigitalValue);
            io_out16(OUT1,t)
        }
        else if(Port==PD){
            if(BoardHandle->Port_D_Direction==0){
                return 3;
            }
            t = (io_in16(OUT1) & 0xFF);
            t = t  | (0xFF00 & (DigitalValue<<8));
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
        BoardHandle->Board_allocated = 0;
        return 0;
    }
    return 1;
}

