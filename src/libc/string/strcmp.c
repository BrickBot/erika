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
 * CVS: $Id: strcmp.c,v 1.1 2003/05/27 10:48:13 pj Exp $
 */

#ifndef __PRIVATE_LIBC_STRCMP__
int strcmp(const char *s1, const char *s2)
{
  for (; *s1==*s2; s1++,s2++)
    if (!*s1) return 0;

  return (*(unsigned const char *)s1)-(*(unsigned const char *)s2);
}
#endif
