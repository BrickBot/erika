/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *		 2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: mode.c,v 1.1 2004/07/13 13:13:15 cirinei Exp $
 */

#include "code.h"
#include "alarms/alarms.h"


extern ERIKA_INT16 offset_err;

/* application needs to setup sensor at at startup (see setup_handler) */
ERIKA_UINT8 mode = MODE_POWERON; 

/* mode masks */
#define MASK_POWERON   0x0001
#define MASK_SETUP1    0x0002
#define MASK_SETUP2    0x0004
#define MASK_SETUP3    0x0008
#define MASK_STOP      0x0010
#define MASK_NORMAL    0x0020
#define MASK_SLOWSPEED 0x0040
#define MASK_FAULT1    0x0080
#define MASK_FAULT2    0x0100
#define MASK_SHUTDOWN  0x0200
#define MASK_SETUP4    0x0400

const ERIKA_INT16 mode_valid_transactions[] = {
  /* poweron   */ MASK_SETUP1                                            ,
  /* setup1    */ MASK_SHUTDOWN|MASK_SETUP2                              ,
  /* setup2    */ MASK_SHUTDOWN|MASK_SETUP3                              ,
  /* setup3    */ MASK_SHUTDOWN|MASK_SETUP4				 ,
  /* setup4    */ MASK_SHUTDOWN|MASK_STOP                                ,
  /* stop      */ MASK_SHUTDOWN|MASK_NORMAL   |MASK_SLOWSPEED|MASK_FAULT1,
  /* normal    */ MASK_SHUTDOWN|MASK_STOP     |MASK_SLOWSPEED|MASK_FAULT1,
  /* slowspeed */ MASK_SHUTDOWN|MASK_STOP     |MASK_NORMAL   |MASK_FAULT1,
  /* fault1    */ MASK_SHUTDOWN|MASK_SLOWSPEED|MASK_FAULT2               ,
  /* fault2    */ MASK_SHUTDOWN                                          ,
  /* shutdown  */ MASK_POWERON
};


/****************************************************************************
  poweroff
****************************************************************************/


void jump_from_poweron(ERIKA_UINT8 newmode)
{
  /* do nothing */
}

void jump_to_poweron(ERIKA_UINT8 oldmode)
{
  /* do nothing */
}

/****************************************************************************
  setup1
****************************************************************************/

void jump_from_setup1(ERIKA_UINT8 newmode)
{
  /* do nothing */
}

void jump_to_setup1(ERIKA_UINT8 oldmode)
{
  ERIKA_alarm_setrel(alarm_thread0, 3, 3); // 6 ms
}

/****************************************************************************
  setup2
****************************************************************************/

void jump_from_setup2(ERIKA_UINT8 newmode)
{
  /* do nothing */
}

void jump_to_setup2(ERIKA_UINT8 oldmode)
{
  /* from setup1 only */

#ifdef __PLAY_NOTE_SUPPORT__		
    ERIKA_play_note(_DO0,10);     
#endif
      
    CTRL_set_black();
		
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_DO0,20);
#endif	
}

/****************************************************************************
  setup3
****************************************************************************/

void jump_from_setup3(ERIKA_UINT8 newmode)
{
  /* do nothing */
}

void jump_to_setup3(ERIKA_UINT8 oldmode)
{
    CTRL_set_yref();
		
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_RE0,6);	
#endif	
}

/****************************************************************************
  setup4
****************************************************************************/

void jump_from_setup4(ERIKA_UINT8 newmode)
{
  /* do nothing */
}

void jump_to_setup4(ERIKA_UINT8 oldmode)
{
    CTRL_set_ywhite();
		
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_MI0,6);	
#endif	
}

/****************************************************************************
  stop
****************************************************************************/

void jump_from_stop(ERIKA_UINT8 newmode)
{
  /* poweron, normal, slowspeed, fault1 */
//  ERIKA_lcd_clear();
}

void jump_to_stop(ERIKA_UINT8 oldmode)
{
  ERIKA_PWM_set(0,0);

  if (oldmode == MODE_SETUP4) {
    CTRL_set_offset();
	
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_FA0,10);	
#endif	
  }
}

