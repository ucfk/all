;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.0.1 #6083 (Dec 17 2010) (MINGW32)
; This file was generated Wed Apr 08 20:23:30 2026
;--------------------------------------------------------
	.module 3
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _int1_isr
	.globl _int0_isr
	.globl _delay
	.globl _T2CON_7
	.globl _T2CON_6
	.globl _T2CON_5
	.globl _T2CON_4
	.globl _T2CON_3
	.globl _T2CON_2
	.globl _T2CON_1
	.globl _T2CON_0
	.globl _RI
	.globl _TI
	.globl _RB8
	.globl _TB8
	.globl _REN
	.globl _SM2
	.globl _SM1
	.globl _SM0
	.globl _P1_7
	.globl _P1_6
	.globl _P1_5
	.globl _P1_4
	.globl _P1_3
	.globl _P1_2
	.globl _P1_1
	.globl _P1_0
	.globl _RXD
	.globl _TXD
	.globl _INT0
	.globl _INT1
	.globl _T0
	.globl _T1
	.globl _WR
	.globl _RD
	.globl _PX0
	.globl _PT0
	.globl _PX1
	.globl _PT1
	.globl _PS
	.globl _EX0
	.globl _ET0
	.globl _EX1
	.globl _ET1
	.globl _ES
	.globl _EA
	.globl _IT0
	.globl _IE0
	.globl _IT1
	.globl _IE1
	.globl _TR0
	.globl _TF0
	.globl _TR1
	.globl _TF1
	.globl _P
	.globl _OV
	.globl _RS0
	.globl _RS1
	.globl _F0
	.globl _AC
	.globl _CY
	.globl _RCAP2L
	.globl _RCAP2H
	.globl _T2CON
	.globl _SBUF
	.globl _SCON
	.globl _IP
	.globl _IE
	.globl _AUXR
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _B
	.globl _ACC
	.globl _PSW
	.globl _P3
	.globl _P2
	.globl _P1
	.globl _P0
	.globl _count
	.globl _seg
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0	=	0x0080
_P1	=	0x0090
_P2	=	0x00a0
_P3	=	0x00b0
_PSW	=	0x00d0
_ACC	=	0x00e0
_B	=	0x00f0
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_AUXR	=	0x008e
_IE	=	0x00a8
_IP	=	0x00b8
_SCON	=	0x0098
_SBUF	=	0x0099
_T2CON	=	0x00c8
_RCAP2H	=	0x00cb
_RCAP2L	=	0x00ca
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_CY	=	0x00d7
_AC	=	0x00d6
_F0	=	0x00d5
_RS1	=	0x00d4
_RS0	=	0x00d3
_OV	=	0x00d2
_P	=	0x00d0
_TF1	=	0x008f
_TR1	=	0x008e
_TF0	=	0x008d
_TR0	=	0x008c
_IE1	=	0x008b
_IT1	=	0x008a
_IE0	=	0x0089
_IT0	=	0x0088
_EA	=	0x00af
_ES	=	0x00ac
_ET1	=	0x00ab
_EX1	=	0x00aa
_ET0	=	0x00a9
_EX0	=	0x00a8
_PS	=	0x00bc
_PT1	=	0x00bb
_PX1	=	0x00ba
_PT0	=	0x00b9
_PX0	=	0x00b8
_RD	=	0x00b7
_WR	=	0x00b6
_T1	=	0x00b5
_T0	=	0x00b4
_INT1	=	0x00b3
_INT0	=	0x00b2
_TXD	=	0x00b1
_RXD	=	0x00b0
_P1_0	=	0x0090
_P1_1	=	0x0091
_P1_2	=	0x0092
_P1_3	=	0x0093
_P1_4	=	0x0094
_P1_5	=	0x0095
_P1_6	=	0x0096
_P1_7	=	0x0097
_SM0	=	0x009f
_SM1	=	0x009e
_SM2	=	0x009d
_REN	=	0x009c
_TB8	=	0x009b
_RB8	=	0x009a
_TI	=	0x0099
_RI	=	0x0098
_T2CON_0	=	0x00c8
_T2CON_1	=	0x00c9
_T2CON_2	=	0x00ca
_T2CON_3	=	0x00cb
_T2CON_4	=	0x00cc
_T2CON_5	=	0x00cd
_T2CON_6	=	0x00ce
_T2CON_7	=	0x00cf
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_seg::
	.ds 10
