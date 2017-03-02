/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * Authors: Paolo Gai <pj@sssup.it>
 *          Alessandro Colantonio <cocciasik@tiscalinet.it>
 *          and others (see the web page below for details)
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the license contained into the
 * license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: initkern.c,v 1.1 2003/05/27 10:49:06 pj Exp $
 */

#include "erikacfg.h"
#include "erika.h"

/*
 * ROM kernel descriptors
 */

/* th_period is expressed in number of timer period, while th_reldline in tick */
    const ERIKA_TYPERELDLINE ERIKA_th_reldline[THREAD_MAX] = {
	  800,  /* 800 tick = 1600us */
          1100,
	  1000 
    };

    const ERIKA_TYPEPRIO ERIKA_th_ready_prio[THREAD_MAX] = {
	0x01,
	0x02,
	0x04
    };

    const ERIKA_TYPEPRIO ERIKA_th_dispatch_prio[THREAD_MAX] = {
	0x01, 
	0x02,
	0x04 
    };

/*
 * RAM kernel descriptors
 */

ERIKA_TYPEABSDLINE ERIKA_th_absdline[THREAD_MAX];

#ifdef __MULTI__
ERIKA_TYPESTATUS ERIKA_th_status[THREAD_MAX] = {
        ERIKA_READY,
        ERIKA_READY,
        ERIKA_READY
    };
#endif

ERIKA_TYPENACT ERIKA_th_nact[THREAD_MAX] = {
	0,
	0,
	0 
    };

ERIKA_TID ERIKA_th_next[THREAD_MAX] = {
	ERIKA_NIL,
	ERIKA_NIL,	
	ERIKA_NIL	
    };

ERIKA_TID ERIKA_rqfirst = ERIKA_NIL; 

ERIKA_TID ERIKA_stkfirst = ERIKA_NIL; 

ERIKA_TYPEPRIO ERIKA_sys_ceiling= 0x0000; // the ceiling of the first task;


/* ROM */
const ERIKA_TYPEPRIO ERIKA_mutex_ceiling[MUTEX_MAX]= {0x04};

/* RAM */
ERIKA_TYPEPRIO ERIKA_mutex_oldceiling[MUTEX_MAX];



