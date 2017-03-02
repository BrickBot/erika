/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: sgetvalue.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "sem/sem.h"

#ifndef __PRIVATE_SEM_GETVALUE__
// returns -1 or the value of the semaphore
void ERIKA_sem_getvalue(ERIKA_SEM *s, ERIKA_TYPESEMCOUNT *value)
{
  ERIKA_hal_begin_primitive();

  if (s->first == ERIKA_NIL)
    *value = s->count;
  else
    *value = -1;

  ERIKA_hal_end_primitive();
}
#endif
