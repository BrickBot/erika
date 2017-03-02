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
 * CVS: $Id: panic.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_SYS_PANIC__
void ERIKA_fp_sys_panic(void)
{
    ERIKA_hal_begin_primitive();
    ERIKA_hal_panic();
    ERIKA_hal_end_primitive();
}
#endif
