/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001,2002 Paolo Gai, Alessandro Colantonio
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: kern.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __INCLUDE_FP_KERN_H__
#define __INCLUDE_FP_KERN_H__

/*************************************************************************
 Kernel Constants
 *************************************************************************/

/* invalid TID */
#define ERIKA_NIL       ((ERIKA_TID)-1)

/* Thread statuses:
   In general, a thread status is needed for two reasons:
   - for debug purposes,

   - and, if used with a Multistack HAL, for storing a flag that let
   the kernel know if the task has some space allocated on its stack.
*/

#if defined(__MULTI__) || defined(__WITH_STATUS__)
#define ERIKA_READY      1
#define ERIKA_STACKED    2

/* used by semaphores and blocking primitives in general */
#define ERIKA_BLOCKED    4

#if defined(__MULTI__)
/* used to know if a task has some space allocated on its stack */
#define ERIKA_WASSTACKED 8
#endif

#endif


/*************************************************************************
 Kernel Types
 *************************************************************************/

/* These types are defined into the Application dependent file types.h:
   ERIKA_TYPEPRIO	priority type
   ERIKA_TYPESTATUS	status type
   ERIKA_TYPENACT	nact type
   ERIKA_MUTEX	        mutex type
   ERIKA_TYPEENABLE     thread abilitation type 
                        (only if __ENABLE_THREAD__ is defined)
*/



/*************************************************************************
 Kernel Variables
 *************************************************************************/

// ROM
extern const ERIKA_TYPEPRIO   ERIKA_th_ready_prio[];	 /* ready priority (for preemption) */
extern const ERIKA_TYPEPRIO   ERIKA_th_dispatch_prio[];  /* dispatch priority (when running) */
#ifndef __FP_NO_MUTEX__
extern const ERIKA_TYPEPRIO   ERIKA_mutex_ceiling[];     /* mutex ceiling */
#endif

// RAM
#if defined(__MULTI__) || defined(__WITH_STATUS__)
extern ERIKA_TYPESTATUS ERIKA_th_status[];	         /* thread status */
#endif

extern ERIKA_TYPENACT   ERIKA_th_nact[];		 /* pending activations */
extern ERIKA_TID        ERIKA_th_next[];		 /* next task in queue */
extern ERIKA_TYPEPRIO   ERIKA_sys_ceiling;		 /* system ceiling */
#ifndef __FP_NO_NUTEX__
extern ERIKA_TYPEPRIO   ERIKA_mutex_oldceiling[];	 /* old mutex ceiling */
#endif
/* The first task into the ready queue */
extern ERIKA_TID ERIKA_rqfirst;

/* The first stacked task */
extern ERIKA_TID ERIKA_stkfirst;

#ifdef __ENABLE_THREAD__
/* an array that contains the abilitation for each thread */
extern ERIKA_TYPEENABLE ERIKA_th_enabled[];
#endif

/*************************************************************************
 Architecture dependent optimizations
 *************************************************************************/

/* kernopt.h declares the __PRIVATE_XXX__ macros that exclude the
   declaration and definition of the optimized primitives for various
   architectures. It should also contain the declaration and
   definition of the inlined functions! */

#if defined(__H8__)
#include "h8/kernopt.h"
#endif

/*************************************************************************
 System functions
 *************************************************************************/

#ifndef __PRIVATE_SYS_PANIC__
void ERIKA_fp_sys_panic(void);
#endif

#ifndef __PRIVATE_SYS_REBOOT__
void ERIKA_fp_sys_reboot(void);
#endif

#ifndef __PRIVATE_SYS_IDLE__
void ERIKA_fp_sys_idle(void);
#endif

#ifndef __PRIVATE_SYS_SCHEDULER__
void ERIKA_fp_sys_scheduler(void);
#endif

#ifndef __PRIVATE_SYS_GETTIME__
#ifdef __TIME_SUPPORT__
ERIKA_TIME ERIKA_fp_sys_gettime();
#endif
#endif


/* Dummy thread is needed for the kernel */
void ERIKA_dummy(void);



/*************************************************************************
 Internal Queue management functions
 *************************************************************************/

/* return the first ready task without extracting it */
#ifndef __PRIVATE_RQ_QUERYFIRST__
__INLINE__ ERIKA_TID ERIKA_rq_queryfirst(void)
{
    return ERIKA_rqfirst;  
}
#endif

/* return the first stacked task (the running task) without extracting it */
#ifndef __PRIVATE_STK_QUERYFIRST__
__INLINE__ ERIKA_TID ERIKA_stk_queryfirst(void)
{
    return ERIKA_stkfirst;  
}
#endif

