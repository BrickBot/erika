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
 * CVS: $Id: kernopt.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __INCLUDE_H8_KERNOPT_H__
#define __INCLUDE_H8_KERNOPT_H__

/* this file is included by fp/kern.h */


/********************
 * H8 Declarations
 ********************/

#ifdef __FP__

/* #define __PRIVATE_SYS_PANIC__ */
/* #define __PRIVATE_SYS_REBOOT__ */
/* #define __PRIVATE_SYS_IDLE__ */
/* #define __PRIVATE_SYS_SCHEDULER__ */
/* #define __PRIVATE_THREAD_MAKE_READY__ */
/* #define __PRIVATE_THREAD_ACTIVATE__ */
/* #define __PRIVATE_THREAD_END_INSTANCE__ */
/* #define __PRIVATE_MUTEX_LOCK__ */
/* #define __PRIVATE_MUTEX_UNLOCK__ */
/* #define __PRIVATE_IRQ_MAKE_READY__ */
/* #define __PRIVATE_IRQ_END_INSTANCE__ */
/* #define __PRIVATE_SYS_GETTIME__ */

    #define ERIKA_sys_panic			ERIKA_fp_sys_panic
    #define ERIKA_sys_reboot  			ERIKA_fp_sys_reboot
    #define ERIKA_sys_idle   			ERIKA_fp_sys_idle
    #define ERIKA_sys_scheduler 		ERIKA_fp_sys_scheduler
    #define ERIKA_thread_make_ready(t,n) 	ERIKA_fp_thread_make_ready(t,n)
    #define ERIKA_thread_activate(t)		ERIKA_fp_thread_activate(t)
    #define ERIKA_mutex_lock(s) 		ERIKA_fp_mutex_lock(s)
    #define ERIKA_mutex_unlock(s) 		ERIKA_fp_mutex_unlock(s)
    #define ERIKA_IRQ_make_ready(t,n)     	ERIKA_fp_IRQ_make_ready(t,n)

    #ifdef __TIME_SUPPORT__
	#define ERIKA_sys_gettime()		ERIKA_fp_sys_gettime()
    #endif
#endif /* __FP__ */

#endif /* __INCLUDE_H8_KERNOPT_H__ */
