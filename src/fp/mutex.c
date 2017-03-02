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
 * CVS: $Id: mutex.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_MUTEX_LOCK__
void ERIKA_fp_mutex_lock(ERIKA_MUTEX m)
{
    ERIKA_hal_begin_primitive();

    ERIKA_mutex_oldceiling[m] = ERIKA_sys_ceiling;
    ERIKA_sys_ceiling |= ERIKA_mutex_ceiling[m];

    ERIKA_hal_end_primitive();
}
#endif /* __PRIVATE_MUTEX_LOCK__ */



#ifndef __PRIVATE_MUTEX_UNLOCK__
void ERIKA_fp_mutex_unlock(ERIKA_MUTEX m)
{
  register ERIKA_TID t;

  ERIKA_hal_begin_primitive();
  
  ERIKA_sys_ceiling = ERIKA_mutex_oldceiling[m];

  /* The following code is the same as the code contained into schedule.c! */

  t = ERIKA_rq_queryfirst();
  /* check if there is a preemption */
  if (t != ERIKA_NIL) {
    if (ERIKA_sys_ceiling < ERIKA_th_ready_prio[t]) {
#if defined(__MULTI__)
      register int flag;

      flag = ERIKA_th_status[t] & ERIKA_WASSTACKED;
#endif
      
#if defined(__MULTI__)|| defined(__WITH_STATUS__)
      ERIKA_th_status[t] = ERIKA_STACKED;
#endif

      ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[t];

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
  
  ERIKA_hal_end_primitive();
}		   
#endif /* __PRIVATE_MUTEX_UNLOCK__ */
