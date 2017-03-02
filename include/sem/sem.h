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
 * CVS: $Id: sem.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __INCLUDE_SEM_SEM_H__
#define __INCLUDE_SEM_SEM_H__

/* Semaphores 
   ----------

   These file declares the E.R.I.K.A. semaphores.
   A semaphore is contained in a data structure called ERIKA_SEM.
   That structure can be initialized statically (recommended), or
   dynamically using the macro sem_init.

   These functions can ONLY be used with a multistack HAL or similar,
   because these semaphore primitives are BLOCKING primitives.

   Types that must be defined by the user:
   ERIKA_TYPESEMCOUNT - Used as counter type for the semaphores.
                        typically is a WORD

*/

// The semaphore descriptor
typedef struct {
  ERIKA_TYPESEMCOUNT count;
  ERIKA_TID    first;
  ERIKA_TID    last;
} ERIKA_SEM;


#ifndef __PRIVATE_SEM_INIT__
#define ERIKA_sem_init(s,value) \
  (s).count = (value),	  \
  (s).first = ERIKA_NIL,  \
  (s).last = ERIKA_NIL
#endif

#ifndef __PRIVATE_SEM_WAIT__
void ERIKA_sem_wait(ERIKA_SEM *s);
#endif

#ifndef __PRIVATE_SEM_TRYWAIT__
/* returns 1 if the counter is decremented, 0 if not */
void ERIKA_sem_trywait(ERIKA_SEM *s, int *result);
#endif

#ifndef __PRIVATE_SEM_POST__
void ERIKA_sem_post(ERIKA_SEM *s);
#endif

#ifndef __PRIVATE_SEM_GETVALUE__
void ERIKA_sem_getvalue(ERIKA_SEM *s, ERIKA_TYPESEMCOUNT *value);
#endif

#ifndef __PRIVATE_IRQ_SEM_POST__
void ERIKA_IRQ_sem_post(ERIKA_SEM *s);
#endif

#endif
