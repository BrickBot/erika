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
 * CVS: $Id: erikacfg.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __SRPTTEST_CONFIG_H__
#define	__SRPTTEST_CONFIG_H__

//#include "erikahal.h"  
/*
 * Kernel configuration
 */

/* number of threads */
#define THREAD_MAX 2

/* number of periodic threads */
// #define PERIODIC_THREAD_MAX 1 // Now it is defined into code.h

/* if 0 <= TID <= (PERIODIC_THREAD_MAX-1) then TID is a periodic thread */
#define TID_thread0 0    /*  periodic */
#define TID_thread1 1    /* not periodic */

/* MUTEX definitions */

#define MUTEX_MAX  1
#define MUTEX_sync 0


#ifdef __TIMER_A_SUPPORT__
    #define TIMER_A_PERIOD  400  /* 400tick = 0.8 ms  */
#endif

	
#endif /* __SRPTTEST_CONFIG_H__ */
