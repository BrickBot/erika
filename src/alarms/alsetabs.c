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
 * CVS: $Id: alsetabs.c,v 1.2 2003/10/20 10:33:21 pj Exp $
 */

#include "erika.h"
#include "alarms/alarms.h"

void ERIKA_alarm_insert(ERIKA_TYPEALARM AlarmID, ERIKA_TYPETICK increment);

#ifndef __PRIVATE_SETABSALARM__
void ERIKA_alarm_setabs(ERIKA_TYPEALARM AlarmID, 
			ERIKA_TYPETICK start, 
			ERIKA_TYPETICK cycle)
{
  register ERIKA_FREG flags;

  flags = ERIKA_hal_begin_nested_primitive();

  ERIKA_alarm_RAM[AlarmID].cycle = cycle;

  ERIKA_alarm_insert(AlarmID, start - 
		     ERIKA_counter_RAM[ERIKA_alarm_ROM[AlarmID].c].value);
  
  ERIKA_hal_end_nested_primitive(flags);
}
#endif
