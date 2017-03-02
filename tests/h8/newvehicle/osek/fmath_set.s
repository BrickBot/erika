;;; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;;; 
;;; http://erika.sssup.it
;;; 
;;; Copyright (C) 2002 Davide Cantini
;;; 
;;; This program is distributed under the GPL license contained into
;;; the license.txt file on the E.R.I.K.A. root directory.
;;; 
;;; CVS: $Id: fmath_set.s,v 1.1 2003/10/20 12:41:48 pj Exp $

;; 
;;  Set number of bit used for fractional part of 16bit fixed point number
;;
	
#ifdef __UNSIGNED_FIXED_USED__			
	.equ	ERIKA_UF_NUM_BIT_FRAC, 4	;; 0 < UF_NUM_BIT_FRAC < 16
#endif


#ifdef __SIGNED_FIXED_USED__

	.equ	ERIKA_SF_NUM_BIT_FRAC, 9	;; 0 < SF_NUM_BIT_FRAC < 16
#endif
