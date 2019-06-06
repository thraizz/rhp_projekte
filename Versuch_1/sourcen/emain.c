#include "simuc.h"

///////////////////////////////////////////////////////////////////////////////////////
// Bedingte Compilierung zur Bestimmung welches "Beispiel" compiliert werden soll  ///
//
// Es darf immer nur ein "define" aktive, d.h. nicht auskommentiert, sein.
//
//#define V1_Aufgabe_1
//#define V1_Aufgabe_2
#define V1_Aufgabe_3
//
///////////////////////////////////////////////////////////////////////////////////////


#ifdef V1_Aufgabe_1


void emain(void* arg)
{
    short int zahl1;
    short int zahl2;
    short int ergebnis;
    char string_1[100];
    char string_2[100];
    char ergebnisstring[100];
    short int pref = 0 ;
    short int mod = 0;
    short int multi = 1;
    short int j;
    short int i;
    short int k;
    short int zwergebnis2;
    short int zwergebnis3;
    short int zwergebnis4;
    short int stelle;
    short int stelle2;



    INIT_BM_WITHOUT_REGISTER_UI;


  do {

    putstring("Bitte 1. Zahl eingeben:\n");
    getstring(string_1);
    wait_for_return();

    pref= 0;

    for (i = 0; string_1[i] != '\0' ;i++)
        {
        if (string_1[i] < '0' || string_1[i] > '9' || i > 3)
            {
            pref = 1;
            break;
     }
    }

    if (pref != 0)
        {
        putstring("Fehler \n");
        }
    }
    while (pref != 0);

    for ( i = 0 ; string_1[i] != '\0'; i++)
    {
        mod++;
    }
     mod--;
     for (j = 0; string_1[j] != '\0'; j++){
         for (k= 0, multi = 1; k<mod;k++){ //mod um die stelle hoch zählen bis es die benötige mutiplikator wert erreicht
             multi *= 10;
         }
         mod--; // runterzählen um auf niederwertige zahl zu kommen

         zahl1 += (string_1[j] - '0')*multi;
     }


     // Start Zahl 2

     mod=0;
     do {

       putstring("Bitte 2. Zahl eingeben:\n");
       getstring(string_2);
       wait_for_return();

       pref= 0;

       for (i = 0; string_2[i] != '\0' ;i++)
           {
           if (string_2[i] < '0' || string_2[i] > '9' || i > 3)
               {
               pref = 1;
               break;
        }
       }

       if (pref != 0)
           {
           putstring("Fehler \n");
           }
       }
       while (pref != 0);

       for ( i = 0 ; string_2[i] != '\0'; i++)
       {
           mod++;
       }
        mod--;
        for (j = 0; string_2[j] != '\0'; j++){
            for (k= 0, multi = 1; k<mod;k++){ //mod um die stelle hoch zählen bis es die benötige mutiplikator wert erreicht
                multi *= 10;
            }
            mod--; // runterzählen um auf niederwertige zahl zu kommen

            zahl2 += (string_2[j] - '0')*multi;
        }
        ergebnis = zahl1 +zahl2;


        //int to char
        zwergebnis2 = ergebnis;
        zwergebnis3 = ergebnis;

        while(zwergebnis2 != 0)
        {
            zwergebnis2 /= 10;
            stelle++;
        }
        stelle2 = stelle-1;
        for(i = 0; i < stelle; i++)
        {
          zwergebnis4 = zwergebnis3 % 10;

          ergebnisstring[stelle2] = '0' + zwergebnis4;

          stelle2--;

          zwergebnis3 /= 10;

        }
            putstring(ergebnisstring);



    END;
}
#endif // V1_Aufgabe_1



#ifdef V1_Aufgabe_2

