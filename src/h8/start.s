;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: start.s,v 1.2 2003/10/20 10:28:56 pj Exp $

.equ	empty_stack, 0xEF30	;; bottom of memory (0x8000-0xEF2F: 28464Byte for data and code)
	
	.global __start
	.extern _ERIKA_dummy
	
.ifdef __MULTI__
        .extern _ERIKA_h8_sp
	.extern _ERIKA_h8_thread_stack_id
	.extern _ERIKA_h8_active_stack_id
.endif
		
.section .init
	.align 1		

;; first instruction executed by H8
__start:
        orc #0x80, ccr  ;; disable interrupt

	;; set Stack Pointer (sp is r7)
	mov.w 	#empty_stack, sp

.ifdef  __NO_ROM_DATA__
	;; nothing to copy
.else
	;; copy initialized data from ROM to RAM
	mov.w	#__DATA_ROM_START, r5
	mov.w 	#__DATA_RAM_START, r6
	mov.w	#__DATA_ROM_SIZE, r3
	mov.w	#0, r2	
	mov.w	#255, r1
_copy0:
	cmp.b	#0, r3h	;; test r3
	bne	_copy1		
	mov.w	r3, r4	;; r3 < 256
	eepmov
	bra	_end_copy
_copy1:
	mov.w	r1, r4	;; r3 > 255
	sub.w	r1, r3
	eepmov
	bra	_copy0
_end_copy:
.endif

.ifdef __CPP__
	mov.w	#__CTORS_START, r4
	mov.w 	#__CTORS_END, r5
_ctors0:
	cmp	r4,r5
	beq	_ctors1
	mov.w	@r4+,r3
	jsr	@r3
	jmp _ctors0
_ctors1:
	
.endif	
		
.ifdef __MULTI__        
        mov.b	@_ERIKA_h8_thread_stack_id, r1l       ; stack id of dummy
	mov.b	r1l, @_ERIKA_h8_active_stack_id	
	mov.b	#0,r1h
	add	r1,r1
	mov.w	@(_ERIKA_h8_sp, r1), sp
.endif

	jmp 	@_ERIKA_dummy

	;; validate the firmware
	.string "Do you byte, when I knock?"
	

