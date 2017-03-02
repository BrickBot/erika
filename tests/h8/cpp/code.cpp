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
 * CVS: $Id: code.cpp,v 1.4 2003/10/20 10:39:11 pj Exp $
 */

/*
 * C++ test
 * this test simply compiles a C++ class and calls its methods
 */

#include "code.h"
#include "myobject.hpp"

void wait(void)
{    
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
}

THREAD thread0(void) {} 
THREAD thread1(void) {}
THREAD thread2(void) {}

template<class T>
void swap(T & o1, T & o2) {
        T tmp = o1;
        o1 = o2;
        o2=tmp;
};

//class Exc {
//public:
//  int _num;
//  Exc(int i):_num(i) {};
//};

//class A {
//public:
//  A() {throw Exc(1);};
//};


volatile int i,j;

myobject test(10);

THREAD ERIKA_dummy(void)
{
  /* initialization */
  ERIKA_init();     

  ERIKA_lcd_write_num(2222); 
  wait();


  test.print1();
  wait();
  test.print2();
  wait();

  swap(i,j);

  /* exceptions currently require support for malloc, free, memset, strcmp */
  //  int j;
  //  try {
  //    A a;
  //  } catch (Exc & e) {
  //    j = e._num;
  //  };


  for (;;) {     	    
    /* sleep mode: do nothing... */      
    ERIKA_sys_idle();
  }
}