void emain(void* arg)
{
    unsigned short int T1, T2;
    unsigned short int temp, help;
    int leftOrRight = -1; //left =0 ;right =1
    int i = 0;
    boolean outofhere = 0 ;

    // Mit den folgenden beiden Defines kann bestimmt werden welche Simulations-Tools verwendet werden.
    // Diese MUESSEN dann auch gestartet werden.
    // ######### IN DIESEM VERSUCH MUESSEN SIE OHNE DAS BANDMODELL ARBEITEN,             ##############
    // ######### da dieses sonst die Eingabe ueber das User-Interface "ueberschreibt".   ##############
    //INIT_BM_WITH_REGISTER_UI;		// Es werden beide Simulations-Tools (Bandmodell.exe und User_Interface.exe) verwendet
    INIT_REGISTER_UI_WITHOUT_BM		// Es wird nur das User-Interface zur Registerdarstellung (User_Interface.exe) verwendet.

    // Initialisierung gemaess Unterpunkt b)
    temp = (1 << 5) | (1 << 4); // temp mit dem Bitmuster 0x0030
                                // initialisieren bei dem nur Bit 5
                                // und Bit 4 gesetzt (1) sind

    temp = ~temp;				// Alle Bits von temp negieren
                                // temp enthält nun das
                                // Bitmuster 0xFFCF

    io_out16 (DIR1, ( io_in16(DIR1) & temp) );	// Nur Bit 5 und 4 den
                                            // Wert 0 ( Lesen ) geben und
                                            // alle anderen Bits unverändert
                                            // lassen.

    temp = (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9);
    io_out16 (DIR1, ( io_in16(DIR1) |  temp) ); //Bit 12 - 9
                                            // auf 1 (Schreiben) setzen. Alle
                                            // anderen Bits unverändert
                                            // lassen.

    while(1) {

        SYNC_SIM;

        temp = io_in16(IN1);		// Einlesen von Port 1

        temp = temp >> 5;	// So oft nach rechts shiften
                            // bis das Bit 5 an der Bitstelle 0 steht.

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

        temp = temp >> 4;	// So oft nach rechts shiften
                            // bis das Bit 4 an der Bitstelle 0 steht.

        T2 = temp & 0x01;	// Alle Bits, bis auf Bit0 auf 0 setzen
                            // und das Ergebnis T2 zuweisen.

        if((T1 == 1 && T2 == 1) || (T1 == 0 && T2 == 0) )
        {

            temp = 1 << 12 | 1 << 11 |1 << 10 | 1 << 9;

            //temp =  ~temp;

            io_out16 (OUT1, (io_in16(OUT1) & temp) );

        }

        if (T1 == 1 && T2 == 0) { //rechts

            temp = 1 << 9;	// Berechnung des Bitmusters 0x0200
                            // bei dem nur das Bit 9 gesetzt ist.
            while(1)
            {
                for(i = 9; i <= 12; i++)
                {
                    temp = 1 << i;

                io_out16(OUT1 , temp);

                temp = io_in16(IN1);
                temp = temp >> 5;
                T1 = temp & 0x01;
                temp = in(IN1);
                temp = temp >> 4;
                T2 = temp & 0x01;

                if (T1 == 0 || T2 == 1 )
                {
                    outofhere = 1;
                    break;
                }

            }
            if (outofhere == 1)
            {/*
                for (i = 9; i <=12; i++)
                {
                    temp = 1 << i;
                    io_out16(OUT1, (temp & 0));
            }*/

            outofhere = 0;
            break;
        }
        else
        {
            temp = 1 << 12;

            io_out16(OUT1, (temp & 0));
        }
      }
    }

        if (T2 == 1 && T1 == 0){ // links
            temp = 1 <<12;	// Berechnung des Bitmusters 0x0200
                    // bei dem nur das Bit 9 gesetzt ist.
            while(1)
            {
                for(i = 12; i >= 9; i--)
                {
                temp = 1 << i;

                io_out16(OUT1, temp);

                temp = io_in16(IN1);
                temp = temp >> 5;
                T1 = temp & 0x01;
                temp = in(IN1);
                temp = temp >> 4;
                T2 = temp & 0x01;

                if (T2 == 0 || T1 == 1)
                {
                    outofhere = 1;
                    break;
                }

            }
            if (outofhere == 1)
            {
               /* for (i = 9; i <=12; i++)
                {
                temp = 1 << i;
                io_out16(OUT1, (temp & 0));
            }*/

            outofhere = 0;
            break;
            }
            else
            {
            temp = 1 << 9;

            io_out16(OUT1, (temp & 0));
        }
    }
}
}
END;
}

#endif // V1_Aufgabe_2

#define HIGHBIT 15 // 9 bis 15
#define LOWBIT 11 // 5 bis 14
#define MASK 0x1f
#define BITMUSTER 0b10110

#ifdef V1_Aufgabe_3

void emain(void* arg)
{
    unsigned short int T1, T2;
    unsigned short int temp, portwert, wert, startwert;

    INIT_REGISTER_UI_WITHOUT_BM

    temp = (1 << 5) | (1 << 4);

    temp = ~temp;

    io_out16(DIR1, (io_in16(DIR1)& temp ));

    startwert = BITMUSTER << LOWBIT;

    while (1)
    {
        SYNC_SIM;

        temp = io_in16(IN1);

        temp = temp >> 4;

        T1 = temp & 0x01;

        temp = temp >> 5;

        T2 = temp & 0x01;

        temp = MASK << LOWBIT;

        io_out16(DIR1, (io_in16(DIR1) |temp ));

        if (T1 == 1 && T2 == 0)
        {
            portwert = (io_in16(OUT1));

            if ((portwert & MASK ) << LOWBIT == 0)
            {
                io_out16(OUT1, portwert | startwert);
            }
            else
            {
                wert = MASK & (portwert >> LOWBIT); // rechtsbündig in wert speichern

                if ((wert & 0x01)== 1) // wenn rechts eins
                {
                    wert = wert >> 1; // wert um eine stelle nach rechts verschieben ok
                    temp = 0x01 << (HIGHBIT - LOWBIT); // und dann
                    wert = wert | temp; // die rechts an linken rand anbringen
                    portwert = wert << LOWBIT;
                }
                else
                {
                    wert = wert >> 1; //wenn kein 1: an rechten rand einfach verschieben wird auto. mit nullen aufgefüllt
                    portwert = wert << LOWBIT;
                }
                io_out16(OUT1 , portwert);

             }
          }
        if (T2 == 1 && T1 == 0)
        {
            portwert = (io_in16(OUT1));

            if ((portwert & MASK << LOWBIT)== 0 )
            {
                io_out16(OUT1, portwert |startwert); //startwert ausgeben
            }
            else
            {
                wert = MASK & (portwert >> LOWBIT);//rechtsbündig in wert speichern

                if ((wert >> (HIGHBIT - LOWBIT)& 0x01) == 1) // wenn links eine eins dann wert um eine Stelle nach links verschiben
                {
                    wert = wert << 1;
                    temp = 0x01;
                    wert = wert | temp;
                    portwert = wert << LOWBIT;
                }
                else
                {
                    wert = wert << 1;
                    portwert = wert << LOWBIT;
                }
                io_out16(OUT1, portwert);

            }
        }
    }


    END;
}
#endif // V1_Aufgabe_3


//  //
