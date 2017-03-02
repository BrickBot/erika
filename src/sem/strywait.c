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
 * CVS: $Id: strywait.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "sem/sem.h"

#ifndef __PRIVATE_SEM_TRYWAIT__
/* returns 1 if the counter is decremented, 0 if not */
void ERIKA_sem_trywait(ERIKA_SEM *s, int *result)
{
  ERIKA_hal_begin_primitive();

  if (s->count) {
    s->count--;
    *result = 1;
  }
  else
    *result = 0;

  ERIKA_hal_end_primitive();
}
#endif
