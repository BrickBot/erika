;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: handstub.s,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
			
;;; This file will be included by a ".s" user file. In that file the user
;;; will call M_set_handler for each handler that he want to install.

.macro M_set_handler handler,ext=_stub
	.section .text
	.align 1
	.global _\handler\ext
    			
_\handler\ext:
	;; don't need to disable interrupt: I=1 by Hw.
	adds	#2, r7		; remove dispatcher return ip
	pop	r6		; it will be pushed by handler
	;; save context
	push	r0
	push	r1
	push	r2
	push	r3
	push	r4
	push	r5
	;; set ERIKA_IRQ_nesting_level to 1
	mov 	#1, r1
	mov	r1, @_ERIKA_IRQ_nesting_level

	;; calls real handler

	jsr	@_\handler

.ifdef __FP__
	jmp	@_ERIKA_fp_IRQ_end_instance		
.endif
	
.endm	

	

	
