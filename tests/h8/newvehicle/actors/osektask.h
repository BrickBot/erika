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
 * CVS: $Id: osektask.h,v 1.1 2003/10/20 12:44:06 pj Exp $
 */

/* ERIKA Educational -> OSEK BCC1 interface stub */

#ifndef __OSEKTASK_H__
#define __OSEKTASK_H__

#include "erika.h"
#include "alarms/alarms.h"
#include "h8/serial.h"

#ifdef cplusplus
extern "C" {
#endif

#define TaskType ERIKA_TID
#define StatusType int

#define DeclareTask(x)
#define TASK(x) void Func##x (void)

__INLINE__ StatusType ActivateTask(TaskType x) 
{ 
  ERIKA_thread_activate(x);
  return 0;
}

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
    //ERIKA_LCD_display(mallocpointer);
    ERIKA_sys_idle();
  }
}

#ifdef cplusplus
}
#endif

#endif


