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
 * CVS: $Id: mode.cpp,v 1.2 2003/11/11 13:53:54 pj Exp $
 */

extern "C" {
#include "osektask.h"
}

#include "code.hpp"
#include "include/port.hpp"
#include "include/rewritereceiver.hpp"

extern "C" {
DeclareAlarm(alarm_thread0);
DeclareAlarm(alarm_beeper);
DeclareAlarm(alarm_fault2timeout);
}

/****************************************************************************
  poweron
****************************************************************************/

class poweronMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void poweronMode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void poweronMode::enterMode(ModeID oldmode)
{
  ERIKA_sys_reboot();
}

/****************************************************************************
  deletefirmware
****************************************************************************/

class deletefirmwareMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void deletefirmwareMode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void deletefirmwareMode::enterMode(ModeID oldmode)
{
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_DO0, 50);
#endif
    delete_firmware();	
}


/****************************************************************************
  setup1
****************************************************************************/

class setup1Mode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void setup1Mode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void setup1Mode::enterMode(ModeID oldmode)
{
  SetRelAlarm(alarm_thread0, 3, 3); // 6 ms

  //ERIKA_lcd_write_num(3405);
  //while (!ERIKA_button_pressed(VIEW_BUTTON));
  //while (ERIKA_button_pressed(VIEW_BUTTON));
}

/****************************************************************************
  setup2
****************************************************************************/

class setup2Mode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void setup2Mode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void setup2Mode::enterMode(ModeID oldmode)
{
  /* from setup1 only */

#ifdef __PLAY_NOTE_SUPPORT__		
    ERIKA_play_note(_DO0,10);     
#endif
      
    id.setBlack();
		
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_DO0,20);
#endif	
}

/****************************************************************************
  setup3
****************************************************************************/

class setup3Mode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void setup3Mode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void setup3Mode::enterMode(ModeID oldmode)
{
  id.setYref();
		
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_RE0,6);	
#endif	
}

/****************************************************************************
  stop
****************************************************************************/

class stopMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void stopMode::exitMode(ModeID newmode)
{
  /* poweron, normal, slowspeed, fault1 */
  ERIKA_lcd_clear();
}

void stopMode::enterMode(ModeID oldmode)
{
  ERIKA_PWM_set(0,0);

  if (oldmode == MODE_SETUP3) {
    id.setYwhite();
	
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_MI0,10);	
#endif	
  }
}

/****************************************************************************
  normal
****************************************************************************/

class normalMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void normalMode::exitMode(ModeID newmode)
{
  /* do nothing */
}

void normalMode::enterMode(ModeID oldmode)
{
  sf.setVref(FPdouble(6)/FPdouble(10));
  sf.init();

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0,10);	
#endif	
}

/****************************************************************************
  slowspeed
****************************************************************************/

class slowspeedMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void slowspeedMode::exitMode(ModeID newmode)
{
  /* poweron, stop, normal, fault1 */
}

void slowspeedMode::enterMode(ModeID oldmode)
{
  sf.setVref(FPdouble(3)/FPdouble(10));
  sf.init();

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0,10);	
#endif	
}

/****************************************************************************
  fault1
****************************************************************************/

class fault1Mode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void fault1Mode::exitMode(ModeID newmode)
{
  sf.setAlpha(FPdouble(-20));
  sf.setBeta(FPdouble(-5));

  CancelAlarm(alarm_beeper);
  CancelAlarm(alarm_fault2timeout);

  c3.connectReceiver(&sf_rec2);
}

void fault1Mode::enterMode(ModeID oldmode)
{
#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0,10);	
#endif	

  obs.setVref(FPdouble(15)/FPdouble(100));
  sf.setVref(FPdouble(15)/FPdouble(100));
  sf.init();
  obs.init();

  sf.setAlpha(FPdouble(-30));
  sf.setBeta(FPdouble(-12));

  SetRelAlarm(alarm_beeper, 500,500);
  SetRelAlarm(alarm_fault2timeout, 5000, 0);

  // no need to disconnect receiver, the semantic is a rewrite...
  //c3.disconnectReceiver(&sf_rec2); 
}

/****************************************************************************
  fault2
****************************************************************************/

class fault2Mode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void fault2Mode::exitMode(ModeID newmode)
{
  CancelAlarm(alarm_beeper);
}

void fault2Mode::enterMode(ModeID oldmode)
{
  ERIKA_PWM_set(0,0);

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0,10);	
#endif	

  SetRelAlarm(alarm_beeper, 166,166);
}

/****************************************************************************
  shutdown
****************************************************************************/

class shutdownMode : public Mode {
public:
  virtual void enterMode(ModeID oldmode);
  virtual void exitMode(ModeID newmode);
};

