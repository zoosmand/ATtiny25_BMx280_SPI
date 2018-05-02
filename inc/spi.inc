;/*
; * spi.inc
; *
; *  Created: 03.05.2018 0:48:50
; *   Author: zoosman
; */ 


;/********************************* SPI Routines *********************************/
/* //////////////////////// SPI Master Transfer Buffer ///////////////////////// */
SPIM_TRANSFER_BUFFER:
  in tmp, USISR
  sbr tmp, (1<<USIOIF)
  cbr tmp, (0xf<<USICNT0)
  out USISR, tmp
  in tmp, USICR
  sbr tmp, (1<<USITC)
  out USIDR, txByte

  _SPIM_TRANSFER_BIT:
    out USICR, tmp
    sbis USISR, USIOIF  
    rjmp _SPIM_TRANSFER_BIT

  in rxByte, USIDR
  ret
