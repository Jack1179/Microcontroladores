//Primera seccion
#include <xc.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//Segunda seccion
#pragma config FOSC =INTOSC_EC
#pragma config WDT=OFF
#define _XTAL_FREQ 1000000

void main(void) {
    TRISB0 = 0b00011111; 
    IDLEN=0 //POR,BOR,MCLR,WDT
            //INT0,INT1,INT2,RB no requiere señal de reloj
    IDLEN=1//POR,BOR,MCLR,WDT
            //21 interruciones  
    SLEEP();
    LATD = 0;
    while (1) {
        LATD0 = 1;
        __delay_ms(1000);
        LATD0 = 0;
    }
}


