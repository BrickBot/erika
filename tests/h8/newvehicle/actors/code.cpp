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
 * CVS: $Id: code.cpp,v 1.2 2003/11/11 13:53:54 pj Exp $
 */

#include "erika.h"
#include "osektask.h"
#include "osekcpp.hpp"
#include "code.hpp"

void wait(int x)
{
  ERIKA_lcd_write_num(x);
  while (!ERIKA_button_pressed(VIEW_BUTTON));
  while (ERIKA_button_pressed(VIEW_BUTTON));
}


/****************************************************************************
  Alarm callbacks
****************************************************************************/

extern "C" {

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
  evt_post(EVT_ALARM_TIMEOUT);
}

}



/****************************************************************************
  Application threads
****************************************************************************/

CPPTASK(thread0, 0)
{ 
  int i;

  /* turn off light (bits 0-1 of port6) before A/D conversion */
  Port6 &= 0xFC;  
  for (i=0; i<20; i++);

  if (mm.getMode() != MODE_POWERON)
    /* start A/D conversion */
    ERIKA_ad_start();

  TerminateTask();
} 



// execution time (average):   1.2ms
// only if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED || mode==MODE_FAULT1) (see per.c)
CPPTASK(thread1, 1)
{
  //  wait(5000);
  /* convert sensors data (in mutual exclusion with the input ports (see code inside control_new.cpp */
  id.fire();
  //  wait(5001);

  if (mm.getMode()==MODE_FAULT1) {
    obs.fire();
#ifdef ACTORDEBUG
    ddd.fire();
#endif
  }

  /* state feedback law */
  sf.fire();
  //  wait(5002);

  /* output conversion */
  oe.fire();
  //  wait(5003);

  /* DIRTY TRICK
     note that in this way we avoid a race between the setting of the
     PWM value and the setting of that value to 0 done by an external
     interrupt source (that is, thread2 executing putting the state in
     fault2 or stop (This can happen also with priority
     thread1>thread2 because thread1 is triggered by the a/d
     converter; thread2 in this case can execute while the a/d
     converter is running */
  osek_core.DisableAllInterrupts();
  if (mm.getMode()==MODE_NORMAL || mm.getMode()==MODE_SLOWSPEED || mm.getMode()==MODE_FAULT1)
    ou.fire();
  else
    osek_core.EnableAllInterrupts();
  //  wait(5004);

  TerminateTask();
}

/* this task simply changes the application mode in response to an
   external command */
CPPTASK(thread2, 2)
{
  ERIKA_UINT8 c;
  ModeID mode = mm.getMode();

  c = PER_get_last_command();

  switch (c) {
  case CMD_STOP:
    if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED)
      evt_post(EVT_CMD_STOP);      
    break;
  case CMD_SLOW:
    if (mode==MODE_NORMAL || mode==MODE_STOP)
      evt_post(EVT_CMD_SLOW);      
    break;
  case CMD_FAST:
    if (mode==MODE_SLOWSPEED || mode==MODE_STOP)
      evt_post(EVT_CMD_FAST);
    break;
  case CMD_FAULT:
    if (mode==MODE_SLOWSPEED || mode==MODE_STOP || mode==MODE_NORMAL)
      evt_post(EVT_CMD_FAULT);
    break;
  case CMD_FAULT2:
    if (mode==MODE_FAULT1)
      evt_post(EVT_CMD_FAULT2);
    break;
  case CMD_RECOVERY:
    if (mode==MODE_FAULT1)
      evt_post(EVT_CMD_RECOVERY);
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
  osek_core.StartupHook();
}

void myStartupHook(void)
{
  GBL_init();

  MODE_init();

  /* initialization of the peripherals used in the application */
  PER_init();

  ERIKA_lcd_write_num(123);

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0, 40);
  ERIKA_play_note(_LA0, 25);
#endif
}


class osek_core osek_core(myStartupHook);


void ERIKA_dummy(void)
{ 
  osek_core.StartOS(DEFAULTAPPMODE);
}

