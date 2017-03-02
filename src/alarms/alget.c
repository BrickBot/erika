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
 * CVS: $Id: alget.c,v 1.2 2003/10/20 10:33:21 pj Exp $
 */

#include "erika.h"
#include "alarms/alarms.h"

#ifndef __PRIVATE_GETALARM__
void ERIKA_alarm_get(ERIKA_TYPEALARM AlarmID, ERIKA_TYPETICK *Tick)
{
  register ERIKA_TYPEALARM current;
  register ERIKA_FREG flags;

  flags = ERIKA_hal_begin_nested_primitive();

  /* to compute the relative value in ticks, we have to follow the counter
     delay chain */
  current = ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].first;
  *Tick = ERIKA_alarm_RAM[current].delta;
  while (current != AlarmID) {
    current = ERIKA_alarm_RAM[current].next;
    *Tick += ERIKA_alarm_RAM[current].delta;
  } 

  ERIKA_hal_end_nested_primitive(flags);
}
#endif
