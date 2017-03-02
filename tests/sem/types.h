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
 * CVS: $Id: types.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __RMTEST_TYPES_H__
#define __RMTEST_TYPES_H__


/* HAL types */

/* from arch.h */

/* TID is usually INT16 and is inserted into the Application dependent types.h */
#define ERIKA_TID ERIKA_INT16


/* These types are defined into the Application dependent file types.h: */
#define ERIKA_TYPEPRIO    ERIKA_INT16
#define ERIKA_TYPESTATUS  ERIKA_UINT8
#define ERIKA_TYPENACT    ERIKA_UINT8  	         
#define ERIKA_MUTEX       ERIKA_UINT16

/* Semaphore types */
#define ERIKA_TYPESEMCOUNT ERIKA_WORD

#endif
