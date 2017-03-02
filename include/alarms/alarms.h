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
 * CVS: $Id: alarms.h,v 1.2 2003/10/20 10:25:32 pj Exp $
 */


#ifndef __INCLUDE_ALARMS_ALARMS_H__
#define __INCLUDE_ALARMS_ALARMS_H__

#ifdef cplusplus
extern "C" {
#endif

/*************************************************************************
 Alarm Types
 *************************************************************************/

/* These types are defined into the Application dependent file types.h:

  ERIKA_TYPEALARM       (an ERIKA_INT8 (signed!) is enough, -1 is the invalid alarm)
  ERIKA_TYPECOUNTER     (an ERIKA_INT8 (signed!) is enough)
  ERIKA_TYPETICK        (the tick type for the counters)
  ERIKA_TYPENOTIFY      (a bitfield with 3 bits)
Note that the names retain the usual E.R.I.K.A. names, and their use is still
the same.
*/


/*************************************************************************
 Variables defined by the application
 *************************************************************************/

/* initvalue: {0, -1} */
typedef struct {
  ERIKA_TYPETICK value;              /* current value of the counter */
  ERIKA_TYPEALARM first;         /* first alarm queued on the counter */
} ERIKA_counter_RAM_type;

/* these are the different types of alarm notifications... */
#define ERIKA_ALARM_ACTION_TASK 0
#define ERIKA_ALARM_ACTION_CALLBACK 1

/* initvalue: {a_valid_counter, a_valid_action, then you must put the correct
   parameters depending on the action } */
typedef struct {
  ERIKA_TYPECOUNTER c;           /* the counter linked to the alarm */

  ERIKA_TYPENOTIFY action;
  
  ERIKA_TID TaskID;

  ERIKA_ADDR f;
} ERIKA_alarm_ROM_type;

/* initvalue: all zeroes --> no initialization! */
typedef struct {
  ERIKA_TYPETICK cycle;          /* cycle for periodic alarms */
  ERIKA_TYPETICK delta;          /* delta expiration time (into a queue!) */
  ERIKA_TYPEALARM next;          /* next alarm in the delta queue */
} ERIKA_alarm_RAM_type;

/* this is the RAM part of a counter. 
   Initvalue = an array of {0,-1} elements */
extern ERIKA_counter_RAM_type       ERIKA_counter_RAM[];

/* this is the fixed part of the configuration of an alarm
   Initvalue= depends on how the alarm notification have to be configured */
extern const ERIKA_alarm_ROM_type   ERIKA_alarm_ROM[];

/* this part is the variable part of an alarm.
   Initvalue: an array of {0,0,0,-1} */
extern ERIKA_alarm_RAM_type         ERIKA_alarm_RAM[];

/***************************************************************************
 * Implementation defined functions
 ***************************************************************************/

/* This function notifies a tick to a counter. That is, the counter is
   incremented by 1.  It must be called into an ISR or into a
   thread notify that the event linked to the counter occurred. 

   The function will also implement the notification of expired alarms
   (calling an alarm callback or activating a thread). 

   The function is NOT atomic, and NO RESCHEDULING will take place
   after the execution of this function. To implement the
   rescheduling at task level, you can use the Schedule() function
   just after this notification.

   These functions can be called both into a thread and into an ISR.
*/
#ifndef __PRIVATE_COUNTER_TICK__
void ERIKA_counter_tick(ERIKA_TYPECOUNTER c);
#endif

#ifndef __PRIVATE_GETALARM__
void ERIKA_alarm_get(ERIKA_TYPEALARM AlarmID, ERIKA_TYPETICK *Tick);
#endif

#ifndef __PRIVATE_SETRELALARM__
void ERIKA_alarm_setrel(ERIKA_TYPEALARM AlarmID, 
			ERIKA_TYPETICK increment, ERIKA_TYPETICK cycle);
#endif

#ifndef __PRIVATE_SETABSALARM__
void ERIKA_alarm_setabs(ERIKA_TYPEALARM AlarmID, 
			ERIKA_TYPETICK start, ERIKA_TYPETICK cycle);
#endif

#ifndef __PRIVATE_CANCELALARM__
void ERIKA_alarm_cancel(ERIKA_TYPEALARM AlarmID);
#endif

#ifdef cplusplus
}
#endif

#endif
