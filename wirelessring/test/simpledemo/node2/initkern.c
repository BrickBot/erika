/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 */

#include "erikacfg.h"
#include "erika.h"
#include "alarms/alarms.h"
/*
 * ROM kernel descriptors
 */
     

const ERIKA_TYPEPRIO ERIKA_th_ready_prio[THREAD_MAX] = {
  0x01,
  0x01
};

const ERIKA_TYPEPRIO ERIKA_th_dispatch_prio[THREAD_MAX] = {
  0x01,
  0x01	
};

/*
 * RAM kernel descriptors
 */

#ifdef __MULTI__
ERIKA_TYPESTATUS ERIKA_th_status[THREAD_MAX] = {
  ERIKA_READY,
  ERIKA_READY
};
#endif

ERIKA_TYPENACT ERIKA_th_nact[THREAD_MAX] = {
  0,
  0
};

ERIKA_TID ERIKA_th_next[THREAD_MAX] = {
  ERIKA_NIL,
  ERIKA_NIL
};

ERIKA_TID ERIKA_rqfirst = ERIKA_NIL; 

ERIKA_TID ERIKA_stkfirst = ERIKA_NIL; 

// the ceiling of the first task;
ERIKA_TYPEPRIO ERIKA_sys_ceiling= 0x0000; 

ERIKA_counter_RAM_type ERIKA_counter_RAM[]=
{ {0,-1} };

const ERIKA_alarm_ROM_type ERIKA_alarm_ROM[]=
{ {0, ERIKA_ALARM_ACTION_TASK, TID_thread0, NULL},
  {0, ERIKA_ALARM_ACTION_TASK, TID_thread1, NULL}};

ERIKA_alarm_RAM_type ERIKA_alarm_RAM[2];

