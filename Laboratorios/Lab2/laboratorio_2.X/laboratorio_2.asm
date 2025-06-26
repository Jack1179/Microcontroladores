include P18F4550.inc 

config FOSC = EC_EC
CONFIG WDT=OFF
CONFIG LVP=OFF

Inicio
    
movlw b'11111000'
movwf TRISE
setf LATE
    
Main
    
NEGRO	
movlw b'00000000'
movwf LATE
AZUL
movlw b'10000100'
movwf LATE       
CYAN
movlw b'00000110'
movwf LATE    
VERDE
movlw b'00000010'
movwf LATE
AMARILLO
movlw b'00000011'
movwf LATE       
BLANCO
movlw b'00000111'
movwf LATE
MAGENTA
movlw b'00000101'
movwf LATE
ROJO
movlw b'00000001'
movwf LATE
    
goto Main
  
end
    
    
