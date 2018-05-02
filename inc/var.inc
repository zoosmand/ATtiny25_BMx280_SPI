;/*
; * var.inc
; *
; *  Created: 03.05.2018 0:58:45
; *   Author: zoosman
; */ 


;/******************************* VARIABLES & DATAS ******************************/

.dseg
.org 0x0060
  BMx280_calib1:	.byte 26
.org 0x0080
  BMx280_calib2:	.byte 16
.org 0x0090
  BMx280_data:		.byte 8
 
;.eseg
;  StartParam:	.db "Ready"