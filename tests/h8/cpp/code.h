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
 * CVS: $Id: code.h,v 1.1 2003/05/27 10:49:06 pj Exp $
 */

#include "erika.h"		

typedef void THREAD;
typedef void HANDLER;

#if defined(__cplusplus)
extern "C" {
#endif

THREAD thread0(void);
THREAD thread1(void);
THREAD thread2(void);
THREAD thread3(void);

#if defined(__cplusplus)
};
#endif
