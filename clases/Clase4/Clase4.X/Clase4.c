//Primera seccion
#include <xc.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//Segunda seccion
#pragma config FOSC =INTOSC_EC
#pragma config WDT=OFF
//Tercera seccion
//char b=10;//8 bits
//int c  ;//16bits
//long d ;//32bits
//bit a ;//1 bit
//float e;
//double f = -0.15;//IEEE24
//O todo float o todo diuble 24bits
//char A[4]={4,5,6,7};
//int B[2][3];
//char *g;

void main(void){
    TRISD0=0;//bcf TRISD,0
             //TRISD=0b11110000;
    LATD=0;
    while(1){
        LATD0=1;
        LATD0=0;
    }          
}
