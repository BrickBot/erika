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
 * CVS: $Id: code.hpp,v 1.2 2003/11/11 13:53:54 pj Exp $
 */

#ifndef __CODE_H__
#define __CODE_H__

#include "osekcpp.hpp"
#include "include/modemanager.hpp"

#define MAX_MODES  11
#define MAX_EVENTS 10

/****************************************************************************
  Global objects
****************************************************************************/

//typedef ModeManager<MAX_MODES, MAX_EVENTS> myModeManager;

extern class osek_core osek_core;
extern class simple_osek_task _thread1;
extern class simple_osek_task _thread0;
//extern myModeManager mm;

extern ModeManager<MAX_MODES, MAX_EVENTS> mm;

CPPDeclareTask(thread0);
CPPDeclareTask(thread1);
CPPDeclareTask(thread2);


/****************************************************************************
  Global objects
****************************************************************************/

#include "include/FPdouble.hpp"
#include "include/rewritereceiver.hpp"
#include "control_new.hpp"

void GBL_init(void);

//Dichiarazioni canali
extern Channel<ERIKA_INT16> c0, c1,c6,c7;
extern Channel<FPdouble> c2, c3, c4,c5;

    
//Dichiarazione primo attore (emissione dati)
//che arrivano su c0 e c1
//ActorInput<ERIKA_INT16,2> source(dbg,cin);    
extern OutputPort<ERIKA_INT16> source_o1;
extern OutputPort<ERIKA_INT16> source_o2;

//Dichiarazione secondo attore (decoder)
extern ActorInputDecoder<ERIKA_INT16,FPdouble> id;
extern InputPort<ERIKA_INT16,1> id_i1;
extern InputPort<ERIKA_INT16,1> id_i2;
extern OutputPort<FPdouble> id_o1;
extern OutputPort<FPdouble> id_o2;
extern RewriteReceiver<ERIKA_INT16> id_rec1;
extern RewriteReceiver<ERIKA_INT16> id_rec2;

extern ActorStateFeedback<FPdouble> sf;
extern RewriteReceiver<FPdouble> sf_rec1;
extern InputPort<FPdouble,1> sf_i1;

extern RewriteReceiver<FPdouble> sf_rec2;
extern InputPort<FPdouble,1> sf_i2;

extern OutputPort<FPdouble> sf_o1;
extern OutputPort<FPdouble> sf_o2;
extern OutputPort<FPdouble> sf_o3;

extern ActorObserver<FPdouble> obs;
extern RewriteReceiver<FPdouble> obs_rec1;
extern RewriteReceiver<FPdouble> obs_rec2;
extern InputPort<FPdouble,1> obs_i1;
extern InputPort<FPdouble,1> obs_i2;
extern OutputPort<FPdouble> obs_o1;
extern Channel<FPdouble> c_obsout;
extern Channel<FPdouble> c_sf;


extern ActorOutputEncoder<FPdouble,ERIKA_INT16> oe;
extern InputPort<FPdouble,1> oe_i1;
extern InputPort<FPdouble,1> oe_i2;
extern OutputPort<ERIKA_INT16> oe_o1;
extern OutputPort<ERIKA_INT16> oe_o2;
extern RewriteReceiver<FPdouble> oe_rec1;
extern RewriteReceiver<FPdouble> oe_rec2;

extern InputPort<ERIKA_INT16,1> ou_i1;
extern InputPort<ERIKA_INT16,1> ou_i2;
extern RewriteReceiver<ERIKA_INT16> ou_rec1;
extern RewriteReceiver<ERIKA_INT16> ou_rec2;

extern ActorMotorOutput<ERIKA_INT16> ou;

#ifdef ACTORDEBUG
extern ActorDebugTheta<FPdouble> ddd;
extern InputPort<FPdouble,1> ddd_p_obs;
extern InputPort<FPdouble,1> ddd_p_st;
extern RewriteReceiver<FPdouble> ddd_r_obs;
extern RewriteReceiver<FPdouble> ddd_r_st;
#endif

