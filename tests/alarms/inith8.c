/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: inith8.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "code.h"


#ifdef __MULTI__

    /* stack start from bottom of RAM 0x8000-0xEF2F (28464 bytes)*/
    #define empty_stack 0xEF30
    
    /* IMPORTANT: if you need more than 256 stacks(?!), hal must be modified! */
    #define STACKID ERIKA_UINT8
    
    /* number of used stacks */
    #define STACK_MAX 3  
    
    /* stack size (in bytes) of each stack */
    #define stack1_size  512 
    #define stack2_size  512
    #define stack3_size  128
  

    /* NOTE:
     * maximum size of application (file .srec) < (0xEFFF - amout of stack used)
     * Consider this question when you set stack configuration.
     */
	
    /* initial sp of each stack */
    #define stack1_empty empty_stack
    #define stack2_empty stack1_empty-stack1_size
    #define stack3_empty stack2_empty-stack2_size
    
    /* id of each stack */
    #define stack1_id 0
    #define stack2_id 1
    #define stack3_id 2

#endif


    /* ip of each thread body (ROM) */
    const ERIKA_ADDR ERIKA_h8_thread_body[THREAD_MAX] = {
	(ERIKA_ADDR) thread0,
	(ERIKA_ADDR) thread1
    };

#ifdef __MULTI__
    /* each thread is assigned a stack_id (ROM) */
    const STACKID ERIKA_h8_thread_stack_id[THREAD_MAX+1] = { 
	stack3_id,        /* ! stackid of dummy stack   ! */
        stack1_id,        /* ! stackid of thread0 stack ! */
	stack2_id         /* ! stackid of thread1 stack ! */
    };    

    /* sp of each stack (RAM)*/
    ERIKA_ADDR ERIKA_h8_sp[STACK_MAX] = {    
	(ERIKA_ADDR) stack1_empty,   /* sp of stack1 */
	(ERIKA_ADDR) stack2_empty,   /* sp of stack2 */
	(ERIKA_ADDR) stack3_empty    /* sp of stack3 */
    };

    /* stack id for stack of running thread (RAM) */
    STACKID ERIKA_h8_active_stack_id; 
#endif



