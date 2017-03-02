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
 * CVS: $Id: spost.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "sem/sem.h"

#ifndef __PRIVATE_SEM_POST__
void ERIKA_sem_post(ERIKA_SEM *s)
{
  ERIKA_TID newthread;

  ERIKA_hal_begin_primitive();

  if (s->first != ERIKA_NIL) {
    newthread = s->first;

      // wake up blocked thread
      if ((s->first = ERIKA_th_next[newthread]) == ERIKA_NIL)
	s->last = ERIKA_NIL;
      
      /* check for preemption */ 
#if defined(__FP__)
      if (ERIKA_sys_ceiling < ERIKA_th_ready_prio[newthread]) {
#endif
	/* we have to schedule th blocked thread */
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	ERIKA_th_status[newthread] = ERIKA_STACKED;
#endif
	ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[newthread];
	
	// insert the extracted task on the topo of the stack
	ERIKA_th_next[newthread] = ERIKA_stk_queryfirst();
	ERIKA_stkfirst = newthread;
	
	ERIKA_hal_stkchange(newthread);
      }
      else {      
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	ERIKA_th_status[newthread] = ERIKA_READY | ERIKA_WASSTACKED;
#endif
	ERIKA_rq_insert(newthread);
      }
  }
  else
    s->count++;

  ERIKA_hal_end_primitive();
}
#endif
