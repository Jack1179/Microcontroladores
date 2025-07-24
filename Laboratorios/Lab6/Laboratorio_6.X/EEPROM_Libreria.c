#include <xc.h>

void EEPROM_Guardar(int dir, char data){
    EEADR = dir;
    EEDATA = data;
    EEPGD = 0; //Acceder a la memoria EEPROM
    CFGS = 0;  // =
    WREN = 1;
    GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    WR = 1;   //Inicializa la escritura
    GIE = 1;
    while(!EEIF);
    EEIF = 0;
    WREN = 0;  //Apagando la escritura
}

unsigned char EEPROM_Lectura(int dir){
    EEADR = dir;
    EEPGD = 0; //Acceder a la memoria EEPROM
    CFGS = 0;  // =
    RD = 1;   //Inicializa la lectura
    return EEDATA;
}