/* extract the running task from the stack */
#ifndef __PRIVATE_STK_GETFIRST__
__INLINE__ void ERIKA_stk_getfirst(void)
{
    ERIKA_stkfirst = ERIKA_th_next[ERIKA_stkfirst];
}
#endif

/* insert a task into the stack  data structures */
#ifndef __PRIVATE_STK_INSERTFIRST__
__INLINE__ void ERIKA_stk_insertfirst(ERIKA_TID t)
{
    ERIKA_th_next[t] = ERIKA_stkfirst;
    ERIKA_stkfirst = t;
}
#endif

/* insert a task into the ready queue */
#ifndef __PRIVATE_RQ_INSERT__
void ERIKA_rq_insert(ERIKA_TID t);
#endif

/* put the first ready task into the stack */
#ifndef __PRIVATE_RQ2STK_EXCHANGE__
ERIKA_TID ERIKA_rq2stk_exchange(void);
#endif



/*************************************************************************
 Primitives
 *************************************************************************/

/* This primitive activates a thread but it DOES NOT execute the
   preemption test!!! Since FP handles pending activations, a thread
   can be activated more than one time with a single call.
   Usually, nact=1 */
#ifndef __PRIVATE_THREAD_MAKE_READY__
void ERIKA_fp_thread_make_ready(ERIKA_TID t, ERIKA_TYPENACT nact);
#endif

/* This primitive activates a thread once and then execute the
   preemption test */
#ifndef __PRIVATE_THREAD_ACTIVATE__
void ERIKA_fp_thread_activate(ERIKA_TID t);
#endif

/* This call terminates a thread instance. It must be called as the
   LAST function call BEFORE the `}' that ends a thread. If the
   primitive is not inserted at the end of */
#ifndef __PRIVATE_THREAD_END_INSTANCE__
void ERIKA_fp_thread_end_instance(void);
#endif

#ifndef __FP_NO_MUTEX__
/* mutexes are called using symbolic names as parameters. using MUTEX
   values different from these constants give an unspecified behavior. 
   Mutex constants are contained into structs.ini */

/* This primitive implements a SRP mutex lock */
#ifndef __PRIVATE_MUTEX_LOCK__
void ERIKA_fp_mutex_lock(ERIKA_MUTEX m);
#endif

/* This primitive implements a SRP mutex unlock */
#ifndef __PRIVATE_MUTEX_UNLOCK__
void ERIKA_fp_mutex_unlock(ERIKA_MUTEX m);
#endif

#endif

/*************************************************************************
 Primitives that have to be called into an IRQ
 *************************************************************************/

/* This primitive shall be atomic and must be called into an IRQ
 * handler. It works as the thread_make_ready */
#ifndef __PRIVATE_IRQ_MAKE_READY__
void ERIKA_fp_IRQ_make_ready (ERIKA_TID t, ERIKA_TYPENACT nact);
#endif

/* This primitive shall be atomic.
   This primitive shall be inserted as the last function in an IRQ handler.
   If the HAL allow IRQ nesting the end_instance should work as follows:
   - it must implement the preemption test only if it is the last IRQ on the stack
   - if there are other interrupts on the stack the IRQ end_instance should do nothing
*/
#ifndef __PRIVATE_IRQ_END_INSTANCE__
void ERIKA_fp_IRQ_end_instance(void);
#endif

/*************************************************************************
 Primitives that can be used to enable/disable a thread
 *************************************************************************/

/* These calls are used to trigger the abilitation of a thread.
   When a thread is enabled, the thread can be activated as usual.
   When a thread is disabled, all thread activations done with the usual 
     primitievs are disables, e.g., they does nothing.

   The primitives can be considered atomic, so no interrupt disabling
   is needed
*/

#ifdef __ENABLE_THREAD__

#ifndef __PRIVATE_THREAD_ENABLE__
__INLINE__ void ERIKA_fp_thread_enable(ERIKA_TID t)
{
  ERIKA_th_enabled[t] = 1;
}
#endif

#ifndef __PRIVATE_THREAD_DISABLE__
__INLINE__ void ERIKA_fp_thread_disable(ERIKA_TID t)
{
  ERIKA_th_enabled[t] = 0;
}
#endif

#ifndef __PRIVATE_THREAD_ISENABLED__
__INLINE__ ERIKA_TYPEENABLE ERIKA_fp_thread_isenabled(ERIKA_TID t)
{
  return ERIKA_th_enabled[t];
}
#endif

#endif

#endif
