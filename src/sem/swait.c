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
 * CVS: $Id: swait.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "sem/sem.h"

#ifndef __PRIVATE_SEM_WAIT__
void ERIKA_sem_wait(ERIKA_SEM *s)
{
  ERIKA_TID current;

  ERIKA_hal_begin_primitive();

  if (s->count)
    s->count--;
  else {
    /* The running task blocks: 
       - it must be removed from the stacked queue
       - and then it must be inserted into the blocked queue */

    /* get the running task */	
    current = ERIKA_stk_queryfirst();

    /* extract the task from the stk data structure */
    ERIKA_stk_getfirst();

    /* The task state switch from STACKED TO BLOCKED */
#if defined(__MULTI__) || defined(__WITH_STATUS__)
    ERIKA_th_status[current] = ERIKA_BLOCKED;
#endif

    /* reset the thread priority bit in the system_ceiling */
    ERIKA_sys_ceiling &= ~ERIKA_th_dispatch_prio[current];

    if (s->first != ERIKA_NIL)
      // the semaphore queue is not empty
      ERIKA_th_next[s->last] = current;
    else
      // the semaphore queue is empty
      s->first = current;

    s->last = current;
    ERIKA_th_next[current] = ERIKA_NIL;

#if defined(__FP__)
    /* check if there is to schedule a ready thread or pop a preempted
     * thread 
     */
    if (ERIKA_rq_queryfirst() == ERIKA_NIL ||
	ERIKA_sys_ceiling >= ERIKA_th_ready_prio[ERIKA_rq_queryfirst()])
#endif
    {
      /* we have to schedule an interrupted thread that is on the top 
       * of its stack; the state is already STACKED! */
      ERIKA_hal_stkchange(ERIKA_stk_queryfirst());
    }
    else { 
      /* we have to schedule a ready thread that may be not yet stacked */
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
#endif
