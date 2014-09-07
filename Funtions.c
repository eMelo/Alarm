#include <pic18f4550.h>
#include <xc.h>
const int DelayCount = 8000;
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
    int y = 0;
    while(!RCIF && y < DelayCount) //Waits for Reception to complete
        y++;
    if(y == DelayCount || RCREG == 'E' || RCREG == 'e')
        return 'N';
    return RCREG; //Returns the 8 bit data
}

void sendTX(char data)
{
    int z = 0;
    while(!TRMT && z < DelayCount)
        z++;
    TXREG = data;
}

void writeTX(char *data,int len)
{
    int x = 0, try = 0,out = 0;
    while(!out)
    {
        while(x<len)
        {
            sendTX(data[x]);
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

}
