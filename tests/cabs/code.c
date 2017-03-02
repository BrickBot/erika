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
 * CVS: $Id: code.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */


#include "erika.h"
#include "cabs/cabs.h"

ERIKA_EXTERN_CAB(cab1);
ERIKA_EXTERN_CAB(cab2);

ERIKA_WORD tick=1;
ERIKA_WORD result=0;

void thread1(void)
{
  void *msg;
  ERIKA_CAB_INDEX num;

  ERIKA_WORD tick_cab1;
  ERIKA_WORD tick_cab2;
  
  ERIKA_cab_getmes(&cab1, &msg, &num);
  tick_cab1 = *((ERIKA_WORD *)msg);
  ERIKA_cab_unget(&cab1, num);

  ERIKA_cab_getmes(&cab2, &msg, &num);
  tick_cab2 = *((ERIKA_WORD *)msg);
  ERIKA_cab_unget(&cab2, num);

  result = tick_cab1 - tick_cab2;

#ifdef __LCD_USED__
  ERIKA_LCD_display(1);
#endif
}

void thread2(void)
{
  void *msg;
  ERIKA_CAB_INDEX num;
  ERIKA_WORD mytick;
  
  ERIKA_cab_getmes(&cab1, &msg, &num);
  mytick = *((ERIKA_WORD *)msg);
  ERIKA_cab_unget(&cab1, num);

  ERIKA_cab_reserve(&cab2, &msg, &num);
  *((ERIKA_WORD *)msg) = mytick*2;
  ERIKA_cab_putmes(&cab2, num);

#ifdef __LCD_USED__
  ERIKA_LCD_display(2);
#endif
}

void thread3(void)
{
  void *msg;
  ERIKA_CAB_INDEX num;

  ERIKA_cab_reserve(&cab1, &msg, &num);
  *((ERIKA_WORD *)msg) = tick++;
  ERIKA_cab_putmes(&cab1, num);

#ifdef __LCD_USED__
  ERIKA_LCD_display(3);
#endif
}

void ERIKA_dummy(void)
{
  /* Initialize cabs */
  ERIKA_cab_init(&cab1);
  ERIKA_cab_init(&cab2);

#ifdef __H8__
  ERIKA_init();
#endif
  
  for (;;) {
    // Background activities, none in this test
    ERIKA_sys_idle();
  }
}

