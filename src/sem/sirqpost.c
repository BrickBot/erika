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
 * CVS: $Id: sirqpost.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "sem/sem.h"

#ifndef __PRIVATE_IRQ_SEM_POST__
void ERIKA_IRQ_sem_post(ERIKA_SEM *s)
{
  ERIKA_TID newthread;

  ERIKA_hal_IRQ_begin_primitive();

  if (s->first != ERIKA_NIL) {
    newthread = s->first;

      // wake up blocked thread
      if ((s->first = ERIKA_th_next[newthread]) == ERIKA_NIL)
	s->last = ERIKA_NIL;
      
#if defined(__MULTI__) || defined(__WITH_STATUS__)
      ERIKA_th_status[newthread] = ERIKA_READY | ERIKA_WASSTACKED;
#endif
      ERIKA_rq_insert(newthread);
  }
    
  else
    s->count++;

  ERIKA_hal_IRQ_end_primitive();
}
#endif
