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
 * CVS: $Id: code.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "code.h"
#include "sem/sem.h"

/* Simple Semaphore test:
   - The first semaphore is initialized using the static initializer
   - The second semaphore is initialized through a macro

  The test is composed by four threads:
  - A "noise" thread that is activated at every timer interrupt
  - Three threads that implements a (1)producer/(2)consumer protocol

  Depending on the presence of a #define CASE2 the threads have a
  different behavior:

  In particular, thread1 blocks on semP only when thread2 has been
  enqueued on it, to test the case in which the post operation of
  thread1 wakes up a thread with priority less than the system
  ceiling.

  A typical trace of case 2 is: (priority of th1 > th2 > th3)
  th1:W(P), P(P), W(B) (th1 blocks), 
  th2:W(V), P(P), W(V) (th2 blocks),
  th3:P(B) (th1 woken up),
  th1:W(P), P(P) (th2 woken up), W(B) (th1 blocks, switch to th2!)
*/

#define CASE2


ERIKA_SEM semP = {1,ERIKA_NIL,ERIKA_NIL};
ERIKA_SEM semV;

#ifdef CASE2
ERIKA_SEM semB = {0,ERIKA_NIL,ERIKA_NIL};
#endif

int items     = 0;
int produced1 = 0;
int consumed1 = 0;
int consumed2 = 0;

/* Noise task */
void thread0(void)
{
  int i;
  for (i=0; i<1000; i++);

#ifdef __LCD_USED__
  ERIKA_LCD_display(0); 
#endif
}

/* Producer */
void thread1(void)
{
  for (;;) {
	  ERIKA_sem_wait(&semP);
	  items++;
	  produced1++;
	  ERIKA_sem_post(&semV);

#ifdef CASE2
	  ERIKA_sem_wait(&semB);
#endif
#ifdef __LCD_USED__
	  ERIKA_LCD_display(1);
#endif	  
  }
}

/* Consumer 1 */
void thread2(void)
{
  for (;;) {

	  ERIKA_sem_wait(&semV);
	  items--;
	  consumed1++;
	  ERIKA_sem_post(&semP);
#ifdef __LCD_USED__
	  ERIKA_LCD_display(2);
#endif
  }
}

/* Consumer 2 */
void thread3(void)
{
  for (;;) {

#ifdef CASE2
	  ERIKA_sem_post(&semB);
#else
	  ERIKA_sem_wait(&semV);
	  items--;
	  consumed2++;
	  ERIKA_sem_post(&semP);
#endif

#ifdef __LCD_USED__
	  ERIKA_lcd_write_num(3);
#endif
  }
}

void my_init(void);

void ERIKA_dummy(void)
{
  ERIKA_sem_init(semV,0);
  
  my_init();
  
  ERIKA_thread_make_ready(1,1);
  ERIKA_thread_make_ready(2,1);
  ERIKA_thread_make_ready(3,1);
  ERIKA_sys_scheduler();
  
  for (;;) {
    // Background activities, none in this test
    ERIKA_sys_idle();
  }
}

