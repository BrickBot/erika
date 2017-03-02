/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2003 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: stubs.c,v 1.1 2003/10/20 10:35:05 pj Exp $
 */

#include "erika.h"

void __terminate() {
  ERIKA_sys_abort();
}

void* __get_eh_context() {
  return (void*) 0;
}


void __builtin_delete(void* ptr) {
  free(ptr);
}

void *__builtin_new(unsigned int size) {
  return malloc(size);
}

void __pure_virtual ()
{
  ERIKA_sys_abort();
}
