#include <xc.h>
#define _XTAL_FREQ 1000000   // Frecuencia del oscilador: 1 MHz

// CONFIGURACIÓN DE FUSIBLES
#pragma config FOSC = INTOSC_EC  // Oscilador interno, salida en RA6
#pragma config WDT = OFF         // Watchdog Timer deshabilitado

void main(void) {
    

    // CONFIGURACIÓN DEL PIN RC2 COMO SALIDA (PWM CCP1)
    TRISC2 = 0;   // RC2 como salida (PWM)

    // CONFIGURACIÓN DEL TIMER2 PARA PWM
    PR2 = 249;
    CCPR1L = 125; 
    
    T2CON=0b00000000;   
                             
    CCP1CON = 0b00001100;      
    TMR2 = 0;         
    TMR2ON=1;
    while (1) {
      
        if (CCPR1L > 248) {
            CCPR1L = 0;  
        }
        CCPR1L = CCPR1L+1;
        __delay_ms(100);
        
    }
}