/****************************************************************************
  normal
****************************************************************************/

void jump_from_normal(ERIKA_UINT8 newmode)
{
  /* poweron, stop, slowspeed, fault1 */
}

void jump_to_normal(ERIKA_UINT8 oldmode)
{
  CTRL_set_normalpowref(NORMSPEED_POWREF);
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_SOL0,10);	
#endif	
}

/****************************************************************************
  slowspeed
****************************************************************************/

void jump_from_slowspeed(ERIKA_UINT8 newmode)
{  
  /* poweron, stop, normal, fault1 */
}

void jump_to_slowspeed(ERIKA_UINT8 oldmode)
{
  CTRL_set_normalpowref(SLOWSPEED_POWREF);

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_SOL0,10);	
#endif	
}

/****************************************************************************
  fault1
****************************************************************************/

void jump_from_fault1(ERIKA_UINT8 newmode)
{
  ERIKA_alarm_cancel(alarm_beeper);
  ERIKA_alarm_cancel(alarm_fault2timeout);
}

void jump_to_fault1(ERIKA_UINT8 oldmode)
{
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_SOL0,10);	
#endif	

  ERIKA_alarm_setrel(alarm_beeper, 500,500);
  ERIKA_alarm_setrel(alarm_fault2timeout, 5000, 0);
}

/****************************************************************************
  fault2
****************************************************************************/

void jump_from_fault2(ERIKA_UINT8 newmode)
{
  ERIKA_alarm_cancel(alarm_beeper);
}

void jump_to_fault2(ERIKA_UINT8 oldmode)
{
  ERIKA_PWM_set(0,0);

#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_SOL0,10);	
#endif	

  ERIKA_alarm_setrel(alarm_beeper, 166,166);
}

/****************************************************************************
  shutdown
****************************************************************************/

void jump_from_shutdown(ERIKA_UINT8 newmode)
{
  ERIKA_sys_reboot();
}

void jump_to_shutdown(ERIKA_UINT8 oldmode)
{
  ERIKA_alarm_cancel(alarm_thread0); // 6 ms

  Port6 &= 0xF8;
  
  MotorDevReg=0x00;
  
  ERIKA_lcd_clear();
  //ERIKA_disable_irq(RUNBUTTON_IRQ);
  //ERIKA_lcd_write_num(7777);
  
  /* go into software standby mode */
  SYSCR |= 0x80;	
  __asm__ (".align 1");
  __asm__ ("andc	#0x7F, ccr");	
  __asm__ ("sleep"); 
}



/****************************************************************************
  the mode tables
****************************************************************************/

void (* const mode_jump_from[])(ERIKA_UINT8 newm) = {
  jump_from_poweron,
  jump_from_setup1,
  jump_from_setup2,
  jump_from_setup3,
  jump_from_stop,
  jump_from_normal,
  jump_from_slowspeed,
  jump_from_fault1,
  jump_from_fault2,
  jump_from_shutdown,
  jump_from_setup4
};

void (* const mode_jump_to[])(ERIKA_UINT8 oldm) = {
  jump_to_poweron,
  jump_to_setup1,
  jump_to_setup2,
  jump_to_setup3,
  jump_to_stop,
  jump_to_normal,
  jump_to_slowspeed,
  jump_to_fault1,
  jump_to_fault2,
  jump_to_shutdown,
  jump_to_setup4
};

/****************************************************************************
  the mode change function
****************************************************************************/

void mode_change(ERIKA_UINT8 m)
{
  ERIKA_UINT8 oldmode;
  if (m != mode && (mode_valid_transactions[mode] | m)) {
    /* this is a valid mode where we can jump to */

    /* leave the current mode to mode m */
    mode_jump_from[mode](m);

    oldmode = mode;
    mode = m;
    /* enter the new mode (these functions can access the current mode */
    mode_jump_to[m](oldmode);
  }
  else {
    /* hmmm... something strange happened */
    while(1)
      ERIKA_lcd_write_num(8000+mode*100+m);
  }
}
