;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;;		  2004 Michele Cirinei
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: set_hand.s,v 1.2 2004/07/14 10:32:19 cirinei Exp $

.include "handstub.s"

;;;  handlers installed by system
.ifdef __TIMER_A_SUPPORT__
	M_set_handler ERIKA_timerA_handler    
.endif

.ifdef __TIMER_B_SUPPORT__
	M_set_handler ERIKA_timerB_handler
.endif

;;;  handlers installed by your application 
	M_set_handler ONOFFbutton_handler
  	M_set_handler RUNbutton_handler
  	M_set_handler ADI_handler
