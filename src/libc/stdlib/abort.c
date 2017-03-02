/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * Authors: Paolo Gai <pj@sssup.it>
 *          Alessandro Colantonio <cocciasik@tiscalinet.it>
 *          and others (see the web page below for details)
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001 Paolo Gai
 *
 * This program is distributed under the license contained into the
 * license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: abort.c,v 1.1 2003/05/27 10:49:06 pj Exp $
 */

#include <erika.h>

/* similar to sys_panic :-) */

#ifndef __PRIVATE_LIBC_ABORT__
void abort(void)
{
    ERIKA_hal_begin_primitive();
    ERIKA_hal_panic();
    ERIKA_hal_end_primitive();
}
#endif
