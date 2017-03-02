;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under GPL license contained into the
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: drivers.s,v 1.8 2003/11/08 17:22:20 mancina Exp $

	.section .text
        
	.global _rom_func0		
	.global _rom_func1
	.global _rom_func2
	.global _rom_func3
	 
.ifdef __MOTOR_A_FP_USED__	
	.global _ERIKA_motorA_action	
	.align 1 	
    _ERIKA_motorA_action: 
	mov	#0xF000, r1  ; device register         
	bld	#1, r0l
	bst	#7, @r1
	bld	#0, r0l
	bst	#6, @r1
	rts
.endif
	
.ifdef __MOTOR_B_FP_USED__	
	.global _ERIKA_motorB_action	
	.align 1
    _ERIKA_motorB_action:
	mov	#0xF000, r1  ; device register         
	bld	#1, r0l
	bst	#3, @r1    ;; erika1.0 bug (write bit 5-4 instead of 3-2): fixed
	bld	#0, r0l
	bst	#2, @r1
	rts
.endif


.ifdef __MOTOR_C_FP_USED__	
	.global _ERIKA_motorC_action	
	.align 1	
    _ERIKA_motorC_action:
	mov	#0xF000, r1  ; device register         
	bld	#1, r0l
	bst	#1, @r1
	bld	#0, r0l
	bst	#0, @r1	          
	rts
.endif

;;; **** New PWM management ****
.ifdef __ERIKA_PWM_USED__
.ifndef __IR_SUPPORT__
	.global _CMI1A_handler
	.global _CMI1B_handler
	.global _OVI1_handler
	.extern	_ERIKA_PWM_cmi1a_value
	.extern	_ERIKA_PWM_cmi1b_value	
	.extern	_ERIKA_PWM_ovi1_value	
	
    .ifdef __ERIKA_PWM_TOFF_BRAKE__
	.equ	PORT_A_TOFF, 0xC0	;; set bit 7-6 to 1-1 (during TOFF)
	.equ	PORT_C_TOFF, 0x03	;; set bit 1-0 to 1-1 (during TOFF)
    .else
	.equ	PORT_A_TOFF, 0x3F	;; set bit 7-6 to 0-0 (during TOFF)
	.equ	PORT_C_TOFF, 0xFC	;; set bit 1-0 to 0-0 (during TOFF)
    .endif
    
    .ifdef __ERIKA_PWM_FAST__
	.equ	PORT_TON, 0x82		;; set bit 7-6-1-0 to 1-0-1-0 (during TON)					
    .endif
    
	.align 1
    ;;; **** PortA PWM
    _CMI1A_handler:
    	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; set output port register
        mov.b	@0xF000, r6l
	.ifdef __ERIKA_PWM_FAST__	
    	    .ifdef __ERIKA_PWM_TOFF_BRAKE__	
		or.b	#PORT_A_TOFF, r6l
	    .else
		and.b	#PORT_A_TOFF, r6l
	    .endif	
	.else	
	    and.b	#0x3F, r6l
	    mov.b	@_ERIKA_PWM_cmi1a_value, r6h
	    or.b	r6h, r6l	
	.endif	
        mov.b	r6l, @0xF000	    	
	;; reset match flag
	bclr	#6, @0xFFD1:8	
    ;; CMI1A_handler ended => return to RCX dispatcher
    rts 

    
    _CMI1B_handler:
	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; set output port register
        mov.b	@0xF000, r6l	    
	.ifdef __ERIKA_PWM_FAST__		
    	    .ifdef __ERIKA_PWM_TOFF_BRAKE__	
		or.b	#PORT_C_TOFF, r6l
	    .else
		and.b	#PORT_C_TOFF, r6l
	    .endif
	.else
	    and.b	#0xFC, r6l
	    mov.b	@_ERIKA_PWM_cmi1b_value, r6h	    
	    or.b	r6h, r6l	    
	.endif	
        mov.b	r6l, @0xF000	
	;; reset match flag
	bclr	#7, @0xFFD1:8	
    ;; CMI1B_handler ended => return to RCX dispatcher
    rts 
            
	.align 1
    _OVI1_handler:
	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; - either port A and C are powered
	;; set output port register	

        mov.b	@_ERIKA_PWM_ovi1_value, r6l
	.ifdef __ERIKA_PWM_FAST__		
	.else
	    mov.b	@0xF000, r6h
	    and.b	#0x3C, r6h
	    or.b	r6h, r6l	    
	.endif	
        mov.b	r6l, @0xF000
	
	;; reset match flag
	bclr	#5, @0xFFD1:8
    ;; OVI1__handler ended => return to RCX dispatcher
    rts     
