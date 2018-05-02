;/*
; * init.inc
; *
; *  Created: 03.05.2018 0:37:58
; *   Author: zoosman
; */ 


;/********************** START AFTER POWER UP OR RESET FROM **********************/
_RESET:
  clr R1
  out SREG, R1
  ldi tmp, low(RAMEND)
  out SPL, tmp

;/*********************************** INIT GPIO **********************************/
INIT_LED:
  sbi LEDDDR, LED0PIN

;/******************** INIT EVENT AND MEASURE REGISTRY FLAGS  ********************/
INIT_FLAGS:
  clr _EREG_
  sbr _EREG_, (1<<_MIF_)

;/******************************** INIT VARIABLES ********************************/
INIT_VARIABLES:
  set_tcnt

;/************************************ Timer *************************************/
rcall INIT_TIMER

cli
;/******************************** Reduce Power **********************************/
INIT_POWERMODE:
  in tmp, PRR
  sbr tmp, (1<<PRTIM1)|(1<<PRADC)
  out PRR, tmp
  in tmp, ACSR
  sbr tmp, (1<<ACD)
  out ACSR, tmp

;/******************************* WatchDog Timer *********************************/
INIT_WATCHDOG:
  in tmp, WDTCR
  sbr tmp, (1<<WDCE)|(1<<WDE)
  out WDTCR, tmp
  sbr tmp, (1<<WDP3)|(1<<WDP0)|(1<<WDIE) ; 8s
  out WDTCR, tmp

;/********************************** USI SPI *************************************/
INIT_SPI_MASTER:
;  sbi SPIPORT, DI
;  sbi SPIPORT, SCK
  sbi SPIPORT, SS_1
  sbi SPIDDR, DO
  sbi SPIDDR, SCK
  sbi SPIDDR, SS_1
  ser tmp
  out USIDR, tmp
  clr tmp
  sbr tmp, (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)
  out USICR, tmp
  clr tmp
  sbr tmp, (1<<USIOIF)
  out USISR, tmp

sei

/********************************* BMx280 INIT ************************************/
rcall INIT_BMx280
