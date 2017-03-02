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
 * CVS: $Id: malloc.c,v 1.2 2003/10/20 10:27:48 pj Exp $
 */

#include <erika.h>

#ifndef __PRIVATE_LIBC_MALLOC__

#define MALLOC_BUFFER_SIZE 32
int mallocbuffer[MALLOC_BUFFER_SIZE];
int mallocpointer = MALLOC_BUFFER_SIZE;

void *malloc(size_t size)
{
  size >>= 1;

  if (mallocpointer > size) {
    mallocpointer -= size;
    return (void *)&mallocbuffer[mallocpointer];
  }
  else {
#ifdef __LCD_USED__
    ERIKA_LCD_display(1111);
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
#endif
    ERIKA_sys_panic();
  }
  
  return 0;
}
#endif
