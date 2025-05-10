;Comentarios se realizan con 
;Primera seccion librerias
include P18F4550.inc 
   
;Segunda seccion configuracion de salidas entradas

CONFIG FOSC=INTOSC_EC
CONFIG FOSC=EC_EC
CONFIG FOSC=XT_XT ;cristal es de 3.999Mhz o menos
CONFIG FOSC =HS ;cristal es 4Mhz o mayoR
;ACTIVAR SOLO UNO CONFIG DE RELOJ

CONFIG WDT=OFF
    
;Tercera seccion de variables y punteros.

aux1 equ 0x0 ;sin signo enteras de 8 bits, iguales 0h y 0x0;
aux2 equ 1h
aux3 equ 2h

;Cuarta seccion codigo principal codigo;
Inicio
   ;aux1 <-5
   movlw .5   ;w <-5  .5 sistema numerico decimal.
   movwf aux1 ;aux1 <-w
   ;aux2 <- 0b11001100
   movlw b'11001100'
   movwf aux2
   ;aux3 <-0xA3
   movlw 0xA3
   movwf aux3
   goto Inicio
end


