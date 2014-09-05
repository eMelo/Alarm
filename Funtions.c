#include <pic18f4550.h>
#include <xc.h>

void UartConfig(void)
{
    SPBRG   = 51;  //8MHZ --> 9600 BaudRate

    BAUDCON = 0;

    TXSTAbits.TXEN = 1; //transmition enable
    TXSTAbits.BRGH = 1; //High Speed Baud Rate
    TXSTAbits.SYNC = 0; //Async

    RCSTAbits.SPEN = 1; //Serial Por Enable
    RCSTAbits.CREN = 1; //Recepcon Enable

    //COnfiguracion de los Puertos
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    //Para el Registro de Recepcion
    PIE1bits.RCIE = 1;
    IPR1bits.RCIP = 1;

    //8Mhz
    
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    
    OSCCONbits.SCS1 = 1;
    OSCCONbits.SCS0 = 1;
    
}

