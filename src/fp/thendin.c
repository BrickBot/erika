/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001 Paolo Gai
 *
 * This program is distributed under the GPL license contained into the
 * license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: thendin.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_THREAD_END_INSTANCE__
void ERIKA_fp_thread_end_instance(void)
{
  ERIKA_TID current;
  
  ERIKA_hal_begin_primitive();
  
  current = ERIKA_stk_queryfirst();
  
  /* decrease the pending activations... ready or stacked => (nact>0) */
  ERIKA_th_nact[current]--;
  
#if defined(__MULTI__) || defined(__WITH_STATUS__)
  /* The task state switch from STACKED TO READY because it end its
   * instance. Note that status=READY and nact=0 ==>> the task is
   * idle!!! */
  ERIKA_th_status[current] = ERIKA_READY;
#endif
  
  /* reset the thread priority bit in the system_ceiling */
  ERIKA_sys_ceiling &= ~ERIKA_th_dispatch_prio[current];
  
  /* extract the task from the stk data structure */
  ERIKA_stk_getfirst();
  
  if (ERIKA_th_nact[current] > 0) {
    /* there are pending activations...  */
    /* we have to reinsert the task into the ready queue before
     * rescheduling!!! */
    ERIKA_rq_insert(current);
  }
  
  /* check if there is to schedule a ready thread or pop a preempted
   * thread */
  if (ERIKA_rq_queryfirst() == ERIKA_NIL ||
      ERIKA_sys_ceiling >= ERIKA_th_ready_prio[ERIKA_rq_queryfirst()]) {
    /* we have to schedule an interrupted thread (already on the
     * stack!!!) the state is already STACKED! */
    ERIKA_hal_endcycle_stacked(ERIKA_stk_queryfirst());
  }
  else { 
    /* we have to schedule a ready thread */
#if defined(__MULTI__)
    register int flag;
    
    flag = ERIKA_th_status[ERIKA_rq_queryfirst()] & ERIKA_WASSTACKED;
#endif
    
#if defined(__MULTI__) || defined(__WITH_STATUS__)
    ERIKA_th_status[ERIKA_rq_queryfirst()] = ERIKA_STACKED;
#endif

    ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[ERIKA_rq_queryfirst()];

#if defined(__MULTI__)
    if (flag)
      ERIKA_hal_endcycle_stacked(ERIKA_rq2stk_exchange());
    else
      ERIKA_hal_endcycle_ready(ERIKA_rq2stk_exchange());
#else
    ERIKA_hal_endcycle_ready(ERIKA_rq2stk_exchange());
#endif
  }
}  
#endif
