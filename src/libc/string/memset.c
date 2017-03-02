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
 * CVS: $Id: memset.c,v 1.1 2003/05/27 10:48:13 pj Exp $
 */

#include <erika.h>

#ifndef __PRIVATE_LIBC_MEMSET__
void *memset(void *s, int c, size_t n)
{
  char *m = (char *)s;
  
  for (; n; n--)
    *m++ = c;

  return s;
}
#endif
