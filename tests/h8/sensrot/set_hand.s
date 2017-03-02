;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: set_hand.s,v 1.2 2003/11/08 17:30:18 mancina Exp $

.include "handstub.s"
	
;;;  handlers installed by system
.ifdef __TIMER_A_SUPPORT__
	M_set_handler ERIKA_timerA_handler    
.endif

.ifdef __TIMER_B_SUPPORT__
	M_set_handler ERIKA_timerB_handler
.endif

;;;  handlers installed by user application 

	M_set_handler ONOFFBUTTON_handler
	M_set_handler RUNBUTTON_handler	
	M_set_handler ADI_handler
	
