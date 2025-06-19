/* #include "kbd4x4.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 02/12/2019
* Electronica y Circuitos
*
*/

#include "kbd4x4.h"

void Keypad_Init(void)
{
    ROW0_DIR = 0;
    ROW1_DIR = 0;
    ROW2_DIR = 0;
    ROW3_DIR = 0;
    COL0_DIR = 1;
    COL1_DIR = 1;
    COL2_DIR = 1;
    COL3_DIR = 1;
}

char Keypad_Get_Char(void)
{
    ROW0_PIN = 0;
    ROW1_PIN = 1;
    ROW2_PIN = 1;
    ROW3_PIN = 1;
    if(COL0_PIN == 0){
        __delay_ms(2);
        while(COL0_PIN == 0);
        __delay_ms(5);
        return '1';
    }
    if(COL1_PIN == 0){
        __delay_ms(2);
        while(COL1_PIN == 0);
        __delay_ms(5);
        return '2';
    }
    if(COL2_PIN == 0){
        __delay_ms(2);
        while(COL2_PIN == 0);
        __delay_ms(5);
        return '3';
    }
    if(COL3_PIN == 0){
        __delay_ms(2);
        while(COL3_PIN == 0);
        __delay_ms(5);
        return 'A';
    }
    
    ROW0_PIN = 1;
    ROW1_PIN = 0;
    ROW2_PIN = 1;
    ROW3_PIN = 1;
    if(COL0_PIN == 0){
        __delay_ms(2);
        while(COL0_PIN == 0);
        __delay_ms(5);
        return '4';
    }
    if(COL1_PIN == 0){
        __delay_ms(2);
        while(COL1_PIN == 0);
        __delay_ms(5);
        return '5';
    }
    if(COL2_PIN == 0){
        __delay_ms(2);
        while(COL2_PIN == 0);
        __delay_ms(5);
        return '6';
    }
    if(COL3_PIN == 0){
        __delay_ms(2);
        while(COL3_PIN == 0);
        __delay_ms(5);
        return 'B';
    }
    
    ROW0_PIN = 1;
    ROW1_PIN = 1;
    ROW2_PIN = 0;
    ROW3_PIN = 1;
    if(COL0_PIN == 0){
        __delay_ms(2);
        while(COL0_PIN == 0);
        __delay_ms(5);
        return '7';
    }
    if(COL1_PIN == 0){
        __delay_ms(2);
        while(COL1_PIN == 0);
        __delay_ms(5);
        return '8';
    }
    if(COL2_PIN == 0){
        __delay_ms(2);
        while(COL2_PIN == 0);
        __delay_ms(5);
        return '9';
    }
    if(COL3_PIN == 0){
        __delay_ms(2);
        while(COL3_PIN == 0);
        __delay_ms(5);
        return 'C';
    }
    
    ROW0_PIN = 1;
    ROW1_PIN = 1;
    ROW2_PIN = 1;
    ROW3_PIN = 0;
    if(COL0_PIN == 0){
        __delay_ms(2);
        while(COL0_PIN == 0);
        __delay_ms(5);
        return '*';
    }
    if(COL1_PIN == 0){
        __delay_ms(2);
        while(COL1_PIN == 0);
        __delay_ms(5);
        return '0';
    }
    if(COL2_PIN == 0){
        __delay_ms(2);
        while(COL2_PIN == 0);
        __delay_ms(5);
        return '#';
    }
    if(COL3_PIN == 0){
        __delay_ms(2);
        while(COL3_PIN == 0);
        __delay_ms(5);
        return 'D';
    }
    return 0;
}