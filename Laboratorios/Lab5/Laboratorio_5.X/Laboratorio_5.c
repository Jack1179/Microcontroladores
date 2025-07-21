#include <xc.h>
#include "lcd.c"
#include "lcd.h"

#pragma config FOSC = INTOSC_EC 
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config BOR = OFF

#define _XTAL_FREQ 1000000

// Estados del programa
#define EST_BIENVENIDA 0
#define EST_INGRESO_OBJETIVO 1
#define EST_CONTEO 2
#define EST_COMPLETADO 3
#define EST_EMERGENCIA 4

// Variables globales
int estado = EST_BIENVENIDA;// variable de estado
unsigned int objetivo = 0;// Valor ingresado
unsigned int contador = 0;// Variable de contador
unsigned int input = 0;// Variable de ingreso
volatile unsigned char Tecla = 0;// tecla de 1 a 16
unsigned char btnRC0_ant = 0; // bit para evitar rebotes y matener el boton pulsado
unsigned int inactividad = 0;// contador para la inactividad
unsigned char lcd_backlight = 1;// señala en que estado esta la luz para poder cambiarla
unsigned char BufferR;
unsigned int VADC=0; 
unsigned int EMOTOR=0;
#define BUZZER LATA2

const char mapa_teclas[17] = {
    0, '1', '4', '7', '*',
       '2', '5', '8', '0',
       '3', '6', '9', '#',
       'A', 'B', 'C', 'D'
};// mapa de teclas 

