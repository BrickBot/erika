/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: erikacfg.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __CABS_CONFIG_H__
#define	__CABS_CONFIG_H__

#if defined(__H8__) && defined(__TIMER_A_SUPPORT__)
// 65000 ticks = 130 ms
#define TIMER_A_PERIOD 65000
#endif

/* application dependent */

#define THREAD_MAX 3
#define MUTEX_MAX  1


#endif
