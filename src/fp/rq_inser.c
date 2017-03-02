/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: rq_inser.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_RQ_INSERT__
// this function inserts a task into the ready queue
void ERIKA_rq_insert(ERIKA_TID t)
{
  ERIKA_TYPEPRIO prio;
  ERIKA_TID p,q;

  p = ERIKA_NIL;
  q = ERIKA_rqfirst;
  prio = ERIKA_th_ready_prio[t];

  while ((q != ERIKA_NIL) && (prio <= ERIKA_th_ready_prio[q])) {
    p = q;
    q = ERIKA_th_next[q];
  }

  if (p != ERIKA_NIL)
    ERIKA_th_next[p] = t;
  else
    ERIKA_rqfirst = t;

  ERIKA_th_next[t] = q;
}
#endif
