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
 * CVS: $Id: rq_exchg.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_RQ2STK_EXCHANGE__
ERIKA_TID ERIKA_rq2stk_exchange(void)
{
  ERIKA_TID temp;

  temp = ERIKA_rqfirst;
  
  // extract the first task from the ready queue
  ERIKA_rqfirst = ERIKA_th_next[temp]; 
  // insert the extracted task on the topo of the stack
  ERIKA_th_next[temp] = ERIKA_stkfirst;
  ERIKA_stkfirst = temp;

  return temp;
}
#endif
