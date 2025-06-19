/* #include "kbd4x4.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 02/12/2019
* Electronica y Circuitos
*
*/

#include <xc.h>
#define _XTAL_FREQ 48000000

#define ROW0_PIN PORTDbits.RD7
#define ROW1_PIN PORTDbits.RD6
#define ROW2_PIN PORTDbits.RD5
#define ROW3_PIN PORTDbits.RD4
#define COL0_PIN PORTDbits.RD3
#define COL1_PIN PORTDbits.RD2
#define COL2_PIN PORTDbits.RD1
#define COL3_PIN PORTDbits.RD0

#define ROW0_DIR TRISDbits.TRISD7
#define ROW1_DIR TRISDbits.TRISD6
#define ROW2_DIR TRISDbits.TRISD5
#define ROW3_DIR TRISDbits.TRISD4
#define COL0_DIR TRISDbits.TRISD3
#define COL1_DIR TRISDbits.TRISD2
#define COL2_DIR TRISDbits.TRISD1
#define COL3_DIR TRISDbits.TRISD0

void Keypad_Init(void);
char Keypad_Get_Char(void);