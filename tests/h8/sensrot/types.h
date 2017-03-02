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
 * CVS: $Id: types.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __SRPTTEST_TYPES_H__
#define __SRPTTEST_TYPES_H__

/* 
 * HAL types 
 */

/* TID is usually INT32 but it is application dependent*/

typedef ERIKA_INT16 ERIKA_TID;

/* 
 * Kernel types 
 */

typedef ERIKA_INT16  ERIKA_TYPEPRIO;
typedef ERIKA_UINT16 ERIKA_TYPESTATUS;
typedef ERIKA_UINT16 ERIKA_TYPENACT;
typedef ERIKA_UINT16 ERIKA_MUTEX;
typedef ERIKA_UINT16 ERIKA_TYPERELDLINE;
typedef ERIKA_UINT16 ERIKA_TYPEABSDLINE;

#endif
