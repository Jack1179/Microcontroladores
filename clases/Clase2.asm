Comentarios se realizan con 
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


;Cuarta seccion codigo principal codigo;
Inicio

;bcf bit clear
;bsf bit ser
Menu
   bcf TRISD,0
   bsf LATD,0
   bcf LATD,0
   
goto Menu
   
   
  
end