.else
	.global _CMI0A_handler
	.global _CMI0B_handler
	.global _OVI0_handler
	.extern	_ERIKA_PWM_cmi0a_value
	.extern	_ERIKA_PWM_cmi0b_value	
	.extern	_ERIKA_PWM_ovi0_value	
	
    .ifdef __ERIKA_PWM_TOFF_BRAKE__
	.equ	PORT_A_TOFF, 0xC0	;; set bit 7-6 to 1-1 (during TOFF)
	.equ	PORT_C_TOFF, 0x03	;; set bit 1-0 to 1-1 (during TOFF)
    .else
	.equ	PORT_A_TOFF, 0x3F	;; set bit 7-6 to 0-0 (during TOFF)
	.equ	PORT_C_TOFF, 0xFC	;; set bit 1-0 to 0-0 (during TOFF)
    .endif
    
    .ifdef __ERIKA_PWM_FAST__
	.equ	PORT_TON, 0x82		;; set bit 7-6-1-0 to 1-0-1-0 (during TON)					
    .endif
    
	.align 1
    ;;; **** PortA PWM
    _CMI0A_handler:
	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; set output port register
        mov.b	@0xF000, r6l
	.ifdef __ERIKA_PWM_FAST__	
    	    .ifdef __ERIKA_PWM_TOFF_BRAKE__	
		or.b	#PORT_A_TOFF, r6l
	    .else
		and.b	#PORT_A_TOFF, r6l
	    .endif	
	.else	
	    and.b	#0x3F, r6l
	    mov.b	@_ERIKA_PWM_cmi0a_value, r6h
;;	    and.b       #0xC0, r6h
	    or.b	r6h, r6l	
	.endif	
        mov.b	r6l, @0xF000	    	
	;; reset match flag
	bclr	#6, @0xFFC9:8	
    ;; CMI0A_handler ended => return to RCX dispatcher
    rts 
    
    
    _CMI0B_handler:
	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; set output port register
        mov.b	@0xF000, r6l	    
	.ifdef __ERIKA_PWM_FAST__		
    	    .ifdef __ERIKA_PWM_TOFF_BRAKE__	
		or.b	#PORT_C_TOFF, r6l
	    .else
		and.b	#PORT_C_TOFF, r6l
	    .endif
	.else
	    and.b	#0xFC, r6l
	    mov.b	@_ERIKA_PWM_cmi0b_value, r6h	    
	    or.b	r6h, r6l	    
	.endif	
        mov.b	r6l, @0xF000	
	;; reset match flag
	bclr	#7, @0xFFC9:8	
    ;; CMI0B_handler ended => return to RCX dispatcher
    rts 
            
	.align 1
    _OVI0_handler:
	;; NOTE:
	;; - Interrupt are masked by RCX dispatcher
	;; - r6 is saved by dispatcher so here we don't save it
	;; - either port A and C are powered
	;; set output port register	

        mov.b	@_ERIKA_PWM_ovi0_value, r6l
	.ifdef __ERIKA_PWM_FAST__		
	.else
	    mov.b	@0xF000, r6h
	    and.b	#0x3C, r6h
	    or.b	r6h, r6l	    
	.endif	
        mov.b	r6l, @0xF000
	
	;; reset match flag
	bclr	#5, @0xFFC9:8
    ;; OVI0__handler ended => return to RCX dispatcher
    rts     
.endif		
.endif

	
;;; ****************************
	

;;; utility function

_rom_func0:
	    	push    r6
    		jsr     @r0
    		mov.w   r6,r0
    		pop     r6
    		rts

	        
_rom_func1:
                push    r6
                mov.w   r1,r6
                jsr     @r0
                mov.w   r6,r0
                pop     r6
                rts

	       
_rom_func2:
		push    r6
    		push    r2
    		mov.w   r1,r6
    		jsr     @r0
    		adds    #2,r7
    		mov.w   r6,r0
    		pop     r6
    		rts
	       
_rom_func3:
    		push    r6
    		mov.w   @(4,r7),r3
    		push    r3
    		push    r2
    		mov.w   r1,r6
    		jsr     @r0
    		adds    #2,r7
    		adds    #2,r7
    		mov.w   r6,r0
    		pop     r6
    		rts


.ifdef __IR_SUPPORT__
.include "handstub.s" 
        M_set_handler ERIKA_TXI_handler
	M_set_handler ERIKA_TEI_handler
 	M_set_handler ERIKA_RXI_handler
        M_set_handler ERIKA_ERI_handler

.ifdef	__ERIKA_PWM_USED__
	M_set_handler CMI0A_handler
	M_set_handler CMI0B_handler
	M_set_handler OVI0_handler
.endif
.else
.ifdef	__IR_RX_SUPPORT__
.include "handstub.s"
	M_set_handler ERIKA_RXI_handler
	M_set_handler ERIKA_ERI_handler
.endif
.ifdef	__ERIKA_PWM_USED__
.include "handstub.s"
	M_set_handler CMI1A_handler
	M_set_handler CMI1B_handler
	M_set_handler OVI1_handler
.endif
.endif
