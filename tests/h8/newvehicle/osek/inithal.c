/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 */

#include "erikacfg.h"
#include "erikahal.h"
#include "code.h"

void Functhread0(void);
void Functhread1(void);
void Functhread2(void);

/* ip of each thread body (ROM) */
const ERIKA_ADDR ERIKA_h8_thread_body[THREAD_MAX] = {
  (ERIKA_ADDR) Functhread0,
  (ERIKA_ADDR) Functhread1,
  (ERIKA_ADDR) Functhread2
};
