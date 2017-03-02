/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: erikacfg.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __CONFIG_H__
#define	__CONFIG_H__

/* HAL configuration */
#ifdef __H8__
#define my_init ERIKA_init
#ifdef __TIMER_A_USER_FUNCTION_USED__
#define TIMER_A_PERIOD 65000 // 65000 ticks = 130 ms
#endif
#endif

/* application dependent */

#define THREAD_MAX 4

#endif
