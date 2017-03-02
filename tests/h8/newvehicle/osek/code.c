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
 * CVS: $Id: code.c,v 1.1 2003/10/20 12:41:47 pj Exp $
 */

#include "osektask.h"
#include "code.h"

/****************************************************************************
  Alarm callbacks
****************************************************************************/
int xxxx=0;

ALARMCALLBACK(alarm_emergencybeep)
{
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_SI1, 20);
#endif
  ERIKA_lcd_write_num(xxxx++);
}

ALARMCALLBACK(alarm_timeout)
{
  mode_change(MODE_FAULT2);
}


/****************************************************************************
  Application threads
****************************************************************************/

TASK(thread0)
{          
  int i;

  /* turn off light (bits 0-1 of port6) before A/D conversion */
  Port6 &= 0xFC;  
  for (i=0; i<20; i++);

  /* start A/D conversion */    
  if (mode != MODE_POWERON)
    ERIKA_ad_start();

  TerminateTask();
} 

// execution time (average):   1.2ms
// only if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED || mode==MODE_FAULT1) (see per.c)
TASK(thread1)
{
  /* convert sensors data (in mutual exclusion with readsensors() ) */
  DisableAllInterrupts();
  CTRL_convertsensors();
  EnableAllInterrupts();
  
  if (mode==MODE_FAULT1) 
    CTRL_observer_law();
  else
    CTRL_feedback_law();

  /* DIRTY TRICK
     note that in this way we avoid a race between the setting of the
     PWM value and the setting of that value to 0 done by an external
     interrupt source (that is, thread2 executing putting the state in
     fault2 or stop (This can happen also with priority
     thread1>thread2 because thread1 is triggered by the a/d
     converter; thread2 in this case can execute while the a/d
     converter is running */
  DisableAllInterrupts();
  if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED || mode==MODE_FAULT1)
    /* it will also call EnableAllInterrupts */
    ERIKA_PWM_set(CTRL_get_lmotor(), CTRL_get_rmotor());
  else
    EnableAllInterrupts();

  TerminateTask();
}

/* this task simply changes the application mode in response to an
   external command */
TASK(thread2)
{
  ERIKA_UINT8 c;

  c = PER_get_last_command();

  switch (c) {
  case CMD_STOP:
    if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED)
      mode_change(MODE_STOP);      
    break;
  case CMD_SLOW:
    if (mode==MODE_NORMAL || mode==MODE_STOP)
      mode_change(MODE_SLOWSPEED);      
    break;
  case CMD_FAST:
    if (mode==MODE_SLOWSPEED || mode==MODE_STOP)
      mode_change(MODE_NORMAL);
    break;
  case CMD_FAULT:
    if (mode==MODE_SLOWSPEED || mode==MODE_STOP || mode==MODE_NORMAL)
      mode_change(MODE_FAULT1);
    break;
  case CMD_FAULT2:
    if (mode==MODE_FAULT1)
      mode_change(MODE_FAULT2);
    break;
  case CMD_RECOVERY:
    if (mode==MODE_FAULT1)
      mode_change(MODE_SLOWSPEED);
    break;
  default:
    ERIKA_lcd_write_num(3333);
  }

  TerminateTask();
}

/****************************************************************************
  Boot and initialization
****************************************************************************/

#define DEFAULTAPPMODE 1

void StartupHook(void)
{
  /* initialization of the peripherals used in the application */
  PER_init();

  /* initialization of the control algorithm */
  CTRL_init();

  ERIKA_lcd_write_num(123);

  mode_change(MODE_SETUP1);

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0, 40);
  ERIKA_play_note(_LA0, 25);
#endif
}

void ERIKA_dummy(void)
{                 
  StartOS(DEFAULTAPPMODE);
}

