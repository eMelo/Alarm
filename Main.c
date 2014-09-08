#include <stdio.h>
#include <stdlib.h>
#include<pic18f4550.h>
#include <plib/usart.h>
#include <xc.h>
#include "Keyboard.h"

void UartConfig(void);
void writeTX(char *data,int len);
void sendTX(char data);
char UART_Read();
void timerConfig(void);

#define ACTIVADA PORTAbits.RA0
#define DISPONIBLE PORTAbits.RA1
#define SOUND PORTEbits.RE2
#define BUZZER PORTEbits.RE1

const int MaxCount = 8000;
const int KeyDelay = 8000;
unsigned char password[]="6691", checker[5];
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
    writeTX("sd0,0;",6);
    writeTX("ssSUPERVISAR AREAS;",19);
    writeTX("sd1,0;",6);
    Sensor_Area.Sala == 0           ? writeTX("ss 1;",5): writeTX("ss  ;",5);
    Sensor_Area.Terraza == 0        ? writeTX("ss 2;",5): writeTX("ss  ;",5);
    Sensor_Area.Galeria == 0        ? writeTX("ss 3;",5): writeTX("ss  ;",5);
    Sensor_Area.Bocina == 0         ? writeTX("ss 4;",5): writeTX("ss  ;",5);
    Sensor_Area.Sensor == 0         ? writeTX("ss 5;",5): writeTX("ss  ;",5);
    Sensor_Area.Inversor == 0       ? writeTX("ss 6;",5): writeTX("ss  ;",5);
    Sensor_Area.CuartoPapi == 0     ? writeTX("ss 7;",5): writeTX("ss  ;",5);
    Sensor_Area.CuartoHermanos == 0 ? writeTX("ss 8;",5): writeTX("ss  ;",5);
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
                if(row == 4)
                    if(pos > 0)
                        pos--;

                else
                {
                    checker[pos] = Keyboard[row-1][col];
                    pos++;
                    x = 0;
                }
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
                if(pos == 4)
                    pos = 0;
                
                else
                {
                    checker[pos] = Keyboard[row-1][col];
                    pos++;
                    x = 0;
                }
            }
        }
        PORTAbits.RA5 = 0;
    }
    if(pos > 3 && digit > 3)
    {
        passCheck();
    }
}

int main()
{
    //Configuracion de Puertos
    portConfig();

    //Configuracion del Uart
    UartConfig();

    //Configuracion del TIMER
    timerConfig();
    PORTA = 0xFF;
    LATA = 0xFF;
    while(1)
    {
        if(ACTIVADA == 1)
        {
            writeTX("sd0,0;",6);
            writeTX("ss   INTRODUCIR   ;",19);
            writeTX("sd1,0;",6);
            writeTX("ss     CODIGO     ;",19);
            //En este caso la Alarma esta Activada
            CheckKeyboard(4);   //Tomo 4 digitos correspondites a la Clave
            //Chequeo el Password
            if(passCheck())
            {
                int try = 0, out = 0;
                while(try < 5 && out < KeyDelay)
                {
                    //Hasta un numero de Intnetos X, y un tiempo determinado
                    writeTX("sd0,0;",6);
                    writeTX("ss   INTRODUCIR   ;",19);
                    writeTX("sd1,0;",6);
                    writeTX("ss     ACCION     ;",19);
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
                CheckArea();
            }
        }
        else if(ACTIVADA == 0)
        {
            SOUND = 0;
            //En este caso la alarma esta desativada
            
            if(PORTD == 0xFF)
            {
                DISPONIBLE = 1;
                writeTX("sd0,0;",6);
                writeTX("ss   INTRODUCIR   ;",19);
                writeTX("sd1,0;",6);
                writeTX("ss     CODIGO     ;",19);

                //Introducir el Codigo
                if(CheckKeyboard(4))
                {
                    writeTX("sd0,0;",6);
                    writeTX("ss   INTRODUCIR   ;",19);
                    writeTX("sd1,0;",6);
                    writeTX("ss     ACCION     ;",19);
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

            else
            {
                DISPONIBLE = 0;
                CheckArea();
                //Desplegar Por UART El area
                /*Proceso*/
            }
        }
    }
    return 0;
}