void shutdownMode::exitMode(ModeID newmode)
{
}

void shutdownMode::enterMode(ModeID oldmode)
{
  CancelAlarm(alarm_thread0); // 6 ms

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


#if 0
void mode_change(ERIKA_UINT8 m)
{
  ERIKA_UINT8 oldmode;
  if (m != mode && (mode_valid_transactions[mode] | m)) {
    /* this is a valid mode where we can jump to */

    /* leave the current mode to mode m */
    mode_jump_exit[mode](m);

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
#endif



static InputPort<EvtID,1> mm_port;
static RewriteReceiver<EvtID> mm_receiver;
ModeManager<MAX_MODES, MAX_EVENTS> mm;

static poweronMode         poweronMode;
static setup1Mode          setup1Mode;
static setup2Mode          setup2Mode;
static setup3Mode          setup3Mode;
static stopMode            stopMode;
static normalMode          normalMode;
static slowspeedMode       slowspeedMode;
static fault1Mode          fault1Mode;
static fault2Mode          fault2Mode;
static shutdownMode        shutdownMode;
static deletefirmwareMode  deletefirmwareMode;

void MODE_init(void)
{
  mm.setInitialMode(MODE_POWERON);
  mm_port.addReceiver(0,&mm_receiver);
  mm.setPort(&mm_port);

  mm.setMode(MODE_POWERON       , &poweronMode);
  mm.setMode(MODE_DELETEFIRMWARE, &deletefirmwareMode);
  mm.setMode(MODE_SETUP1        , &setup1Mode);
  mm.setMode(MODE_SETUP2        , &setup2Mode);
  mm.setMode(MODE_SETUP3        , &setup3Mode);
  mm.setMode(MODE_STOP          , &stopMode);
  mm.setMode(MODE_NORMAL        , &normalMode);
  mm.setMode(MODE_SLOWSPEED     , &slowspeedMode);
  mm.setMode(MODE_FAULT1        , &fault1Mode);
  mm.setMode(MODE_FAULT2        , &fault2Mode);
  mm.setMode(MODE_SHUTDOWN      , &shutdownMode);


  mm.ignoreEvent(EVT_BTN_RUN);

  mm.setTransition(MODE_POWERON  , EVT_START        , MODE_SETUP1);
  
  mm.setTransition(MODE_SETUP1   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_SETUP1   , EVT_BTN_RUN      , MODE_SETUP2);

  mm.setTransition(MODE_SETUP2   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_SETUP2   , EVT_BTN_RUN      , MODE_SETUP3);

  mm.setTransition(MODE_SETUP3   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_SETUP3   , EVT_BTN_RUN      , MODE_STOP);

  mm.setTransition(MODE_STOP     , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_STOP     , EVT_CMD_FAST     , MODE_NORMAL);
  mm.setTransition(MODE_STOP     , EVT_CMD_SLOW     , MODE_SLOWSPEED);
  mm.setTransition(MODE_STOP     , EVT_CMD_FAULT    , MODE_FAULT1);

  mm.setTransition(MODE_NORMAL   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_NORMAL   , EVT_CMD_STOP     , MODE_STOP);
  mm.setTransition(MODE_NORMAL   , EVT_CMD_SLOW     , MODE_SLOWSPEED);
  mm.setTransition(MODE_NORMAL   , EVT_CMD_FAULT    , MODE_FAULT1);

  mm.setTransition(MODE_SLOWSPEED, EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_SLOWSPEED, EVT_CMD_STOP     , MODE_STOP);
  mm.setTransition(MODE_SLOWSPEED, EVT_CMD_FAST     , MODE_NORMAL);
  mm.setTransition(MODE_SLOWSPEED, EVT_CMD_FAULT    , MODE_FAULT1);

  mm.setTransition(MODE_FAULT1   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);
  mm.setTransition(MODE_FAULT1   , EVT_CMD_RECOVERY , MODE_SLOWSPEED);
  mm.setTransition(MODE_FAULT1   , EVT_ALARM_TIMEOUT, MODE_FAULT2);
  mm.setTransition(MODE_FAULT1   , EVT_CMD_FAULT2   , MODE_FAULT2);

  mm.setTransition(MODE_FAULT2   , EVT_BTN_ONOFF    , MODE_SHUTDOWN);

  mm.setTransition(MODE_SHUTDOWN , EVT_BTN_ONOFF    , MODE_POWERON);
  mm.setTransition(MODE_SHUTDOWN , EVT_BTN_RUN      , MODE_DELETEFIRMWARE);

  mm.init();

  evt_post(EVT_START);
}

static EvtID mydummyEvt;

void evt_post(EvtID m)
{
  int i;

  mydummyEvt = m;
  mm_receiver.put(mydummyEvt, i);
  mm.fire();
}
