/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * Authors: Paolo Gai <pj@sssup.it>
 *          Alessandro Colantonio <cocciasik@tiscalinet.it>
 *          and others (see the web page below for details)
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the license contained into the
 * license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: myobject.cpp,v 1.2 2003/10/20 10:39:11 pj Exp $
 */

#include "myobject.hpp"

void myobject::print1(void)
{
  ERIKA_lcd_write_num(i++); 
}

void myobject::print2(void)
{
  ERIKA_lcd_write_num(i); 
}

void myobject::pluto(void)
{
  ERIKA_lcd_write_num(i); 
}
