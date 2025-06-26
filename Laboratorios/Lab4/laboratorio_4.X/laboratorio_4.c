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

// Variables globales
int estado = EST_BIENVENIDA;
unsigned int objetivo = 0;
unsigned int contador = 0;
unsigned int input = 0;
volatile unsigned char Tecla = 0;
unsigned char btnRC0_ant = 0;
unsigned int inactividad = 0;
unsigned char lcd_backlight = 1;

const char mapa_teclas[17] = {
    0, '1', '2', '3', 'A',
       '4', '5', '6', 'B',
       '7', '8', '9', 'C',
       '*', '0', '#', 'D'
};

const unsigned char runner0[8] = {
    0b00100, 0b00100, 0b01110, 0b00100,
    0b01100, 0b10100, 0b00100, 0b01010
};
const unsigned char runner1[8] = {
    0b00100, 0b00100, 0b01110, 0b00100,
    0b01000, 0b00110, 0b01000, 0b10000
};
const unsigned char runner2[8] = {
    0b00100, 0b00100, 0b01110, 0b00100,
    0b00100, 0b01100, 0b01000, 0b00100
};
const unsigned char explosion[8] = {
    0b00100, 0b10101, 0b01110, 0b11111,
    0b01110, 0b10101, 0b00100, 0b00000
};

// Prototipos
void unidad(int i);
void color(int i);
void __interrupt() ISR(void);
void mostrar_conteo(void);
void lcd_animacion_bienvenida(void);
void mostrar_valor(void);
void emergencia(void);
void mostrar_completado(void);
void inicializar_hardware(void);

void main(void) {
    Lcd_Init();
    __delay_ms(1000);
    lcd_animacion_bienvenida();

    inicializar_hardware();

    estado = EST_INGRESO_OBJETIVO;
    mostrar_valor();

    while (1) {
        unsigned char btnRC0 = RC0;
        if (btnRC0 == 1 && btnRC0_ant == 0) {
            inactividad = 0;
            if (estado == EST_CONTEO) {
                if (contador < 59) contador++;
                if (contador >= objetivo) {
                    mostrar_conteo();
                    estado = EST_COMPLETADO;
                    mostrar_completado();
                } else {
                    mostrar_conteo();
                }
            }
        }
        btnRC0_ant = btnRC0;

        if (Tecla != 0) {
            inactividad = 0;
            char tecla = mapa_teclas[Tecla];
            Tecla = 0;

            if (tecla == 'D') {
                emergencia();
            }

            if (tecla == 'C') {
                inactividad = 0;
                lcd_backlight = !lcd_backlight;
                LATA5 = lcd_backlight;
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
                    if (tecla == 'B') {
                        contador = 0;
                        unidad(0);
                        LATE = 0b00000101;
                        estado = EST_COMPLETADO;
                        mostrar_completado();
                    } else if (tecla == 'A') {
                        contador = 0;
                        mostrar_conteo();
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

void inicializar_hardware(void) {
    TRISD = 0; LATD = 0;
    TRISE = 0; LATE = 0;
    TRISA = 0b00000101; 
    TRISB = 0b11110000; LATB = 0x0F;
    TRISC0 = 1; // RC0 como entrada
    RBPU = 0;
    __delay_ms(1000);

    T0CON = 0b00000001; // Timer0 con prescaler 1:4
    TMR0 = 3036;        // 100 ms aprox
    TMR0IF = 0;
    TMR0IE = 1;

    GIE = 1;
    PEIE = 1;
    RBIF = 0;
    RBIE = 1;
    TMR0ON = 1;

    
    lcd_backlight = 1;
}

void __interrupt() ISR(void) {
    if (RBIF == 1) {
        if (PORTB != 0b11110000) {
            Tecla = 0;
            LATB = 0b11111110;
            if (RB4 == 0) Tecla = 1;
            else if (RB5 == 0) Tecla = 2;
            else if (RB6 == 0) Tecla = 3;
            else if (RB7 == 0) Tecla = 4;
            else {
                LATB = 0b11111101;
                if (RB4 == 0) Tecla = 5;
                else if (RB5 == 0) Tecla = 6;
                else if (RB6 == 0) Tecla = 7;
                else if (RB7 == 0) Tecla = 8;
                else {
                    LATB = 0b11111011;
                    if (RB4 == 0) Tecla = 9;
                    else if (RB5 == 0) Tecla = 10;
                    else if (RB6 == 0) Tecla = 11;
                    else if (RB7 == 0) Tecla = 12;
                    else {
                        LATB = 0b11110111;
                        if (RB4 == 0) Tecla = 13;
                        else if (RB5 == 0) Tecla = 14;
                        else if (RB6 == 0) Tecla = 15;
                        else if (RB7 == 0) Tecla = 16;
                    }
                }
            }
            LATB = 0b11110000;
        }
        __delay_ms(100);
        RBIF = 0;
        inactividad = 0;
    }

    if (TMR0IF) {
        TMR0 = 3036;
        TMR0IF = 0;
        LATA1 ^= 1;
        inactividad++;

        if (inactividad == 10) {
            lcd_backlight = 0;
            LATA5 = 0;
        }

        if (inactividad >= 20) { 
            Lcd_Clear();
            LATE = 0;
            LATD = 0;
            LATA5 = 0;

            TMR0IE = 0;
            RBIE = 1;
            RBIF = 0;

            Sleep();

            // Al despertar
            inactividad = 0;
            TMR0IE = 1;
            lcd_backlight = 1;
            LATA5 = 1;
            
            if (estado==EST_INGRESO_OBJETIVO){
                mostrar_valor();
            }else if (estado == EST_CONTEO ){
                mostrar_conteo();
            }else if(estado==EST_COMPLETADO) {
                mostrar_completado();
            }
        }
    }
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
    LATD = (LATD & 0xF0) | (i & 0x0F);
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

void mostrar_valor(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Ingrese valor:");
}

void mostrar_completado(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Conteo Completo!");
    Lcd_Set_Cursor(2, 1);
    Lcd_String("Presione OK");
}

void emergencia(void) {
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

void lcd_animacion_bienvenida(void) {
    LCD_CrearCaracter(0, runner0);
    LCD_CrearCaracter(1, runner1);
    LCD_CrearCaracter(2, runner2);
    LCD_CrearCaracter(3, explosion);

    const char mensaje[] = "  Contador De Objetos  ";
    const int pasos = 16;

    for (int offset = 0; offset < pasos; offset++) {
        Lcd_Set_Cursor(1, 1);
        for (int j = 0; j < 16; j++) {
            char c = mensaje[offset + j];
            Lcd_Char((c >= 32 && c <= 126) ? c : ' ');
        }

        Lcd_Set_Cursor(2, offset + 1);
        Lcd_Char(offset % 3);
        __delay_ms(312);
        Lcd_Set_Cursor(2, offset + 1);
        Lcd_Char(' ');
    }

    Lcd_Set_Cursor(2, 16);
    Lcd_Char(3);
    Lcd_Set_Cursor(1, 1);
    Lcd_String("     ¡BOOM!         ");
    __delay_ms(500);

    for (int i = 0; i < 3; i++) {
        Lcd_Set_Cursor(2, 16);
        Lcd_Char(' ');
        __delay_ms(150);
        Lcd_Set_Cursor(2, 16);
        Lcd_Char(3);
        __delay_ms(150);
    }

    __delay_ms(800);
    Lcd_Clear();
} 