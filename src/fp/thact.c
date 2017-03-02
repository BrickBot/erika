/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001-2002 Paolo Gai, Alessandro Colantonio
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: thact.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_THREAD_ACTIVATE__

void ERIKA_fp_thread_activate(ERIKA_TID t)
{
  register ERIKA_TID tmp;
  
  ERIKA_hal_begin_primitive();
  
#ifdef __ENABLE_THREAD__
  if (ERIKA_th_enabled[t]) {
#endif
    
      /* check for first activation */
      if (ERIKA_th_nact[t] == 0) {
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	ERIKA_th_status[t] = ERIKA_READY;
#endif
	ERIKA_rq_insert(t);
      }
      
      /* activate the thread */
      ERIKA_th_nact[t] ++;
      
      /* check for preemption */ 
      tmp = ERIKA_rq_queryfirst();
      if (tmp != ERIKA_NIL) {
	if (ERIKA_sys_ceiling < ERIKA_th_ready_prio[tmp]) {
#if defined(__MULTI__)
	  register int flag;
	  
	  flag = ERIKA_th_status[tmp] & ERIKA_WASSTACKED;
#endif
	  
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	  ERIKA_th_status[tmp] = ERIKA_STACKED;
#endif
	  
	  ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[tmp];
	  
#if defined(__MULTI__)
	  if (flag)
	    ERIKA_hal_stkchange(ERIKA_rq2stk_exchange());
	  else
	    ERIKA_hal_ready2stacked(ERIKA_rq2stk_exchange());
#else
	  ERIKA_hal_ready2stacked(ERIKA_rq2stk_exchange());
#endif
	}
      }
      
#ifdef __ENABLE_THREAD__
  }
#endif
    
  ERIKA_hal_end_primitive();
}

#endif
