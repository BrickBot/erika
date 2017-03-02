/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: osekirq.h,v 1.1 2003/10/20 12:41:44 pj Exp $
 */

/* ERIKA Educational -> OSEK BCC1 interface stub */

#ifndef __OSEKIRQ_H__
#define __OSEKIRQ_H__

#include "erika.h"
#include "alarms/alarms.h"
#include "h8/serial.h"

#define TaskType ERIKA_TID
#define StatusType int

__INLINE__ TaskType ActivateTask(ERIKA_TID x)
{
  ERIKA_IRQ_make_ready(x, 1);
  return 0;
}

__INLINE__ void TerminateTask(void) {}

#define ISR(name) ERIKA_declare_stub(name); void (name)(void)

#define DisableAllInterrupts ERIKA_interrupt_off
#define EnableAllInterrupts ERIKA_interrupt_on

void StartupHook(void);
__INLINE__ void StartOS(int mode)
{
  /* H8 basic initialization */
  ERIKA_init();     

  StartupHook();

  ERIKA_hal_enableIRQ();

  for (;;) {   
    ERIKA_sys_idle();
  }
}


#endif
