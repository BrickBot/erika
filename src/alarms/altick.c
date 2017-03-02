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
 * CVS: $Id: altick.c,v 1.2 2003/10/20 10:33:21 pj Exp $
 */

#include "erika.h"
#include "alarms/alarms.h"

#ifndef __PRIVATE_COUNTER_TICK__
void ERIKA_alarm_insert(ERIKA_TYPEALARM AlarmID, ERIKA_TYPETICK increment)
{
  register ERIKA_TYPEALARM current, previous;

  current = ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first;
  //  ERIKA_lcd_write_num(9901); wait(); ERIKA_lcd_write_num(current); wait();
  //  ERIKA_lcd_write_num(9902); wait(); ERIKA_lcd_write_num(increment); wait();
  if (current == -1) {
    /* the alarm becomes the first into the delta queue */
    ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first = AlarmID;
  } else if (ERIKA_alarm_RAM[current].delta > increment) {
    ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first = AlarmID;
    ERIKA_alarm_RAM[current].delta -= increment;
  }
  else {
    /* the alarm is not the first into the delta queue */

    /* follow the delta chain until I reach the right place */
    do {
      increment -= ERIKA_alarm_RAM[current].delta;
      previous = current;
      current = ERIKA_alarm_RAM[current].next;
    } while(current != -1 && ERIKA_alarm_RAM[current].delta <= increment);

    /* insert the alarm between previous and current */
    if (current != -1)
      ERIKA_alarm_RAM[current].delta -= increment;
    ERIKA_alarm_RAM[previous].next = AlarmID;
  }
  //  ERIKA_lcd_write_num(9903); wait(); ERIKA_lcd_write_num(increment); wait();

  ERIKA_alarm_RAM[AlarmID].delta = increment;
  ERIKA_alarm_RAM[AlarmID].next = current;
}


void ERIKA_counter_tick(ERIKA_TYPECOUNTER c)
{
  register ERIKA_TYPEALARM current;
  register ERIKA_TID t;
  register ERIKA_FREG flags;

  flags = ERIKA_hal_begin_nested_primitive();

  /* increment the counter value */
  ERIKA_counter_RAM[c].value++;

  /* if there are queued alarms */
  if (ERIKA_counter_RAM[c].first != -1) {
    /* decrement first queued alarm delta */
    ERIKA_alarm_RAM[ERIKA_counter_RAM[c].first].delta--;

    /* execute all the alarms with counter 0 */
    current = ERIKA_counter_RAM[c].first;
    while (!ERIKA_alarm_RAM[current].delta) {

      /* remove the current entry */
      ERIKA_counter_RAM[c].first = ERIKA_alarm_RAM[current].next;

      /* the alarm is cyclic? */
      if (ERIKA_alarm_RAM[current].cycle) {
	/* enqueue it again 
	   note: this can modify ERIKA_counter_RAM[c].first!!! see (*)
	*/
	ERIKA_alarm_insert(current,ERIKA_alarm_RAM[current].cycle);
      }

      /* note: the alarm callbacks can maybe change the alarm
       * settings!!! that is the reason why we need to remove an alarm
       * before executing the notification
       */

      /* execute it */
      switch (ERIKA_alarm_ROM[current].action) {

      case  ERIKA_ALARM_ACTION_TASK:
	/* activate the task; NOTE: no preemption at all... 
	   This code was directly copied from ActivateTask */

	t = ERIKA_alarm_ROM[current].TaskID;

	/* this code is similar to the thread_make_ready primitive... */
#ifdef __ENABLE_THREAD__
	if (ERIKA_th_enabled[t]) {
#endif
    
	    if (ERIKA_th_nact[t] == 0) {
	      /* IDLE
	       * no preemption --> the thread goes into the ready
	       * queue The preemption test will be done into
	       * sys_scheduler()
	       */ 

#if defined(__MULTI__) || defined(__WITH_STATUS__)
	      ERIKA_th_status[t] = ERIKA_READY;
#endif
	      ERIKA_rq_insert(t);
	    }
	    
	    ERIKA_th_nact[t]++;
	
#ifdef __ENABLE_THREAD__
	}
#endif
  
  	break;

      case ERIKA_ALARM_ACTION_CALLBACK:
	((void (*)(void))ERIKA_alarm_ROM[current].f)();
	break;
      };
      
      /* (*) here we need ERIKA_counter_RAM[c].first again... */
      if ((current = ERIKA_counter_RAM[c].first) == -1) break;
    }
  }    

  ERIKA_hal_end_nested_primitive(flags);
}
#endif
