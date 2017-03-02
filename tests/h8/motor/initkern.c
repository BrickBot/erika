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
 * CVS: $Id: initkern.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erikacfg.h"
#include "erika.h"

/*
 * ROM kernel descriptors
 */
     
    const ERIKA_TYPEPRIO ERIKA_th_ready_prio[THREAD_MAX] = {
	0x01,
	0x02
    };

    const ERIKA_TYPEPRIO ERIKA_th_dispatch_prio[THREAD_MAX] = {
	0x01,
	0x02	
    };

/*
 * RAM kernel descriptors
 */

#ifdef __MULTI__
ERIKA_TYPESTATUS ERIKA_th_status[THREAD_MAX] = {
        ERIKA_READY,
        ERIKA_READY
    };
#endif

ERIKA_TYPENACT ERIKA_th_nact[THREAD_MAX] = {
	0,
	0
    };

ERIKA_TID ERIKA_th_next[THREAD_MAX] = {
	ERIKA_NIL,
	ERIKA_NIL
    };

ERIKA_TID ERIKA_rqfirst = ERIKA_NIL; 

ERIKA_TID ERIKA_stkfirst = ERIKA_NIL; 

// the ceiling of the first task;
ERIKA_TYPEPRIO ERIKA_sys_ceiling= 0x0000; 


/* ROM */
const ERIKA_TYPEPRIO ERIKA_mutex_ceiling[MUTEX_MAX]= {0x02};

/* RAM */
ERIKA_TYPEPRIO ERIKA_mutex_oldceiling[MUTEX_MAX]; 



