/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *               2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 */

#include "erikacfg.h"
#include "erikahal.h"
#include "code.h"


/* ip of each thread body (ROM) */
const ERIKA_ADDR ERIKA_h8_thread_body[THREAD_MAX] = {
  (ERIKA_ADDR) thread0,
  (ERIKA_ADDR) thread1,
  (ERIKA_ADDR) thread2
};
