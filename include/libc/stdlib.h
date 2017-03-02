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
 * CVS: $Id: stdlib.h,v 1.2 2003/10/20 10:26:42 pj Exp $
 */

#include "erika.h"

#if defined(__cplusplus)
extern "C" {
#endif

  void abort(void);
  
  /* a minimal allocator, that simply allocates without deallocating */
  void *malloc(int p);
  void free(void *p);
  

#if defined(__cplusplus)
};
#endif
