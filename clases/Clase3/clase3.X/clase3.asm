include P18F4550.inc
CONFIG FOSC=INTOSC_EC
CONFIG WDT=OFF
CONFIG PBADEN=OFF
CONFIG LVP=OFF
aux1 equ 0h
aux2 equ 1h
aux3 equ 3h

ORG 0h ; a donde debe ir si el micro de reinicia
  goto Inicio
ORG 8h
  goto ISR; si hay una interrupcion a donde debe ir

Inicio ;RB7 Rojo RB6 Amarillo  RB5 Verde
  movlw b'11110000' ; habilitar puerto completo
  movwf TRISB;x2
  bcf INTCON2,INTEDG0; ; flanco de subida o bajada
  bcf INTCON,INT0IF;
  bsf INTCON,INT0IE;
  bsf INTCON,GTE; habilitacion global de interrupciones
  clrf LATB
Menu

Verde
  movlw b'00100000'
  movwf LATB
  goto Verde
 
Retardo
	movlw .254
	movwf aux1
	movlw .155
	movwf aux2
	movlw .2
	movwf aux3
AuxRetardo
	decfsz aux1,f
	goto AuxRetardo
	movlw .254
	movwf aux1
	decfsz aux2,f
	goto AuxRetardo
	movlw .155
	movwf aux2
	decfsz aux3,f
	goto AuxRetardo
	return
ISR  
   
Amarillo
  movlw b'01000000'
  movwf LATB
  call Retardo
Rojo
  movlw b'10000000'
  movwf LATB
  call Retardo
  call Retardo
RojoAmarillo
  movlw b'11000000'
  movwf LATB
  call Retardo
bcf INTCON,INT0IF
    retfie 
end