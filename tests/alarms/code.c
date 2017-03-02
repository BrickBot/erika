/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2000-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: code.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */


#include "erika.h"
#include "alarms/alarms.h"


/* Test Description:
   This test has 4 tasks with priority th1<th2<th3<th4

   This test uses:
   - Multistack
   - StartOS
   - ActivateTask into StartupHook (no preemption)
   - TerminateTask on the base level
   - Schedule with and without preemption
   - 2 alarms attached to a counter
   - 1 alarm attached to another counter
   - GetAlarmBase SetRelAlarm SetAbsAlarm CancelAlarm

   Basically, the test repeatly calls the tick function to increment
   ticks on the two counters. Only one task is used, the others are
   never activated...
*/

#include "code.h"

#ifdef __LCD_USED__
/*
 * Display a number during a short pause 
 */
//#define LOOP_LENGHT 100000
#ifndef __H8__
#define LOOP_LENGHT 10000
#else
#define LOOP_LENGHT 1000
#endif
//#define LOOP_LENGHT 100
//#define LOOP_LENGHT 10
//#define LOOP_LENGHT 1
void pause_and_display(int disp)
{
  int i=0;
  int j=0;
  
  for (i=0; i<LOOP_LENGHT; i++) {
#ifdef __LCD_USED__        
    ERIKA_LCD_display(disp);
#endif
    for(j=0; j<10; j++) {
      /* do nothing */
    }
  }
}
#else
#define pause_and_display(x)
#endif



volatile int activations = 0;
volatile int callbacks = 0;

void mycallback(void)
{
  callbacks++;
}

void thread0(void)
{
  ERIKA_TYPETICK t;
  int x;
  
#ifdef __LCD_USED__
    ERIKA_play_note(_DO0, 10);
    ERIKA_LCD_display(0);
#endif
    
  pause_and_display(0x0);

  ERIKA_alarm_setrel(alarm0, 3, 3);
  ERIKA_alarm_setabs(alarm1, 2, 2);
  ERIKA_alarm_setrel(alarm2, 2, 0);

  ERIKA_alarm_get(alarm0, &t);
  
  for (x=0; x<10; x++) {
    ERIKA_counter_tick(0);
    ERIKA_counter_tick(1);
    ERIKA_sys_scheduler();
  }

  ERIKA_alarm_setabs(alarm2, 14, 0);

  ERIKA_alarm_cancel(alarm0);
  ERIKA_alarm_cancel(alarm2);
  ERIKA_alarm_cancel(alarm1);

  ERIKA_alarm_setrel(alarm0, 3, 3);
  ERIKA_alarm_setabs(alarm1, 12, 0);

  for (x=0; x<10; x++) {
    ERIKA_counter_tick(0);
    ERIKA_counter_tick(1);
    ERIKA_sys_scheduler();
  }
  

#ifdef __LCD_USED__
    ERIKA_LCD_clear();
#endif
}



void thread1(void)
{
  activations++;

#ifdef __LCD_USED__
    ERIKA_LCD_display(1);
#endif
}

void ERIKA_dummy(void)
{
#ifdef __H8__
  ERIKA_init();
#endif

#ifdef __LCD_USED__
  ERIKA_LCD_init();
#endif

  ERIKA_thread_make_ready(0,1);
  ERIKA_thread_activate(1);

  for (;;) {
    // Background activities, none in this test
#ifdef __LCD_USED__
    ERIKA_LCD_display(-1);
#endif    
    ERIKA_sys_idle();
  }
}