/****************************************************************************
  Sensors offset and noises
****************************************************************************/

/* Used for light noise reduction (0: very sensible, 1, .., 10: insensible) */
#define LIGHT_TOLERANCE 0
/* light sensors have different ranges so I use this offset */
#define OFFSET_ERR (20>>LIGHT_TOLERANCE)

/* Davide's  application had 50 (0.2 m/s emergency) and 110 (0.4 m/s normal) */
/* Michael's application had 40 (0.2 m/s emergency) and  80 (0.4 m/s normal) */
#define EMRGSPEED_POWREF 50
#define SLOWSPEED_POWREF 60
#define NORMSPEED_POWREF 110


/****************************************************************************
  Application modes
****************************************************************************/

/*
  POWERON   - all is off
  SETUP1    - waiting for the black sensor reading
  SETUP2    - waiting for the gray sensor reading
  SETUP3    - waiting for the white sensor reading 
  STOP      - speed=0
  NORMAL    - two sensors mode, normal speed
  SLOWSPEED - two sensors mode, medium speed
  FAULT1    - one sensor mode, slow speed, beeper = on_low
  FAULT2    - speed = 0, beeper = on_high
*/
  

/* mode constants */
#define MODE_POWERON         0
#define MODE_SETUP1          1
#define MODE_SETUP2          2
#define MODE_SETUP3          3
#define MODE_STOP            4
#define MODE_NORMAL          5
#define MODE_SLOWSPEED       6
#define MODE_FAULT1          7
#define MODE_FAULT2          8
#define MODE_SHUTDOWN        9
#define MODE_DELETEFIRMWARE 10

//void mode_change(ERIKA_UINT8 m);


/****************************************************************************
  Application events
****************************************************************************/


/* event constants */
#define EVT_START         0
#define EVT_CMD_STOP      1
#define EVT_CMD_SLOW      2
#define EVT_CMD_FAST      3
#define EVT_CMD_FAULT     4
#define EVT_CMD_FAULT2    5
#define EVT_CMD_RECOVERY  6
#define EVT_BTN_ONOFF     7
#define EVT_BTN_RUN       8
#define EVT_ALARM_TIMEOUT 9



/****************************************************************************
  IR commands
****************************************************************************/

#define CMD_STOP     0
#define CMD_SLOW     1
#define CMD_FAST     2
#define CMD_FAULT    3
#define CMD_FAULT2   4
#define CMD_RECOVERY 5

/****************************************************************************
  Interfaces
****************************************************************************/

extern "C" {


  /* Peripheral Initialization part */
  void PER_init(void);

  /* get the latest command received on the serial line */
  ERIKA_UINT8 PER_get_last_command(void);
}

  /* Control part */

  void CTRL_init(void);

  /* change the reference speed in normal mode */
  void CTRL_set_normalpowref(ERIKA_INT16 value);

  /* called when a sensor data is ready */
  /* applylaw and readsensors must be executed in mutual exclusion */
  void CTRL_readsensors(ERIKA_INT16 a, ERIKA_INT16 b);
  void CTRL_convertsensors(void);

  /* the two control laws */
  void CTRL_feedback_law(void);
  void CTRL_observer_law(void);

  /* these three functions are used during the setup phase to get the
     reference values */
  void CTRL_set_black(void);
  void CTRL_set_yref(void);
  void CTRL_set_ywhite(void);

  /* these functions returns the controlled values considering saturation */
  ERIKA_INT16 CTRL_get_lmotor(void);
  ERIKA_INT16 CTRL_get_rmotor(void);

void MODE_init(void);
void evt_post(EvtID m);

template <typename T> void wait(T c)
{
  ERIKA_lcd_write_num(c);
  while (!ERIKA_button_pressed(VIEW_BUTTON));
  while (ERIKA_button_pressed(VIEW_BUTTON));
}

#endif
