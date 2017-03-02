/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: irq_tim.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifdef __H8__
 // called by timerA handler (every 130ms, see erikacfg.h)
 void ERIKA_timerA_user_function(void)
 {
   static ERIKA_UINT8 count=10;
   count--;    
   if (count==0)
   { 
     // executes every 1.3 s (130ms * 10)  
     ERIKA_IRQ_make_ready(0,1);
     count=10;
   }
 }
#endif

