#include <pic18f4550.h>

void UartConfig(void)
{
    //COnfiguracion de los Puertos
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    SPBRGH  = 0;
    SPBRG   = 16;

    TXSTAbits.TXEN = 1; //transmition enable
    TXSTAbits.BRGH = 1; //High Speed Baud Rate

    RCSTAbits.SPEN = 1; //Serial Por Enable
    RCSTAbits.CREN = 1;

    BAUDCONbits.BRG16 = 1;

    //Para el Registro de Recepcion
    PIE1bits.RCIE = 1;
    IPR1bits.RCIP = 1;

    //8Mhz
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;

}

