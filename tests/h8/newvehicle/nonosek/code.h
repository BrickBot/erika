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
 * CVS: $Id: code.h,v 1.1 2003/10/20 12:41:45 pj Exp $
 */

#include "erika.h"		
#include "h8/serial.h"

#ifndef __CODE_H__
#define __CODE_H__

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
  
#define MAX_MODES 10
extern ERIKA_UINT8 mode;

/* mode constants */
#define MODE_POWERON   0
#define MODE_SETUP1    1
#define MODE_SETUP2    2
#define MODE_SETUP3    3
#define MODE_STOP      4
#define MODE_NORMAL    5
#define MODE_SLOWSPEED 6
#define MODE_FAULT1    7
#define MODE_FAULT2    8
#define MODE_SHUTDOWN  9

void mode_change(ERIKA_UINT8 m);

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

/* Peripheral Initialization part */
void PER_init(void);

/* get the latest command received on the serial line */
ERIKA_UINT8 PER_get_last_command(void);


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


/****************************************************************************
  Threads
****************************************************************************/

void thread0(void);
void thread1(void);
void thread2(void);

#endif
