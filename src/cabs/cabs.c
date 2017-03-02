/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: cabs.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "cabs/cabs.h"

#ifndef __PRIVATE_CAB_INIT__
void ERIKA_cab_init(const struct ERIKA_cab_ROM_desc *c)
{
  ERIKA_CAB_INDEX i;
  
  for (i=0; i<c->n_msg-1; i++) {
    c->freestk[i] = i;
    c->used[i] = 0;
  } 
}
#endif

#ifndef __PRIVATE_CAB_RESERVE__
void ERIKA_cab_reserve(const struct ERIKA_cab_ROM_desc *c, void **msg, ERIKA_CAB_INDEX *msg_num)
{
  ERIKA_CAB_INDEX i;

  ERIKA_hal_begin_primitive();
    
  i = --(c->ram_desc->free);

  *msg = (void *)(c->msgs + i*c->dim_msg);
  *msg_num = c->freestk[i];

  ERIKA_hal_end_primitive();
}
#endif

#ifndef __PRIVATE_CAB_PUTMES__
void ERIKA_cab_putmes(const struct ERIKA_cab_ROM_desc *c, ERIKA_CAB_INDEX msg_num)
{
  ERIKA_CAB_INDEX old;

  ERIKA_hal_begin_primitive();

  old = c->ram_desc->mrd;

  if (!c->used[old]) {
    c->freestk[c->ram_desc->free++] = old;
  }
  
  c->ram_desc->mrd = msg_num;

  ERIKA_hal_end_primitive();
}
#endif

#ifndef __PRIVATE_CAB_GETMES__
void ERIKA_cab_getmes(const struct ERIKA_cab_ROM_desc *c, void **msg, ERIKA_CAB_INDEX *msg_num)
{
  ERIKA_CAB_INDEX mrd;

  ERIKA_hal_begin_primitive();

  mrd = c->ram_desc->mrd;
  c->used[mrd]++;

  *msg = (void *)(c->msgs + mrd*c->dim_msg);
  *msg_num = mrd;

  ERIKA_hal_end_primitive();
}
#endif

#ifndef __PRIVATE_CAB_UNGET__
void ERIKA_cab_unget(const struct ERIKA_cab_ROM_desc *c, ERIKA_CAB_INDEX msg_num)
{
  ERIKA_hal_begin_primitive();

  c->used[msg_num]--;

  if (!c->used[msg_num] && msg_num != c->ram_desc->mrd) {
    c->freestk[c->ram_desc->free++] = msg_num;
  }

  ERIKA_hal_end_primitive();
}
#endif
