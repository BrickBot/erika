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
 * CVS: $Id: myobject.hpp,v 1.2 2003/10/20 10:39:11 pj Exp $
 */

#include <erika.h>

#ifndef __MYOBJECT__
#define __MYOBJECT__

class pippo {
public:
  virtual void pluto(void) = 0;
};

class myobject : public pippo {
  int i;
public:
  myobject(int par) : i(par) {};

  void print1(void);
  virtual void print2(void);
  virtual void pluto(void);
};

#endif
