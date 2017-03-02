/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: types.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#ifndef __ALARMS_TYPES_H__
#define __ALARMS_TYPES_H__


/* HAL types */

#if defined(__H8__)

typedef ERIKA_INT16 ERIKA_TID;

typedef ERIKA_INT16  ERIKA_TYPEPRIO;
typedef ERIKA_UINT16 ERIKA_TYPESTATUS;
typedef ERIKA_UINT16 ERIKA_TYPENACT;
typedef ERIKA_UINT16 ERIKA_MUTEX;
typedef ERIKA_UINT16 ERIKA_TYPEENABLE;
typedef ERIKA_UINT16 ERIKA_TYPERELDLINE; 
typedef ERIKA_UINT16 ERIKA_TYPEABSDLINE;

typedef ERIKA_INT8  ERIKA_TYPEALARM;
typedef ERIKA_INT8  ERIKA_TYPECOUNTER;
typedef ERIKA_UINT8 ERIKA_TYPETICK;
typedef ERIKA_INT8  ERIKA_TYPENOTIFY;

#endif



#endif
