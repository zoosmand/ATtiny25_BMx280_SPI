;/*
; * macroses.inc
; *
; *  Created: 03.05.2018 0:36:09
; *   Author: zoosman
; */ 


;/******************************** Long Time Counter ******************************/
.MACRO set_tcnt
  ldi tcntH, 0x00 // 0x01 0x00
  ldi tcntL, 0x7f // 0xff 0x05
.ENDMACRO

;/*********************************** DELAY MACRO *********************************/
.MACRO DELAY_MACRO
  ldi tmp, @0
  mov dClock, tmp
  ldi tmp, @1
  mov dRate, tmp
  rcall DELAY
.ENDMACRO
