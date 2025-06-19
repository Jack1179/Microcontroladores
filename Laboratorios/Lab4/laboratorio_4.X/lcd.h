#ifndef LCD_H 
#define LCD_H

#include <xc.h>
#define _XTAL_FREQ 1000000  // Ajusta a tu frecuencia real

#define RS LATD0
#define EN LATD1
#define D4 LATD2
#define D5 LATD3
#define D6 LATD4
#define D7 LATD5

void Lcd_Init(void);
void Lcd_Cmd(char cmd);
void Lcd_Char(char data);
void Lcd_String(const char *str);
void Lcd_Set_Cursor(char row, char column);
void Lcd_Clear(void);
void Lcd_Shift_Left(void);
void Lcd_Shift_Right(void);

#endif
