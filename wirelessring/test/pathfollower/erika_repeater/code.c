/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *               2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: code.c,v 1.1 2004/07/13 13:13:14 cirinei Exp $
 */

#include "code.h"
#include "alarms/alarms.h"


/****************************************************************************
  Alarm callbacks
****************************************************************************/
int xxxx=0;
void alarm_emergencybeep(void)
{
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_SI1, 20);
#endif
  ERIKA_lcd_write_num(xxxx++);
}

void alarm_timeout(void)
{
  mode_change(MODE_FAULT2);
}


/****************************************************************************
  Application threads
****************************************************************************/

void thread0(void)
{          
  int i;

  /* turn off light (bits 0-1 of port6) before A/D conversion */
  Port6 &= 0xFC;  
  for (i=0; i<20; i++);

  /* start A/D conversion */    
  if (mode != MODE_POWERON)
    ERIKA_ad_start();
} 

// execution time (average):   1.2ms
// only if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED || mode==MODE_FAULT1) (see per.c)
void thread1(void)
{
  /* convert sensors data (in mutual exclusion with readsensors() ) */
  ERIKA_interrupt_off();
  CTRL_convertsensors();
  ERIKA_interrupt_on();
  
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
  ERIKA_interrupt_off();
  if (mode==MODE_NORMAL || mode==MODE_SLOWSPEED || mode==MODE_FAULT1)
    ERIKA_PWM_set(CTRL_get_lmotor(), CTRL_get_rmotor());
  else
    ERIKA_interrupt_on();
}

/* this task simply changes the application mode in response to an
   external command */
void thread2(void)
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
}

/****************************************************************************
  Boot and initialization
****************************************************************************/

void ERIKA_dummy(void)
{                 
  /* H8 basic initialization */
  ERIKA_init();     

  /* initialization of the peripherals used in the application */
  PER_init();

  /* initialization of the control algorithm */
  CTRL_init();

  ERIKA_lcd_write_num(123);

  mode_change(MODE_SETUP1);

  ERIKA_hal_enableIRQ();
    
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0, 40);
  ERIKA_play_note(_LA0, 25);
#endif    

while (!ERIKA_button_pressed(PRGM_BUTTON));
	IL_pic_config();

  for (;;) {
    ERIKA_sys_idle();
  }
}

