/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2000-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: erika.h,v 1.2 2003/05/27 10:46:53 pj Exp $
 */

#ifndef __INCLUDE_KERNEL_H__
#define __INCLUDE_KERNEL_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "erikacfg.h"
#include "erikahal.h"

/* Fixed priority with SRP */
#ifdef __FP__
#include "fp/kern.h"
#endif

#if defined(__cplusplus)
};
#endif

#endif
