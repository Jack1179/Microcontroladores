include P18F4550.inc
CONFIG FOSC=HS
CONFIG WDT=OFF
CONFIG LVP=OFF
CONFIG PBADEN=OFF

cblock  0x20    
    ColorIndex    ; Solo usamos 1 byte
endc        ; bariable para secuencia de color RGB

ORG 0h
    goto Inicio
ORG 8h
    goto ISRH
ORG 18h
    goto ISRL

Inicio
    bsf INTCON, GIE	; Habilita interrupciones globales    
    bsf RCON, IPEN      ;habilita esquema de prioridades
    bsf INTCON, GIEH    ;prioridad alta
    bsf INTCON, GIEL    ;prioridad baja

    ;=== TMR0 configuración ===
    movlw b'00000011' ;escala 1:16
    movwf T0CON  
    movlw 0x0B
    movwf TMR0H
    movlw 0xDC
    movwf TMR0L  ;precarga
    bcf INTCON, TMR0IF
    bsf INTCON, TMR0IE
    bcf INTCON2, TMR0IP  

    ;=== SALIDAS LATD (bits 0-3 para display 0?9) ===
    movlw b'11110000'
    movwf TRISD
    clrf LATD

    ;=== SALIDAS RGB en LATC (bits 0?2) ===
    movlw b'11111000' ; RC0=R, RC1=G, RC2=B
    movwf TRISC
    clrf LATC          ; Color inicial: NEGRO
    clrf ColorIndex    ; Inicia índice de color en 0

   ;=== SALIDAS de led 1hz ===
    movlw b'01111111'
    movwf TRISB
    clrf LATB 

    ;=== INTERRUPCIONES EXTERNAS ===
    bsf INTCON, INT0IE
    bsf INTCON2, INTEDG0
    bsf INTCON3, INT1IE
    bsf INTCON3, INT1IP
    bsf INTCON2, INTEDG1
    bcf INTCON, INT0IF
    bcf INTCON3, INT1IF
    bsf INTCON3, INT2IE
    bcf INTCON3, INT2IF
    bsf INTCON2, INTEDG2
    bcf INTCON3, INT2IP

    bsf T0CON, TMR0ON

Main;ciclo infinito
    goto Main 

; === ISR de baja prioridad: TMR0, INT2 ===
ISRL
    btfsc INTCON3, INT2IF
    goto Reiniciar

    btfsc INTCON, TMR0IF
    goto LED1HZ

    retfie

Reiniciar
    clrf LATD             ; Reinicia el contador
    bcf INTCON3, INT2IF
    retfie

LED1HZ     ; cambia el led cada 1s
    bcf INTCON, TMR0IF
    movlw 0x0B
    movwf TMR0H
    movlw 0xDC
    movwf TMR0L
    btg LATB, 7
    retfie

; === ISR de alta prioridad: INT0 e INT1 ===
ISRH
    btfsc INTCON, INT0IF
    goto INC 

    btfsc INTCON3, INT1IF
    goto STOP

    retfie

; === Incrementa LATD hasta 9, luego reinicia y cambia color en LATC ===
INC
    bcf INTCON, INT0IF
    movf LATD, W
    xorlw 9
    btfsc STATUS, Z
    goto Cero
    incf LATD, F
    retfie

Cero
    clrf LATD
    incf ColorIndex, F
    movf ColorIndex, W
    xorlw d'1'
    btfsc STATUS, Z
    goto MAGENTA
    movf ColorIndex, W
    xorlw d'2'
    btfsc STATUS, Z
    goto AZUL
    movf ColorIndex, W
    xorlw d'3'
    btfsc STATUS, Z
    goto CYAN
    movf ColorIndex, W
    xorlw d'4'
    btfsc STATUS, Z
    goto VERDE
    movf ColorIndex, W
    xorlw d'5'
    btfsc STATUS, Z
    goto AMARILLO
    movf ColorIndex, W
    xorlw d'6'
    btfsc STATUS, Z
    goto BLANCO

    ; Si supera 6, reinicia el índice y vuelve a MAGENTA
    clrf ColorIndex
    goto MAGENTA

MAGENTA
    movlw b'00000101' ; RC0=1, RC2=1 (Rojo + Azul)
    movwf LATC
    retfie

AZUL
    movlw b'00000100'
    movwf LATC
    retfie

CYAN
    movlw b'00000110' ; Azul + Verde
    movwf LATC
    retfie

VERDE
    movlw b'00000010'
    movwf LATC
    retfie

AMARILLO
    movlw b'00000011' ; Rojo + Verde
    movwf LATC
    retfie

BLANCO
    movlw b'00000111' ; R + G + B
    movwf LATC
    retfie

STOP
    movlw b'11111001'; bucle infinito
    movwf LATC
    goto STOP  

end
