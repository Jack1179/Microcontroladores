#include "lcd.h"

void Lcd_Init() {
    TRISD = 0;
    LATD = 0;
    TRISA = 0b00000101; 
    LATA = 0;
    TRISA5 = 0; // Pin backlight
    LATA5 = 1;
    __delay_ms(20);
    Lcd_Cmd(0x02);  // 4 bits
    Lcd_Cmd(0x28);  // 5x7 matrix
    Lcd_Cmd(0x0C);  // LCD on , cursor OFF
    Lcd_Cmd(0x06);  // int
    Lcd_Cmd(0x01);  // clean LCD
    __delay_ms(2);
}

void Lcd_Cmd(char cmd) {
    RS = 0;
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1; __delay_ms(2); EN = 0;

    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1; __delay_ms(2); EN = 0;
}

void Lcd_Char(char data) {
    RS = 1;
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1; __delay_us(200); EN = 0;

    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1; __delay_us(200); EN = 0;
}

void Lcd_String(const char *str) {
    while (*str) {
        Lcd_Char(*str++);
    }
}

void Lcd_Set_Cursor(char row, char column) {
    char pos = (row == 1) ? 0x80 + (column - 1) : 0xC0 + (column - 1);
    Lcd_Cmd(pos);
}

void Lcd_Clear() {
    Lcd_Cmd(0x01);
    __delay_ms(2);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x18);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x1C);
}
void LCD_CrearCaracter(unsigned char pos, const unsigned char *pattern) {
    Lcd_Cmd(0x40 + (pos * 8));
    for (char i = 0; i < 8; i++) {
        Lcd_Char(pattern[i]);
    }
}