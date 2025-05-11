
include P18F4550.inc  
;Tercera seccion de variables y punteros.

aux1 equ 0x0 ;sin signo enteras de 8 bits, iguales 0h y 0x0;
aux2 equ 1h
aux3 equ 2h
aux4 equ 3h
Inicio
;1###########################################################
   movlw .7   ;w <-7
   movwf aux1 ;aux1 <-w
   addlw .3 ;w <- 7+3
   movwf aux1 ;aux1 <-10
;2#############################################################
   movlw .8 ; w<-8
   movwf aux1 ; aux1 <-w
   movlw .10; w<-10
   movwf aux2; aux2 <-w
   
   movf aux1,w; w<-aux1
   addwf aux2,w ;w<-w+aux2
   movwf aux3 ; aux3 <-w
;3################################################################
   movlw .5; w-<5
   movwf aux1; aux1 <-w
   sublw .9 ; w<- (9-w)
   movwf aux2; aux2<-w
;4#############################################################
   movlw .6 ; w<-6
   movwf aux1 ; aux1 <-w
   movlw .4; w<-4
   movwf aux2; aux2 <-w
   movf aux2,w; w<-aux2
   subwf aux1,w ;w<- (aux1-w);
   movwf aux3 ; aux3 <-w
;5############################################################
  movlw .5 ;w<-5
  movwf aux1 ;aux1 <-w
  movf aux1,w ;w<-aux1
  mullw 4 ; PRODH_PRODL<-(w*4)
  movf PRODL,w ; w<-PRODL
  movwf aux3 ; aux3<-w
;6############################################################
  movlw .12 ;w <- 5
  movwf aux1;aux1 <-w
  movlw .15
  movwf aux2;aux2 <-w
  movf aux1,w ;w<-aux1
  mulwf aux2 ; PRODH_PRODL <- (w*aux2)
  movf PRODL,w ; w<-PRODL
  movwf aux3 ;aux3 <-w
;7##############################################################
  movlw b'00001100' ;w <- 12
  movwf aux1 ; aux1 <-w
  comf aux1,w; w<- Complemeto a 1 de aux1
  movwf aux2; aux2 <- w
;8############################################################
  movlw b'00001100' ;w <- 12
  movwf aux1 ; aux1 <-w
  negf aux1; w<- Complemeto a 2 de aux1
;9###########################################################
  movlw .35 ;w <- 35
  movwf aux1 ; aux1 <-w
  iorlw .7 ;w<- 7 or w 
  movwf aux2; aux2 <-w
;10###########################################################
  movlw .20 ;w <- 20
  movwf aux1 ; aux1 <-w
  movlw .56 ;w <- 56
  movwf aux2; aux2 <- w
  movf aux1,w ; aux1 <-w
  iorwf aux2,w ;w<- w or aux2  
  movwf aux3; aux3 <-w   
;11###########################################################
  movlw .62 ;w <- 62
  movwf aux1 ; aux1 <-w
  andlw	.15 ;w<- 15 y w 
  movwf aux2; aux2 <-w
;12###########################################################
  movlw .100 ;w <- 100
  movwf aux1 ; aux1 <-w
  movlw .45 ;w <- 45
  movwf aux2; aux2 <- w
  movf aux1,w ; aux1 <-w
  andwf aux2,w ;w<- w y aux2  
  movwf aux3; aux3 <-w
;13###########################################################
  movlw .120 ;w <- 120
  movwf aux1 ; aux1 <-w
  xorlw	.1 ;w<- 1 xor w 
  movwf aux2; aux2 <-w
;14###########################################################
  movlw .17 ;w <- 17
  movwf aux1 ; aux1 <-w
  movlw .90 ;w <- 90
  movwf aux2; aux2 <- w
  movf aux1,w ; aux1 <-w
  xorwf aux2,w ;w<- w xor aux2  
  movwf aux3; aux3 <-w
;15##########################################################
  movlw .25 ;w <- 25
  movwf aux1 ; aux1 <-w
  movlw .40 ;w <- 40
  movwf aux2; aux2 <- w
  movlw .103 ;w <- 103
  movwf aux3; aux3 <- w
  movf aux1,w; w<-aux1
  iorwf aux2; aux2 <- w O aux1
  movf aux3,w ;w<-aux3
  xorlw 0xD0 ;w<- w xor 0xd0
  andwf aux2,w ;w<-w y aux2
  movwf aux4; aux4<-w
;16#######################################################
  movlw .18 ;w <- 18
  movwf aux1 ; aux1 <-w
  movlw .60 ;w <- 60
  movwf aux2; aux2 <- w
  movlw .16 ;w <- 16
  movwf aux3; aux3 <- w
  movf aux1,w; w<-aux1
  addwf aux2; aux2 <- w+aux1
  movf aux3,w ;w<-aux3
  sublw b'11010' ;w<- 11010-w
  negf WREG; w<- -w
  mullw .3; PRODH_PRODL <- w*3
  movf PRODL,w; w <- RRODL
  subwf aux2,w ;w <- aux2-w
  movwf aux4; aux4 <- w
goto Inicio
end


