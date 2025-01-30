	TMR5_CTL  = 008Fh		; Timer 5 control register
	TMR5_DR_L = 0090h		; Timer 5 data-read (LSB) --> read this first
	TMR5_DR_H = 0091h		; Timer 5 data-read (MSB) --> read this after LSB
	TMR5_RR_L = 0090h		; Timer 5 reload register (LSB)
	TMR5_RR_H = 0091h		; Timer 5 reload register (MSB)


	assume	ADL=1

	section	.text
	public	_timerTestSetup
	public	_timer5interrupt
	
_timerTestSetup:
	push af
	push bc
	push de
	
	ld a, 10h + (08h + 04h) + 01h
	out (TMR5_CTL), a		; TMR5_CTL
	ld a, 0ffh		
	out (TMR5_RR_L), a		; TMR5_RR_L	
	out (TMR5_RR_H), a		; TMR5_RR_H
	
	ld bc, 0ffffh
	ld (_upperTimer5Counter), bc
	
	ld a, 18h
	ld e, 10h				;mos_setintvector ---> TMR5 interrupt at interrupt vector table row 10h
	ld.lil hl, _timer5interrupt
	rst.lil 08h
	
	
	ld a, 40h + 10h + (08h + 04h) + 02h + 01h	
	out (TMR5_CTL), a	; TMR5_CTL ---> enable TMR5
	
	
	ld a,'@' 			;get character in A
	rst.lil 010h		;send to MOS / VDP
	
	pop de
	pop bc
	pop af
	ret
	
	
_timer5interrupt:
	di
	push af
	push.l hl
	
	ld.lil hl, _upperTimer5Counter
	dec.lil (hl)
	in a, (TMR5_CTL)		; Read TMR5_CTL
	
	ld a, '@'				; Print '@' in the console each time this interrupt is triggered.
	rst.lil 10h	
	
	pop.l hl
	pop af
	ei
	reti
	
	
	
	section	.data,"aw",@progbits
	public	_upperTimer5Counter
_upperTimer5Counter:
	db	10
