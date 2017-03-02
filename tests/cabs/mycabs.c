/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2000-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: mycabs.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "cabs/cabs.h"

/* Define_cab(name,number of messages, dimension of the buffer in WORDS ); 
   see also types.h and code.c */

ERIKA_DEFINE_CAB(cab1, 4, 1);
ERIKA_DEFINE_CAB(cab2, 3, 1);
