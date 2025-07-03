include P18F4550.inc

;=== CONFIGURACIÓN DE FUSES ===
CONFIG FOSC = HS
CONFIG WDT = OFF
CONFIG LVP = OFF
CONFIG PBADEN = OFF  ; Desactiva analógicos en PORTB<4:0>

;=== VARIABLES ===
cblock  0x20    
    ColorIndex    ; Índice de color RGB
    First         ; Bandera para primer color
endc

;=== VECTOR DE RESET Y DE INTERRUPCIONES ===
ORG 0x00
    goto Inicio
ORG 0x08
    goto ISRH     ; Interrupciones de alta prioridad
ORG 0x18
    goto ISRL     ; Interrupciones de baja prioridad

;=== INICIO DEL PROGRAMA ===
Inicio
    ; Habilitar interrupciones globales y por prioridad
    bsf RCON, IPEN      ; Habilita prioridades
    bsf INTCON, GIE     ; Globales
    bsf INTCON, GIEH    ; Alta prioridad
    bsf INTCON, GIEL    ; Baja prioridad

    ; === DESACTIVAR ENTRADAS ANALÓGICAS ===
    movlw   0x0F
    movwf   ADCON1      ; Todos los ANx como digitales
    clrf    ADCON0      ; Apaga el módulo ADC

    ; === CONFIGURACIÓN DE TMR0 ===
    movlw b'00000011'   ; Prescaler 1:16, modo 16 bits
    movwf T0CON  
    movlw 0x0B
    movwf TMR0H
    movlw 0xDC
    movwf TMR0L          ; Precarga 3036 ? ~1s con 48 MHz
    bcf INTCON, TMR0IF
    bsf INTCON, TMR0IE
    bcf INTCON2, TMR0IP  ; Baja prioridad
    bsf T0CON, TMR0ON    ; Encender Timer0

    ; === PUERTO D: DISPLAY 7448 ===
    movlw b'11110000'    ; RD0-RD3 salidas
    movwf TRISD
    clrf LATD

    ; === PUERTO E: LED RGB (RC0=R, RC1=G, RC2=B) ===
    movlw b'11111000'
    movwf TRISE
    clrf LATE
    clrf ColorIndex

    ; === PUERTO A: LED 1Hz (RA1 entrada, otros salidas) ===
    movlw b'11111101'
    movwf TRISA
    clrf LATA 

    ; === INTERRUPCIONES EXTERNAS INT0, INT1, INT2 ===
    bsf INTCON, INT0IE
    bsf INTCON2, INTEDG0
    bsf INTCON3, INT1IE
    bsf INTCON3, INT1IP    ; Alta prioridad
    bsf INTCON2, INTEDG1
    bcf INTCON, INT0IF
    bcf INTCON3, INT1IF
    bsf INTCON3, INT2IE
    bsf INTCON2, INTEDG2
    bcf INTCON3, INT2IF
    bcf INTCON3, INT2IP    ; Baja prioridad

    ; === COLOR INICIAL ===
    clrf First

Main
    goto Main

; === ISR BAJA PRIORIDAD ===
ISRL
    btfsc INTCON3, INT2IF
    goto Reiniciar

    btfsc INTCON, TMR0IF
    goto LED1HZ

    retfie

Reiniciar
    clrf LATD 
    clrf ColorIndex
    movlw b'00000101'      ; Magenta: Rojo + Azul
    movwf LATE
    bcf INTCON3, INT2IF
    retfie

LED1HZ
    bcf INTCON, TMR0IF
    movlw 0x0B
    movwf TMR0H
    movlw 0xDC
    movwf TMR0L
    btg LATA, 1
    retfie

; === ISR ALTA PRIORIDAD ===
ISRH
    btfsc INTCON, INT0IF
    goto INC

    btfsc INTCON3, INT1IF
    goto STOP

    retfie

; === LÓGICA DE CONTADOR ===
INC
    bcf INTCON, INT0IF
    btfss First,0
    call MAGENTA
    bsf First,0
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
    goto AZUL
    movf ColorIndex, W
    xorlw d'2'
    btfsc STATUS, Z
    goto CYAN
    movf ColorIndex, W
    xorlw d'3'
    btfsc STATUS, Z
    goto VERDE
    movf ColorIndex, W
    xorlw d'4'
    btfsc STATUS, Z
    goto AMARILLO
    movf ColorIndex, W
    xorlw d'5'
    btfsc STATUS, Z
    goto BLANCO
    clrf ColorIndex
    goto MAGENTA

; === COLORES RGB EN LATE ===
MAGENTA
    movlw b'00000101' ; R + B
    movwf LATE
    retfie

AZUL
    movlw b'00000100'
    movwf LATE
    retfie

CYAN
    movlw b'00000110' ; G + B
    movwf LATE
    retfie

VERDE
    movlw b'00000010'
    movwf LATE
    retfie

AMARILLO
    movlw b'00000011' ; R + G
    movwf LATE
    retfie

BLANCO
    movlw b'00000111' ; R + G + B
    movwf LATE
    retfie

; === BUCLE INFINITO AL DETENER ===
STOP
    movlw b'11111001'
    movwf LATE
    goto STOP

end
