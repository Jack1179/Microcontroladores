#include <xc.c>

#pragma config FOSC =INTOSC_EC
#pragma config WDT=OFF
#define _XTAL_FREQ 1000000
void main(void){
    TRISD0=0;//bcf TRISD,0
             //TRISD=0b11110000;
    LATD=0;
    while(1){
        LATD0=1;
    __delay_ms(1000);    
        LATD0=0;
    }          
}