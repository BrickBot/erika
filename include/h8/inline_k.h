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
 * CVS: $Id: inline_k.h,v 1.2 2003/10/20 10:12:33 pj Exp $
 */

#include "erikacfg.h"
#include "h8/arch.h"
#include "h8/drivers.h"

/*******************************
 * H8 hal variables declaration
 *******************************/ 

extern ERIKA_UINT16 ERIKA_nested_primitive_counter;
extern ERIKA_WORD ERIKA_IRQ_nesting_level;

/*******************************
 * H8 hal functions declaration
 *******************************/ 

/*
 * Context handling
 */

void ERIKA_h8_hal_endcycle_ready(ERIKA_TID t);

void ERIKA_h8_hal_endcycle_stacked(ERIKA_TID t);

void ERIKA_h8_hal_ready2stacked(ERIKA_TID t);

void ERIKA_h8_hal_IRQ_ready(ERIKA_TID t);

void ERIKA_h8_hal_IRQ_stacked(ERIKA_TID t);

#ifdef __MULTI__
    void ERIKA_h8_hal_stkchange(ERIKA_TID t);
#endif

/*
 * Utility function
 */

void ERIKA_h8_hal_reboot(void);


/*********************
 * Inline functions
 *********************/

/*
 * Context handling
 */

__INLINE__ void ERIKA_hal_endcycle_ready(ERIKA_TID t)
{
    ERIKA_h8_hal_endcycle_ready(t);
}

__INLINE__ void ERIKA_hal_endcycle_stacked(ERIKA_TID t)
{
    ERIKA_h8_hal_endcycle_stacked(t);
}

__INLINE__ void ERIKA_hal_ready2stacked(ERIKA_TID t)
{
    ERIKA_h8_hal_ready2stacked(t);
}

__INLINE__ void ERIKA_hal_IRQ_ready(ERIKA_TID t)
{
    ERIKA_h8_hal_IRQ_ready(t);
}

__INLINE__ void ERIKA_hal_IRQ_stacked(ERIKA_TID t)
{
    ERIKA_h8_hal_IRQ_stacked(t);
}

#ifdef __MULTI__
    __INLINE__ void ERIKA_hal_stkchange(ERIKA_TID t)
    {
	ERIKA_h8_hal_stkchange(t);
    }
#endif


/*
 * Primitive definition
 */

__INLINE__ void ERIKA_hal_begin_primitive(void)
{  
    /* disable IRQ (I=1) */
    __asm__(".align 1 ");
    __asm__("orc #0x80, ccr ");
           
}

__INLINE__ void ERIKA_hal_end_primitive(void)
{
    /* enable IRQ (I=0) */  
    __asm__(".align 1");
    __asm__("andc #0x7F, ccr");	    
}

__INLINE__ void ERIKA_hal_IRQ_begin_primitive(void)
{
     // nothing to do! (nested irq not supported)
}

__INLINE__ void ERIKA_hal_IRQ_end_primitive(void)
{
     // nothing to do! (nested irq not supported)
}

__INLINE__ ERIKA_FREG ERIKA_hal_begin_nested_primitive(void)
{
  register ERIKA_UINT8 f;
  __asm__(".align 1");
  __asm__("stc	ccr, %0l" :  "=r"(f));
  __asm__("orc 	#0x80, ccr");
  return f;
 
}

__INLINE__ void ERIKA_hal_end_nested_primitive(ERIKA_FREG f)
{
  __asm__(".align 1");
  __asm__("ldc	%0l, ccr" : : "r"(f));
}

/*
 * Interrupt handling
 */

__INLINE__ void ERIKA_hal_enableIRQ(void)
{
    /* enable IRQ (I=0) */  
    __asm__(".align 1");
    __asm__("andc #0x7F, ccr");
            
}

__INLINE__ void ERIKA_hal_disableIRQ(void)
{
    /* disable IRQ (I=1) */
    __asm__(".align 1");
    __asm__("orc #0x80, ccr");
            
}

__INLINE__ void ERIKA_hal_IRQ_enableIRQ(void)
{
    // nothing to do! (since nested irq not present)
}
__INLINE__ void ERIKA_hal_IRQ_disableIRQ(void)
{
    // nothing to do! (since nested irq not present)
}

/*
 * Utility function
 */

__INLINE__ void ERIKA_hal_idle(void)
{
  /* cpu enters in power down state.								*
   * Two possible states are possible, depending on the bit 7 of SYSCR:				*
   * - 0: go in sleep mode;									*
   * - 1: go in software standby mode.         							*
   * In software standby mode clock system is halted and settling time (~8ms) is needed 	*
   * to recover from: this can happen only with an external interrupt (ON/OFF,RUN button).   	* 
   * Sleep mode is less power saving but exit from it is possible also with internal		*
   * interrupt and no settling time is needed.  						*
   * So be sure about the value of that bit before calling this primitive.	             	*/

    SYSCR &= 0x7F;    // sleep mode   
    __asm__(".align 1");
    __asm__("andc #0x7F, ccr  ;; enable interrupt before sleep or... sleep forever!!!! ");
    __asm__("sleep");	    
}

__INLINE__ void ERIKA_hal_reboot(void)
{
   ERIKA_h8_hal_reboot();
}

__INLINE__ void ERIKA_hal_panic(void)
{
    ERIKA_h8_hal_reboot();
}

#ifdef __TIME_SUPPORT__
__INLINE__ ERIKA_TIME ERIKA_hal_gettime(void)
{
    return T16FRC;  
}
#endif

__INLINE__ ERIKA_WORD ERIKA_hal_get_IRQ_nesting_level(void)
{
    return ERIKA_IRQ_nesting_level;
}

