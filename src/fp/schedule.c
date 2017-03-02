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
 * CVS: $Id: schedule.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_SYS_SCHEDULER__
void ERIKA_fp_sys_scheduler(void)
{
  register ERIKA_TID t;

  ERIKA_hal_begin_primitive();

  t = ERIKA_rq_queryfirst();

  if (t != ERIKA_NIL) {
    if (ERIKA_sys_ceiling < ERIKA_th_ready_prio[t]) {
      // we have to schedule a ready thread
#if defined(__MULTI__)
      register int flag;

      flag = ERIKA_th_status[t] & ERIKA_WASSTACKED;
#endif

      ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[t];

#if defined(__MULTI__) || defined(__WITH_STATUS__)
      ERIKA_th_status[t] = ERIKA_STACKED;
#endif

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
