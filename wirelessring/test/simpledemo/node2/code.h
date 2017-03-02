/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: code.h,v 1.1 2004/07/13 13:13:21 cirinei Exp $
 */

#include "erika.h"		
#include "h8/serial.h"

#ifndef __CODE_H__
#define __CODE_H__

/****************************************************************************
  Interfaces
****************************************************************************/

/* Peripheral Initialization part */
void PER_init(void);

void ONOFFbutton_handler();
/****************************************************************************
  Threads
****************************************************************************/

void thread0(void);
void thread1(void);
#endif