//Old Main
 #if(0)
int main()
{
    /*Configuracion de Columnas del Teclado*/
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;

    /*Configuracion de las Filas del Teclado*/
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 0; //Poner en 1
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;

    //PORTBbits.RB5 ^= 1;
    /*Leds Indicadores de Status*/
    TRISAbits.RA0 = 0;  //Activada
    TRISAbits.RA1 = 0;  //Disponible

    UartConfig();
    /*Uso del Puerto D Para Deteccion de las Areas*/
    TRISD = 0xFF;

    while(1)
    {
        PORTBbits.RB5  ^= 1;
        int go = 0;
        while(!go)
        {
            datax[0] = 's';
            datax[1] = 'c';
            datax[3] = ';';
            writeTX(3);
            if(PIR1bits.RCIF)
            {
                if(RCREG == 'E' || RCREG == 'e')
                    go = 0;
                else if(RCREG == 'O' || RCREG == 'o')
                    go = 1;
            }
        }
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
                ;
            }
            //Introducir Accion
            CheckKeyboard(1);
        }
    }
    return 0;
}
#endif
