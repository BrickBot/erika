/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: initkern.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"


/* ROM */
const ERIKA_TYPEPRIO ERIKA_th_ready_prio   [THREAD_MAX] = {0x01, 0x02};		/* task preemption level   */
const ERIKA_TYPEPRIO     ERIKA_th_dispatch_prio[THREAD_MAX] = {0x01, 0x02};		/* task threshold	   */

/* RAM */
/* Note: there are no pending activations!!! */
#if defined(__MULTI__) || defined(__WITH_STATUS__)
ERIKA_TYPESTATUS   ERIKA_th_status       [THREAD_MAX] = {ERIKA_READY, ERIKA_READY};	 /* status of the thread   */
#endif

ERIKA_TYPENACT     ERIKA_th_nact	     [THREAD_MAX] = {0, 0};		 	 /* pending activations    */
ERIKA_TID          ERIKA_th_next	     [THREAD_MAX] = {ERIKA_NIL, ERIKA_NIL};		 /* for task queues        */
ERIKA_TYPEPRIO     ERIKA_sys_ceiling                  = 0x0000;                      /* always 0...            */

/* The first task into the ready queue */
ERIKA_TID ERIKA_rqfirst  = ERIKA_NIL;

/* The first stacked task */
ERIKA_TID ERIKA_stkfirst = ERIKA_NIL;


/* alarms related data structures */

/* Alarms */

#include "alarms/alarms.h"

ERIKA_counter_RAM_type       ERIKA_counter_RAM[] = 
  {/* counter 1 */ {0, -1},
   /* counter 2 */ {0, -1}};

void mycallback(void);

const ERIKA_alarm_ROM_type   ERIKA_alarm_ROM[] = 
{
  {0, ERIKA_ALARM_ACTION_TASK    , 1, NULL },
  {0, ERIKA_ALARM_ACTION_CALLBACK, 0, (ERIKA_ADDR)mycallback},
  {1, ERIKA_ALARM_ACTION_CALLBACK, 0, (ERIKA_ADDR)mycallback} //note:counter 1
};

ERIKA_alarm_RAM_type         ERIKA_alarm_RAM[3];
