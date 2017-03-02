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
 * CVS: $Id: arch.h,v 1.3 2003/10/20 10:16:55 pj Exp $
 */


#ifndef __INCLUDE_H8_ARCH_H__
#define __INCLUDE_H8_ARCH_H__


#include "erikacfg.h"

/*
 * Compiler dependent interface
 */

/* inline definitions */

#ifdef __NO_INLINE__
#define __INLINE__ static
#else
#define __INLINE__ static inline
#endif


/***************************
 * 	HAL Constants
 ***************************/
 
/* invalid pointer */
#define NULL 0


/***************************
 * 	HAL Types
 ***************************/

/* Primitive data types */
typedef unsigned char 	ERIKA_BIT;
typedef unsigned char 	ERIKA_UINT8;
typedef unsigned short 	ERIKA_UINT16;
typedef signed char 	ERIKA_INT8;
typedef signed short 	ERIKA_INT16;
typedef unsigned short	ERIKA_WORD;
typedef unsigned short	ERIKA_FREG;

/* Thread IDs */
/* TID is application dependent! */

/* Addresses (that have the same size of a pointer) */
typedef ERIKA_UINT16 	* ERIKA_ADDR;

/* IRQs */
typedef ERIKA_UINT16 	ERIKA_TYPEIRQ;

#ifdef __TIME_SUPPORT__
    /* Timer size */
    typedef ERIKA_UINT16 ERIKA_TIME;
#endif

/***************************
 * 	LIBC Types
 ***************************/

#ifdef __LIBC__
typedef unsigned int size_t;
#endif


/***********************************************
 * 	Application dependent data types
 ***********************************************/

#include "types.h"

/***************************
 * 	HAL Variables
 ***************************/

/* Thread function body pointer */
extern const ERIKA_ADDR ERIKA_h8_thread_body[];

#ifdef __MULTI__

    /* Stack pointer of each stack */
    extern ERIKA_ADDR ERIKA_h8_sp[];

    /* Index of the stack used by the task (ROM)*/
    extern const ERIKA_UINT8 ERIKA_h8_thread_stack_id[];

    /* Index of the stack used by running task */
    extern ERIKA_UINT8 ERIKA_h8_active_stack_id;
    
#endif /* __MULTI__ */


/**************************** 
 * 	HAL Functions
 ****************************/

/*
 * Context handling
 */
/* typically called at the end of a thread instance */
__INLINE__ void ERIKA_hal_endcycle_ready(ERIKA_TID t);
__INLINE__ void ERIKA_hal_endcycle_stacked(ERIKA_TID t);

/* typically called into a generic primitive to implement preemption */
__INLINE__ void ERIKA_hal_ready2stacked(ERIKA_TID t);

/* typically called at the end of an interrupt */
__INLINE__ void ERIKA_hal_IRQ_ready(ERIKA_TID t);
__INLINE__ void ERIKA_hal_IRQ_stacked(ERIKA_TID t);

#ifdef __MULTI__
/* called to change the active stack, typically used with locking semaphores */
    __INLINE__ void ERIKA_hal_stkchange(ERIKA_TID t);    
#endif /* __MULTI__ */


/*
 * Generic Primitives
 */

/* called as _first_ function of a generic primitive */
__INLINE__ void ERIKA_hal_begin_primitive(void);

/* called as _last_ function of a generic primitive */
__INLINE__ void ERIKA_hal_end_primitive(void);

/* called as _first_ function of a primitive called into an IRQ handler */
__INLINE__ void ERIKA_hal_IRQ_begin_primitive(void);

/* called as _last_ function of a primitive called into an IRQ handler */
__INLINE__ void ERIKA_hal_IRQ_end_primitive(void);

/* called as _first_ function of a primitive that can be called into
   an IRQ and into a task */
__INLINE__ ERIKA_FREG ERIKA_hal_begin_nested_primitive(void);

/* called as _last_ function of a primitive that can be called into
   an IRQ and into a task */
__INLINE__ void ERIKA_hal_end_nested_primitive(ERIKA_FREG f);

/*
 * Interrupt handling
 */

/* Disable/Enable Interrupts */
__INLINE__ void ERIKA_hal_enableIRQ(void);
__INLINE__ void ERIKA_hal_disableIRQ(void);

/* Disable/Enable interrupts when into an IRQ Routine */
__INLINE__ void ERIKA_hal_IRQ_enableIRQ(void);
__INLINE__ void ERIKA_hal_IRQ_disableIRQ(void);


/*
 * Utility function
 */

__INLINE__ void ERIKA_hal_reboot(void);

__INLINE__ void ERIKA_hal_idle(void);

__INLINE__ void ERIKA_hal_panic(void);

#ifdef __TIME_SUPPORT__
/* read current time (used by EDF scheduler) */
    __INLINE__ ERIKA_TIME ERIKA_hal_gettime(void);          
#endif

/*
 * Nested Interrupts Handling 
 */

//#ifdef __ALLOW_NESTED_IRQ__
__INLINE__ ERIKA_WORD ERIKA_hal_get_IRQ_nesting_level(void);
//#endif

#endif 
