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
 * CVS: $Id: osektask.h,v 1.1 2003/10/20 12:41:49 pj Exp $
 */

/* ERIKA Educational -> OSEK BCC1 interface stub */

#ifndef __OSEKTASK_H__
#define __OSEKTASK_H__

#include "erika.h"
#include "alarms/alarms.h"
#include "h8/serial.h"

#define DeclareTask(x)
#define TASK(x) void Func##x (void)

#define ActivateTask(x) ERIKA_thread_activate(x)
__INLINE__ void TerminateTask(void) {}

#define DeclareAlarm(x)
#define ALARMCALLBACK(x) void (x)(void)

#define SetRelAlarm ERIKA_alarm_setrel
#define CancelAlarm ERIKA_alarm_cancel

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


