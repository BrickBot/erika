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
 * CVS: $Id: string.h,v 1.1 2003/05/27 10:46:53 pj Exp $
 */

#include "erika.h"

#if defined(__cplusplus)
extern "C" {
#endif

void *memset(void *s, int c, size_t n);
int strcmp(const char *s1, const char *s2);

#if defined(__cplusplus)
};
#endif
