/*
 * Project: S.Ha.R.K.
 *
 * Coordinators: 
 *   Giorgio Buttazzo    <giorgio@sssup.it>
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *
 * Authors     : 
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *   (see the web pages for full authors list)
 *
 * ReTiS Lab (Scuola Superiore S.Anna - Pisa - Italy)
 *
 * http://www.sssup.it
 * http://retis.sssup.it
 * http://shark.sssup.it
 */
/*
 * Copyright (C) 2000 Paolo Gai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "kernel/kern.h"
#include "modules/edf.h"
#include "modules/cbs.h"
#include "modules/rr.h"
#include "modules/dummy.h"

#include "modules/sem.h"
#include "modules/hartport.h"
#include "modules/cabs.h"

#include "drivers/mouse.h"
#include "drivers/keyb.h"


/*+ sysyem tick in us +*/
#define TICK 0

/*+ RR tick in us +*/
#define RRTICK 10000

TIME __kernel_register_levels__(void *arg)
{
  struct multiboot_info *mb = (struct multiboot_info *)arg;

  EDF_register_level(EDF_ENABLE_ALL);
  CBS_register_level(CBS_ENABLE_ALL, 0);
  RR_register_level(RRTICK, RR_MAIN_YES, mb);
  dummy_register_level();

  SEM_register_module();

  CABS_register_module();

  return TICK;
}

TASK __init__(void *arg)
{
  struct multiboot_info *mb = (struct multiboot_info *)arg;

  KEYB_PARMS kparms = BASE_KEYB;
  MOUSE_PARMS mparms = BASE_MOUSE;
  
  HARTPORT_init();

  keyb_def_ctrlC(kparms, NULL);
  keyb_def_map(kparms,itaMap);
  KEYB_init(&kparms);
  mouse_def_ps2(mparms);
  mouse_init(&mparms);

  __call_main__(mb);

  return (void *)0;
}

