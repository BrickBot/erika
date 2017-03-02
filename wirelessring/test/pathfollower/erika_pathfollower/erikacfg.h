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
 */

#ifndef __SRPTTEST_CONFIG_H__
#define	__SRPTTEST_CONFIG_H__

#include "erikahal.h"  

/*
 * Kernel configuration
 */

/* number of threads */
#define THREAD_MAX 3

/* if 0 <= TID <= (PERIODIC_THREAD_MAX-1) then TID is a periodic thread */
#define TID_thread0 0    /*  periodic */
#define TID_thread1 1    /* not periodic, activated by thread0 */
#define TID_thread2 2    /* activated by serial interrupt */

/* MUTEX definitions */

#define MUTEX_MAX  1
#define MUTEX_sync 0


#ifdef __TIMER_A_SUPPORT__
    #define TIMER_A_PERIOD  400  /* 400tick = 0.8 ms  */
#endif


/****************************************************************************
  Alarms & counters
****************************************************************************/

#define counter0 0

// alarm for periodic thread
#define alarm_thread0 0

// alarm used for the beeper
#define alarm_beeper 1

// timeout from fault1 to fault2
#define alarm_fault2timeout 2

	
#endif /* __SRPTTEST_CONFIG_H__ */
