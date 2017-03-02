/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Michele Cirinei
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
#define THREAD_MAX 2

/* if 0 <= TID <= (PERIODIC_THREAD_MAX-1) then TID is a periodic thread */
#define TID_thread0 0    /* periodic */
#define TID_thread1 1
#define counter0 0
#define alarm_tx 0
#define alarm_rx 1
#endif /* __SRPTTEST_CONFIG_H__ */
