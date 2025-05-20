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
  goto ISRH; si hay una interrupcion a donde debe ir si es de alta prioridad
ORG 18h
  goto ISRL; salta si hay interrupcion de baja prioridad.

Inicio ;RB7 Rojo RB6 Amarillo  RB5 Verde
  movlw b'00011111' ; habilitar puerto completo
  movwf TRISB;x2
  bcf INTCON2,INTEDG0;  flanco de subida o bajada
  bcf INTCON2,INTEDG1


  bcf INTCON,INT0IF;flag
  bsf INTCON,INT0IE;enable
; interrupcion externa 0 no tiene bit de prioridad siempre sera de maxima prioridad
  bcf INTCON3,INT1IF;flag
  bsf INTCON3,INT1IE;enable

  bsf RCON,IPEN; esquema de prioridades
  bcf INTCON3,INT1IP; 1 Prioridad alta, 0 Prioridad baja
  bsf INTCON,PETE  
  bsf INTCON,GTE; habilitacion global de interrupciones
   
  bcf TRISD,0
  bcf LATD,0
  movlw b'00000001'; configuracion de interrupcion de tiempo
  movwf T0CON
  movlw 0xB
  movwf TMR0H
  movlw 0xDC
  movwf TMR0L
  bcf INTCON, TMR0IF
  bsf INTCON, TMR0IE
  bsf T0CON, TMR0ON
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
ISRH
btfsc INTCON,INT0IF 
goto ISR_INT0
btfsc INTCON3,INT1IF
goto ISR_INT1
retfie

bcf INTCON, INT0IF
btg LATD,0

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
bcf INTCON,TMR0IF
movlw 0xB
movwf TMR0H
movlw 0xDC
movwf TMR0L
btg LATD,0
retfie
end


