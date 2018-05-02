;/*
; * bmx280.inc
; *
; *  Created: 03.05.2018 0:51:40
; *   Author: zoosman
; */ 


;/******************************** BMx280 macroses *******************************/
.MACRO BMx280_READ_MACRO
  ldi YH, high(@0-1)
  ldi YL, low(@0-1)
  ldi txByte, @1
  ldi tmp, @2
  mov R0, tmp
  rcall BMx280_READ
.ENDMACRO

.MACRO BMx280_WRITE_MACRO
  ldi YH, high(@0)
  ldi YL, low(@0)
  ldi tmp, @1
  mov R0, tmp
  rcall BMx280_WRITE
.ENDMACRO



;/******************************** BMx280 routines *******************************/
INIT_BMx280:
  cbr _EREG_, (1<<_SPIERF_) ; clear SPI error flag
;  rcall BMx280_WRITE_F5
  BMx280_READ_MACRO BMx280_data, 0xd0, 0x02 ; Read BMx280 ID, BMP280 - 0x58, BME280 - 0x60
  ld tmp, -Y
  cpi tmp, 0x58
  brne _INIT_BMx280_ID_FAILED
  ; Datasheet 5.4 Register description
;  rcall BMx280_WRITE_F2_F4 ; f2 - ctrl_hum register, osrh_t 001 - oversampling x1, comment this string on BMP280
  rcall BMx280_WRITE_F4 ; f4 - ctrl_meas register, osrs_t 001 - oversampling x1, osrs_p 001 - oversampling x1, mode 01 - Forced Mode. Datasheet 3.6 Power modes, comment this string on BME280
  BMx280_READ_MACRO BMx280_calib1, 0x88, 0x1a ; Read calibration registers calib00-calib25
;  BMx280_READ_MACRO BMx280_calib2, 0xe1, 0x10 ; Read calibration registers calib26-calib41, comment this string on BMP280
  rjmp _INIT_BMx280_EXIT

  _INIT_BMx280_ID_FAILED:
    sbr _EREG_, (1<<_SPIERF_)
  _INIT_BMx280_EXIT:
    ret

BMx280_READ:
  sbrc _EREG_, _SPIERF_
  ret
  mov R0, tmp
  sbr txByte, (1<<7) ; set read byte
  cbi SPIPORT, SS_1 ; call slave
  _READ_DATA:
    rcall SPIM_TRANSFER_BUFFER 
	clr txByte  
    st Y+, rxByte 
    dec R0
    brne _READ_DATA
  sbi SPIPORT, SS_1 ; release slave
  ret

BMx280_WRITE:
  sbrc _EREG_, _SPIERF_
  ret
  mov R0, tmp
  cbi SPIPORT, SS_1 ; call slave
  _WRITE_DATA:
    ld txByte, Y+ 
    rcall SPIM_TRANSFER_BUFFER   
    dec R0
    brne _WRITE_DATA
  sbi SPIPORT, SS_1 ; release slave
  ret

BMx280_WRITE_F5:
  ldi YH, high(BMx280_data)
  ldi YL, low(BMx280_data)
  ldi tmp, 0xf5
  cbr tmp, (1<<7) ; set write byte
  st Y+, tmp
  ldi tmp, 0x01
  st Y+, tmp
  BMx280_WRITE_MACRO BMx280_data, 0x02
  ret

BMx280_WRITE_F4:
  ldi YH, high(BMx280_data)
  ldi YL, low(BMx280_data)
  ldi tmp, 0xf4
  cbr tmp, (1<<7) ; set write byte
  st Y+, tmp
  ldi tmp, 0x25
  st Y+, tmp
  BMx280_WRITE_MACRO BMx280_data, 0x02 ; f4 - ctrl_meas register, osrs_t 001 - oversampling x1, osrs_p 001 - oversampling x1, mode 01 - Forced Mode. Datasheet 3.6 Power modes
  ret

BMx280_WRITE_F2_F4:
  ldi YH, high(BMx280_data)
  ldi YL, low(BMx280_data)
  ldi tmp, 0xf2
  cbr tmp, (1<<7) ; set write byte
  st Y+, tmp
  ldi tmp, 0x01 ; f2 - ctrl_hum register, osrh_t 001 - oversampling x1
  st Y+, tmp
  ldi tmp, 0xf4
  cbr tmp, (1<<7) ; set write byte
  st Y+, tmp
  ldi tmp, 0x25 ; f4 - ctrl_meas register, osrs_t 001 - oversampling x1, osrs_p 001 - oversampling x1, mode 01 - Forced Mode. Datasheet 3.6 Power modes
  st Y+, tmp
  BMx280_WRITE_MACRO BMx280_data, 0x04
  ret
