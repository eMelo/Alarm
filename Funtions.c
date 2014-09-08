#include <pic18f4550.h>
#include <xc.h>
const int DelayCount = 300; //10 Segundos  --> un segundo = 30 conteos
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

char UART_Read()
{
    if(TMR0 > 65000)
        TMR0 = 0;
    int y = TMR0;
    while(!RCIF && ((TMR0-y) < DelayCount)); //Waits for Reception to complete
    if(y == DelayCount || RCREG == 'E' || RCREG == 'e')
        return 'N';
    return RCREG; //Returns the 8 bit data
}

void sendTX(char data)
{
    if(TMR0 > 65000)
        TMR0 = 0;
    int z = TMR0;
    while(!TRMT && ((TMR0-z) < DelayCount));  //Waits for Transmition to complete
    TXREG = data;
}

void writeTX(char *data,int len)
{
    int x = 0, try = 0,out = 0;
    while(!out)
    {
        while(x<len)
        {
            sendTX(*(data+x));
            x++;
        }
        char reader = UART_Read();
        if(reader == 'N'&& try < 3)
        {
            try++;
            out=0;
        }
        else
            out = 1;
    }
}
void timerConfig(void)
{
    T0CON = 0b10000111;
    TMR0 = 0;
}