const unsigned char runner0[8] = {
    0b00100, 
    0b00100, 
    0b01110, 
    0b00100,
    0b00100, 
    0b00100, 
    0b00100, 
    0b01010
};
const unsigned char runner1[8] = {
    0b00100, 
    0b00100, 
    0b01110, 
    0b00100,
    0b00100, 
    0b00110, 
    0b01001, 
    0b10000
};
const unsigned char runner2[8] = {
    0b00100, 
    0b00100, 
    0b01110, 
    0b00100,
    0b00100, 
    0b01100, 
    0b01000,
    0b00100
};
const unsigned char rpg[8] = {
    0b00000, 
    0b00000, 
    0b00110, 
    0b11111,
    0b00110, 
    0b00000, 
    0b00000, 
    0b00000
};
const unsigned char explosion[8] = {
    0b00100, 
    0b10101, 
    0b01110, 
    0b11111,
    0b01110, 
    0b10101, 
    0b00100, 
    0b00000
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
void Transmitir(unsigned char);
void Transmitir_Numero(unsigned char valor);
void Transmitir_Cadena(const char *cadena);
unsigned char Recibir(void);
unsigned char ConversionADC(unsigned char canal);
void mensaje_reset(void);
void beep_corto(void);
void beep_largo(void);

// codigo principal
void main(void) {
    inicializar_hardware();// inicializa los pines
    inactividad = 0;
    Lcd_Init();// inicializa el lcd
    beep_corto();
    __delay_ms(1000);// tiempo de proteccion
    mensaje_reset();
    inactividad = 0;
    lcd_animacion_bienvenida();// animacion de bienvenida
    estado = EST_INGRESO_OBJETIVO;// segundo estado
    mostrar_valor();//

    while (1) {
        unsigned char btnRC0 = RC0;
        if (btnRC0 == 1 && btnRC0_ant == 0) {
            inactividad = 0;
            if (estado == EST_CONTEO) {
                if (contador < 59) contador++;
                if(contador%10==0)beep_corto();
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
        
    if (EMOTOR != 0) {
    if (EMOTOR == 1) {
        LATC2 = 1;  // Prender motor
    } 
    else if (EMOTOR == 2) {
        LATC2 = 0;  // Apagar motor
    }
} else {
    if (VADC >= 127) {
        LATC2 = 1;  // Activar por ADC
    } else {
        LATC2 = 0;  // Desactivar por ADC
    }
}               
        if (Tecla != 0) {
            inactividad = 0;
            char tecla = mapa_teclas[Tecla];
            Tecla = 0;
        
            if (tecla == 'D') emergencia();
            if (tecla == 'C') {
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
                        estado = EST_COMPLETADO;
                        mostrar_completado();
                    } else if (tecla == 'A') {
                        contador = 0;
                        mostrar_conteo();
                    }
                    break;

                case EST_COMPLETADO:
                    beep_largo();
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
// configuracion de pines
void inicializar_hardware(void) {
    TRISD = 0; LATD = 0; //Salidas de 7 segmentos y LCD
    TRISE = 0; LATE = 0; //Salidas de Led RGB
    TRISC2=0; LATC2=0;
    TRISC0=1;

    TRISA = 0b00000001;// Salidas de E y R LCD y led 1hz
    TRISB = 0b11110000;// Salidas y entras de teclado matricial
    LATB = 0b00001111;//Pones de RB3 a RB0 en 1
    RBPU = 0;// Activa resistencias de pull-up de RB7 a RB4

    INT2IE = 0; // Desactiva interrucion externa 2
    INT2IF = 0;// Pone la flag de interrucion externa 2 en 0
    
    // Configuracion de comunicaciones
    
    TXSTA=0b00100100;
    RCSTA=0b10010000;        
    BAUDCON=0b00001000;        
    SPBRG=25;   
    
    RCIE=1;
    TXIE=0;
    RCIF=0;
    TXIF=0;     
    
    //Configuracion ADC
    
    ADCON0 = 0b00000001; // Desavtiva funciones analogicas del puerto A
    ADCON1 = 0b00001110; 
    ADCON2=  0b00001000;
            
            
    __delay_ms(1000); // delay de protecion

 // Configuracion de interuccion de tiempo0 para 
    TRISC0 = 1;
    T0CON = 0b00000001;
    TMR0 = 3036;
    TMR0IF = 0;
    TMR0IE = 1;
// Configuraciones de interrupciones
    GIE = 1;
    PEIE = 1;
    RBIF = 0;
    RBIE = 1;
    TMR0ON = 1;
    lcd_backlight = 1;
}
// interrupcion de teclado y de temporizador de 1hz
void __interrupt() ISR(void) {
    if (RBIF == 1) {
        
        if (PORTB != 0b11110000) {
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
        __delay_ms(350);
        RBIF = 0;
        if (inactividad > 10 & inactividad <20) {
            lcd_backlight = 1;
            LATA5 = 1;
        }
        inactividad = 0;
    }

    if (TMR0IF) {
        TMR0 = 3036;
        TMR0IF = 0;
        LATA1 ^= 1;
        inactividad++;
// Control ADC y motor:
        VADC=ConversionADC(0);
        Transmitir_Cadena("Valor del ADC: ");
        Transmitir_Numero(VADC);
// inactividad por 10s apaga la luz
        if (inactividad == 10) {
            lcd_backlight = 0;
            LATA5 = 0;
        }
// inactividad por 20s pone el pic en sleep
        if (inactividad >= 20) {
            Lcd_Clear();
            LATE = 0;
            LATD = 0;
            LATA5 = 0;
            LATC2=0;

            TMR0IE = 1;
            RBIE = 1;
            RBIF = 0;

            Sleep();

            inactividad = 0;
            color(contador / 10);
            unidad(contador % 10);
            TMR0IE = 1;
            lcd_backlight = 1;
            LATA5 = 1;
// imprime el estado que estaba la pantalla
            if (estado == EST_INGRESO_OBJETIVO) mostrar_valor();
            else if (estado == EST_CONTEO) mostrar_conteo();
            else if (estado == EST_COMPLETADO) mostrar_completado();
        }
    }
    
    if (RCIF==1) {
    inactividad = 0;
    if (OERR) {
        CREN = 0;
        CREN = 1;
    }

    BufferR = RCREG; // Leer solo una vez y almacenar
    //Transmitir(BufferR); // Eco 
    if (estado != EST_EMERGENCIA){
        if (BufferR == 'P' || BufferR == 'p'){
            Transmitir_Cadena("Parada de emergencia \r\n");
            emergencia();
        }
        else if (BufferR == 'E' || BufferR == 'e'){
            Transmitir_Cadena("Encender Motor \r\n");
            EMOTOR = 1;
        }
        else if (BufferR == 'A' || BufferR == 'a'){
            Transmitir_Cadena("Apagar motor \r\n");
            EMOTOR = 2;
        }
        else if ((BufferR == 'R' || BufferR == 'r') && estado == EST_CONTEO){
            Transmitir_Cadena("Reset de conteo \r\n ");
            contador = 0;
            mostrar_conteo();
        }
    }
   
}

}
// muentra el objetivo y el restante en el LCD
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
//Coloca las unidades en el 7 segmentos
void unidad(int i) {
    LATD = (LATD & 0xF0) | (i & 0x0F);
}
// Coloca las decenas en el 7 segmentos
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
// Muentra pantalla lcd el valor a escribir
void mostrar_valor(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Ingrese valor:");
}
// dibuja en el lcd indicando conteo completo
void mostrar_completado(void) {
    unidad(objetivo%10);
    color(objetivo/10);
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Conteo Completo!");
    Lcd_Set_Cursor(2, 1);
    Lcd_String("Presione OK");
}
// Pone en estado de emergencia con el boton de parada bibuja mensaje en el lcd y led en rojo
void emergencia(void) {
    estado = EST_EMERGENCIA;
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("! EMERGENCIA !");
    Lcd_Set_Cursor(2, 1);
    Lcd_String("Reset manual");
    LATE = 0b00000001;
    LATC2 = 0; 
    TMR0ON = 0;
    while (1);
}
// animacion de bienbenida
void lcd_animacion_bienvenida(void) {
    LCD_CrearCaracter(0, runner0);
    LCD_CrearCaracter(1, runner1);
    LCD_CrearCaracter(2, runner2);
    LCD_CrearCaracter(3, explosion);
    LCD_CrearCaracter(4, rpg);

    const char mensaje[] = "Bienvenido al Contador";
    const int pasos = 16;

    for (int offset = 0; offset < pasos; offset++) {
        Lcd_Set_Cursor(1, 1);
        for (int j = 0; j < 16; j++) {
            char c = mensaje[offset + j];
            Lcd_Char((c >= 32 && c <= 126) ? c : ' ');
        }
        Lcd_Set_Cursor(2, offset);
        Lcd_Char(4);
        Lcd_Set_Cursor(2, offset + 1);
        Lcd_Char(offset % 3);
        __delay_ms(312);
        Lcd_Set_Cursor(2, offset);
        Lcd_Char(' ');
        Lcd_Set_Cursor(2, offset + 1);
        Lcd_Char(' ');
    }

    Lcd_Set_Cursor(2, 16);
    Lcd_Char(3);
    Lcd_Set_Cursor(1, 1);
    Lcd_String("      !BOOM!         ");
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
    inactividad = 0;
}
unsigned char Recibir(void){
    return BufferR; 
}

void Transmitir(unsigned char BufferT){
    while (!TXIF); 
    TXREG = BufferT;
}
void Transmitir_Cadena(const char *cadena) {
    while (*cadena) {
        Transmitir(*cadena);
        cadena++;
    }
}
void Transmitir_Numero(unsigned char valor) {
    Transmitir((valor / 100) + '0');
    Transmitir(((valor / 10) % 10) + '0');
    Transmitir((valor % 10) + '0');
    Transmitir('\r');
    Transmitir('\n');
}
unsigned char ConversionADC(unsigned char canal){
    if (canal>12)
        return 0;
    ADCON0=(canal<<2)|0b00000001; // Selección del canal y encendido del módulo ADC
    GO = 1;                  // Inicia la conversión
    while(GO);              // Espera a que termine
    return ADRESH;               // Retorna solo la parte alta
}
void mensaje_reset(void) {
    if (POR==0) {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_String("Falla de energia");
        POR=1;
        __delay_ms(1500);
    } else{
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_String("Reset de usuario");
        __delay_ms(1500);
    }   
}

void beep_corto(void) {
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
}

void beep_largo(void) {
    BUZZER = 1;
    __delay_ms(500);
    BUZZER = 0;
}