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
 * CVS: $Id: alcancel.c,v 1.2 2003/10/20 10:33:20 pj Exp $
 */

#include "erika.h"
#include "alarms/alarms.h"

#ifndef __PRIVATE_CANCELALARM__
void ERIKA_alarm_cancel(ERIKA_TYPEALARM AlarmID)
{
  register ERIKA_TYPEALARM current, previous;
  register ERIKA_FREG flags;

  flags = ERIKA_hal_begin_nested_primitive();

  /* to compute the relative value in ticks, we have to follow the counter
     delay chain */
  current = ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first;

  if (current == AlarmID) {
    /* the alarm is the first one in the delta queue */
    ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first = 
      ERIKA_alarm_RAM[AlarmID].next;
  } else {
    /* the alarm is not the first one in the delta queue */
    while (current != AlarmID) {
      previous = current;
      current = ERIKA_alarm_RAM[current].next;
      if (current == -1) {
	/* alarm not present */
	ERIKA_hal_end_nested_primitive(flags);
	return;
      }
    } 
    ERIKA_alarm_RAM[previous].next = ERIKA_alarm_RAM[AlarmID].next;
  }

  if (ERIKA_alarm_RAM[AlarmID].next != -1) {
    ERIKA_alarm_RAM[ERIKA_alarm_RAM[AlarmID].next].delta +=
      ERIKA_alarm_RAM[AlarmID].delta;
  }

  ERIKA_hal_end_nested_primitive(flags);
}
#endif