_count::
	.ds 1
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG	(DATA)
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
	ljmp	_int0_isr
	.ds	5
	reti
	.ds	7
	ljmp	_int1_isr
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:2: ERROR: no line number 2 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	_seg,#0x3F
	mov	(_seg + 0x0001),#0x06
	mov	(_seg + 0x0002),#0x5B
	mov	(_seg + 0x0003),#0x4F
	mov	(_seg + 0x0004),#0x66
	mov	(_seg + 0x0005),#0x6D
	mov	(_seg + 0x0006),#0x7D
	mov	(_seg + 0x0007),#0x07
	mov	(_seg + 0x0008),#0x7F
	mov	(_seg + 0x0009),#0x6F
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:4: ERROR: no line number 4 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	_count,#0x1B
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	lcall	_main
;	return from main will lock up
	sjmp .
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'delay'
;------------------------------------------------------------
;i                         Allocated to registers r2 r3 
;j                         Allocated to registers r4 r5 
;------------------------------------------------------------
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:6: void delay() {
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:8: for(i=0;i<5;i++)
	mov	r2,#0x00
	mov	r3,#0x00
00104$:
	clr	c
	mov	a,r2
	subb	a,#0x05
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00108$
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:9: for(j=0;j<200;j++);
	mov	r4,#0xC8
	mov	r5,#0x00
00103$:
	dec	r4
	cjne	r4,#0xff,00117$
	dec	r5
00117$:
	mov	a,r4
	orl	a,r5
	jnz	00103$
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:8: for(i=0;i<5;i++)
	inc	r2
	cjne	r2,#0x00,00104$
	inc	r3
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'int0_isr'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:12: void int0_isr(void) interrupt 0 {
;	-----------------------------------------
;	 function int0_isr
;	-----------------------------------------
_int0_isr:
	push	acc
	push	psw
	mov	psw,#0x00
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:13: count++;
	inc	_count
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:14: if(count > 99) count = 0;
	mov	a,#0x63
	cjne	a,_count,00106$
00106$:
	jnc	00103$
	mov	_count,#0x00
00103$:
	pop	psw
	pop	acc
	reti
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'int1_isr'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:17: void int1_isr(void) interrupt 2 {
;	-----------------------------------------
;	 function int1_isr
;	-----------------------------------------
_int1_isr:
	push	acc
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:18: if(count == 0) count = 99;
	mov	a,_count
	jnz	00102$
	mov	_count,#0x63
	sjmp	00104$
00102$:
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:19: else count--;
	dec	_count
00104$:
	pop	acc
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;tens                      Allocated to registers r2 
;ones                      Allocated to registers r3 
;------------------------------------------------------------
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:22: void main() {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:24: IT0 = 1;
	setb	_IT0
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:25: IT1 = 1;
	setb	_IT1
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:26: EX0 = 1;
	setb	_EX0
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:27: EX1 = 1;
	setb	_EX1
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:28: EA  = 1;
	setb	_EA
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:30: while(1) {
00102$:
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:31: tens = count / 10;
	mov	b,#0x0A
	mov	a,_count
	div	ab
	mov	r2,a
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:32: ones = count % 10;
	mov	b,#0x0A
	mov	a,_count
	div	ab
	mov	r3,b
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:35: ERROR: no line number 35 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	_P2,#0xFD
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:36: ERROR: no line number 36 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	a,r2
	add	a,#_seg
	mov	r0,a
	mov	_P1,@r0
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:37: ERROR: no line number 37 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	push	ar3
	lcall	_delay
	pop	ar3
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:40: ERROR: no line number 40 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	_P2,#0xFE
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:41: ERROR: no line number 41 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	mov	a,r3
	add	a,#_seg
	mov	r0,a
	mov	_P1,@r0
;	C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c:42: ERROR: no line number 42 in file C:\Users\User\Desktop\Labmicro\LAB08M~1\3\3.c
	lcall	_delay
	sjmp	00102$
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
