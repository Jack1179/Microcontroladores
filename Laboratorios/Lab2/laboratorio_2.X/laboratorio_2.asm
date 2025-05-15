include P18F4550.inc 

config FOSC = EC_EC
CONFIG WDT=OFF
CONFIG LVP=OFF

Inicio
    
movlw b'11111000'
movwf TRISD
setf LAT    D
    
Main
    
NEGRO	
movlw b'00000000'
movwf LATD
AZUL
movlw b'00000100'
movwf LATD       
CYAN
movlw b'00000110'
movwf LATD    
VERDE
movlw b'00000010'
movwf LATD
AMARILLOt
movlw b'00000011'
movwf LATD       
BLANCO
movlw b'00000111'
movwf LATD
MAGENTA
movlw b'00000101'
movwf LATD
ROJO
movlw b'00000001'
movwf LATD
    
goto	Main
  
end
    
    
