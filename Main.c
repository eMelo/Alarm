#include <stdio.h>
#include <stdlib.h>
#include<pic18f4550.h>
#include <plib/usart.h>
#include <xc.h>
#include "Keyboard.h"

#define ACTIVADA PORTAbits.RA0
#define DISPONIBLE PORTAbits.RA1
#define SOUND PORTEbits.RE2
#define BUZZER PORTEbits.RE1

const int MaxCount = 8000;
const int KeyDelay = 8000;
unsigned char password[]="6691", checker[5];
char datax[20];
unsigned char Keyboard[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

union Areas
{
    struct
    {
        unsigned Sala  :1;
        unsigned Terraza  :1;
        unsigned Galeria  :1;
        unsigned Bocina  :1;
        unsigned Sensor  :1;
        unsigned Inversor  :1;
        unsigned CuartoPapi  :1;
        unsigned CuartoHermanos  :1;
    };
    struct
    {
        unsigned  char all;
    };
}Sensor_Area;

void UartConfig(void)
{
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    SPBRG   = 51; //51  25
    TXSTA   = 0b00100100;
    RCSTA   = 0b10010000;
    BAUDCON = 0b00000000;

    PIE1bits.RCIE = 1;
    IPR1bits.RCIP = 1;

    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;

}

int passCheck(void)
{
    int temp = 0;
    for(temp; temp < 4;temp++)
        if(checker[temp] != password[temp])
            return 0;
    return 1;
}

void CheckArea(void)
{
    Sensor_Area.all = PORTD;
}

int CheckRow(void)
{
    int c = 0;  //Temporal
    if(PORTBbits.RB4 == 1)
    {
        while(PORTBbits.RB4 == 1 && c < KeyDelay)
            c++;
        return 1;
    }
    if(PORTBbits.RB5 == 1)
    {
        while(PORTBbits.RB5 == 1 && c < KeyDelay)
            c++;
        return 2;
    }
    if(PORTBbits.RB6 == 1)
    {
        while(PORTBbits.RB6 == 1 && c < KeyDelay)
            c++;
        return 3;
    }
    if(PORTBbits.RB7 == 1)
    {
        while(PORTBbits.RB7 == 1 && c < KeyDelay)
            c++;
        return 4;
    }
    return 0;
}

int CheckKeyboard(int digit)
{
    //Display **** to Screen
    //Si le dan a una de las Teclas Desplegar un Asterisco
    int x = 0, pos = 0, col = 0, row = 0;
    for(x = 0; x < MaxCount && pos < digit; x++)
    {
        PORTA = (PORTA & 0xC3);
        PORTAbits.RA2 = 1;
        {
            col = 0;
            row = CheckRow();
            if(row)
            {
                checker[pos] = Keyboard[row-1][col];
                pos++;
                x = 0;
            }
        }

        PORTAbits.RA2 = 0;
        PORTAbits.RA3 = 1;
        {
            col = 1;
            row = CheckRow();
            if(row)
            {
                checker[pos] = Keyboard[row-1][col];
                pos++;
                x = 0;
            }
        }

        PORTAbits.RA3 = 0;
        PORTAbits.RA4 = 1;
        {
            col = 2;
            row = CheckRow();
            if(row)
            {
                checker[pos] = Keyboard[row-1][col];
                pos++;
                x = 0;
            }
        }

        PORTAbits.RA4 = 0;
        PORTAbits.RA5 = 1;
        {
            col = 3;
            row = CheckRow();
            if(row)
            {
                checker[pos] = Keyboard[row-1][col];
                pos++;
                x = 0;
            }
        }
        PORTAbits.RA5 = 0;
    }
    if(pos > 3 && digit > 3)
    {
        passCheck();
    }
}

void sendTX(char data)
{
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

void writeTX(int len)
{
    int x = 0;
    while(x<len)
    {
        sendTX(datax[x]);
        x++;
    }
}

int main()
{
    /*Configuracion de Columnas del Teclado*/
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;

    /*Configuracion de las Filas del Teclado*/
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;

    /*Leds Indicadores de Status*/
    TRISAbits.RA0 = 0;  //Activada
    TRISAbits.RA1 = 0;  //Disponible

    /*Puerto INdicador de SOnido*/
    PORTEbits.RE2 = 0;
    TRISEbits.RE2 = 0;
    
    /*Uso del Puerto D Para Deteccion de las Areas*/
    TRISD = 0xFF;

    while(1)
    {
        if(ACTIVADA == 1)
        {
            //En este caso la Alarma esta Activada
            CheckKeyboard(4);   //Tomo 4 digitos correspondites a la Clave
            //Chequeo el Password
            if(passCheck())
            {
                int try = 0, out = 0;
                while(try < 5 && out < KeyDelay)
                {
                    //Hasta un numero de Intnetos X, y un tiempo determinado
                    if(CheckKeyboard(1))
                    {
                        //Desactivar
                        if(checker[0] == 'D')
                        {
                            ACTIVADA = 0;
                            SOUND = 0;
                        }
                        try++;
                    }
                    out++;
                }
                //Si usuario no recuerda la Clave o Alguien mas intenta acceder
                if(try == 5)
                    SOUND = 1;
            }
            if(PORTD != 0xFF)
            {
                SOUND = 1;
                //UART MOSTRAR EL AREA DONDE FUE ACTIVADA
            }
        }
        else if(ACTIVADA == 0)
        {
            SOUND = 0;
            //En este caso la alarma esta desativada
            if(PORTD == 0xFF)
                DISPONIBLE = 1;

            else
            {
                DISPONIBLE = 0;
                CheckArea();
                //Desplegar Por UART El area
                /*Proceso*/

                //Introducir el Codigo
                if(CheckKeyboard(4))
                {
                    //Introducir Accion
                    if(CheckKeyboard(1))
                    {
                        if(checker[0]=='A')
                        {
                            SOUND = 0;
                            ACTIVADA = 1;
                        }
                        else if(checker[0] == 'B')
                            SOUND = 1;
                        else if(checker[0] == 'C')
                            SOUND = 0;
                        else if(checker[0] == 'D')
                        {
                            SOUND = 0;
                            ACTIVADA = 0;
                        }
                    }
                }   
            }
        }
    }
    return 0;
}