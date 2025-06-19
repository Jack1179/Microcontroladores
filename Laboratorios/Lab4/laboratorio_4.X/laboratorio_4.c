#include <xc.h> 
#include "lcd.c"
#include "lcd.h"

#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 1000000

// Estados
#define EST_BIENVENIDA 0
#define EST_INGRESO_OBJETIVO 1
#define EST_CONTEO 2
#define EST_COMPLETADO 3
#define EST_EMERGENCIA 4


int estado = EST_BIENVENIDA;

unsigned int objetivo = 0;
unsigned int contador = 0;
unsigned int input = 0;

unsigned long inactividad = 0;
unsigned char lcd_backlight = 1;

const char teclas[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

char Teclado4x4(void);
void unidad(int i);
void color(int i);
void interrupt ISR(void);
void mostrar_conteo(void);
void lcd_animacion_bienvenida(void);
void mostrar_valor(void);

void main(void) {
    // LCD
    Lcd_Init();
    TRISD = 0x00; LATD = 0x00;
    TRISD6 = 0; // RD6 como salida para luz de fondo
    LATD6 = 1;

    // Teclado
    TRISB = 0b11110000; LATB = 0x00;
    RBPU = 0;

    // LED y display
    TRISC = 0x00; LATC = 0x00;
    TRISE = 0x00; LATE = 0x00;
    TRISC2 = 0; LATC2 = 0;

    // Timer0
    T0CON = 0b00000001; // 16 bits, prescaler 1:4
    TMR0 = 3036;
    TMR0IF = 0;
    TMR0IE = 1;
    TMR0ON = 1;

    IPEN = 0;
    GIE = 1;
    PEIE = 1;

    // Bienvenida
    lcd_animacion_bienvenida();
    estado = EST_INGRESO_OBJETIVO;

    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Ingrese valor:");

    while (1) {
        char tecla = Teclado4x4();

       

        if (tecla == 'D') {
            estado = EST_EMERGENCIA;
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_String("! EMERGENCIA !");
            Lcd_Set_Cursor(2, 1);
            Lcd_String("Reset manual");
            LATE = 0b00000001;
            TMR0ON = 0;
            
            while (1);
        }

        if (tecla != 0) {
            inactividad = 0; // reset tiempo inactivo

            if (tecla == 'C') { // tecla 16
                lcd_backlight = !lcd_backlight;
                LATD6 = lcd_backlight;
            }

            switch (estado) {
                case EST_INGRESO_OBJETIVO:
                    if (tecla >= '0' && tecla <= '9') {
                        input = input * 10 + (tecla - '0');
                        if (input > 99) input = 0;
                        mostrar_valor();
                        Lcd_Set_Cursor(2, 7);
                        Lcd_Char((input / 10) + '0');
                        Lcd_Char((input % 10) + '0');
                    } else if (tecla == '*') {
                        input = input / 10;
                        mostrar_valor();
                        Lcd_Set_Cursor(2, 7);
                        Lcd_Char((input / 10) + '0');
                        Lcd_Char((input % 10) + '0');
                    } else if (tecla == '#') {
                        if (input >= 1 && input <= 59) {
                            objetivo = input;
                            contador = 0;
                            input = 0;
                            estado = EST_CONTEO;
                            mostrar_conteo();
                        } else {
                            Lcd_Clear();
                            Lcd_Set_Cursor(1, 1);
                            Lcd_String("Rango invalido");
                            __delay_ms(1500);
                            input = 0;
                            mostrar_valor();
                        }
                    }
                    break;

                case EST_CONTEO:
                    if (tecla == 'A') {
                        if (contador < 59) contador++;
                        if (contador >= objetivo || contador >= 59) {
                            mostrar_conteo();
                            estado = EST_COMPLETADO;
                            Lcd_Clear();
                            Lcd_Set_Cursor(1, 1);
                            Lcd_String("Conteo Completo!");
                            Lcd_Set_Cursor(2, 1);
                            Lcd_String("Presione OK");
                        } else {
                            mostrar_conteo();
                        }
                    }  else if (tecla == 'B') {
                        estado = EST_INGRESO_OBJETIVO;
                        input = 0;
                        contador = 0;
                        unidad(0);
                        LATE = 0b00000101;
                        mostrar_valor();

                    }
                    break;

                case EST_COMPLETADO:
                    if (tecla == '#') {
                        estado = EST_INGRESO_OBJETIVO;
                        input = 0;
                        mostrar_valor();
                    }
                    break;
            }
            __delay_ms(200);
        }  
      }
    }


char Teclado4x4(void) {
    for (int fila = 0; fila < 4; fila++) {
        LATB = ~(1 << fila) & 0x0F;
        __delay_ms(5);
        for (int col = 0; col < 4; col++) {
            if (!(PORTB & (1 << (col + 4)))) {
                while (!(PORTB & (1 << (col + 4))));
                return teclas[fila][col];
            }
        }
    }
    return 0;
}

void mostrar_conteo(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Objetivo:");
    Lcd_Char((objetivo / 10) + '0');
    Lcd_Char((objetivo % 10) + '0');

    Lcd_Set_Cursor(2, 1);
    Lcd_String("Faltantes:");
    int faltan = objetivo - contador;
    if (faltan < 0) faltan = 0;
    Lcd_Char((faltan / 10) + '0');
    Lcd_Char((faltan % 10) + '0');

    unidad(contador % 10);
    color(contador / 10);
}

void unidad(int i) {
    LATC = (LATC & 0x0F) | (i << 4);
}

void color(int i) {
    switch (i) {
        case 0: LATE = 0b00000101; break;
        case 1: LATE = 0b00000100; break;
        case 2: LATE = 0b00000110; break;
        case 3: LATE = 0b00000010; break;
        case 4: LATE = 0b00000011; break;
        case 5: LATE = 0b00000111; break;
        default: LATE = 0x00; break;
    }
}

void interrupt ISR(void) {
    if (TMR0IF) {
        TMR0 = 3036;
        TMR0IF = 0;
        LATC2 ^= 1;
    }
}
void mostrar_valor(void) {
 Lcd_Clear();
 Lcd_Set_Cursor(1, 1);
 Lcd_String("Ingrese valor:");
                         }
void lcd_animacion_bienvenida(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String(" Contador De Objetos ");
    __delay_ms(300);
    for (int i = 0; i < 8; i++) {
        Lcd_Shift_Left();
        __delay_ms(500);
    }
    for (int i = 0; i < 8; i++) {
        Lcd_Shift_Right();
        __delay_ms(500);
    }
}