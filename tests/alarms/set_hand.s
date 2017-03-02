;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: set_hand.s,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $

;;; H8 asm file

.include "handstub.s"
	
;;;  handlers installed by system
.ifdef __TIMER_A_SUPPORT__
	M_set_handler ERIKA_timerA_handler    
.endif

.ifdef __TIMER_B_SUPPORT__
	M_set_handler ERIKA_timerB_handler
.endif

.ifdef __MOTOR_A_PWM_USED__
	M_set_handler CMI1A_handler
.endif

.ifdef __MOTOR_C_PWM_USED__
	M_set_handler CMI1B_handler	
.endif

.ifdef __MOTOR_PWM_SUPPORT__
	M_set_handler OVI1_handler	
.endif
	

;;;  handlers installed by user application 


	
