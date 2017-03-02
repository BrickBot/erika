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
 * CVS: $Id: inithal.c,v 1.1 2003/05/27 10:49:06 pj Exp $
 */

#include "erikacfg.h"
#include "erikahal.h"
#include "code.h"


#ifdef __MULTI__

    /* stack start from bottom of RAM 0x8000-0xEF2F (28464 bytes)*/
    #define empty_stack 0xEF30
    
    /* IMPORTANT: if you need more than 256 stacks(?!), hal must be modified! */
    #define STACKID ERIKA_UINT8
    
    /* number of used stacks */
    #define STACK_MAX 5  
    
    /* stack size (in bytes) of each stack */
    #define stack1_size  128 
    #define stack2_size  128
    #define stack3_size  128
    #define stack4_size  128
    #define stack5_size  128 /* last size could be used for checking stack overflow */

    /* NOTE:
     * maximum size of application (file .srec) < (0x6F00 - amout of stack used)
     * Look at this when you set stack configuration.
     */
	
    /* initial sp of each stack */
    #define stack1_empty empty_stack
    #define stack2_empty stack1_empty-stack1_size
    #define stack3_empty stack2_empty-stack2_size
    #define stack4_empty stack3_empty-stack3_size
    #define stack5_empty stack4_empty-stack4_size
    
    /* id of each stack */
    #define stack1_id 0
    #define stack2_id 1
    #define stack3_id 2
    #define stack4_id 3
    #define stack5_id 4
#endif


    /* ip of each thread body (ROM) */
    const ERIKA_ADDR ERIKA_h8_thread_body[THREAD_MAX] = {
	(ERIKA_ADDR) thread0 ,
	(ERIKA_ADDR) thread1 ,
	(ERIKA_ADDR) thread2 	
    };

#ifdef __MULTI__
    /* each thread is assigned a stack_id (ROM) */
    const STACKID ERIKA_h8_thread_stack_id[THREAD_MAX+1] = { 
	stack1_id,        /* ! stackid of dummy stack   ! */
        stack2_id,        /* ! stackid of thread0 stack ! */
	stack3_id,        /* ! stackid of thread1 stack ! */
	stack4_id,        /* ! stackid of thread1 stack ! */	
    };    

    /* sp of each stack (RAM)*/
    ERIKA_ADDR ERIKA_h8_sp[STACK_MAX] = {    
	(ERIKA_ADDR) stack1_empty,   /* sp of stack1 */
	(ERIKA_ADDR) stack2_empty,   /* sp of stack2 */
	(ERIKA_ADDR) stack3_empty,   /* sp of stack3 */
	(ERIKA_ADDR) stack4_empty,   /* sp of stack4 */
	(ERIKA_ADDR) stack5_empty    /* sp of stack5 */
    };

    /* stack id for stack of running thread (RAM) */
    STACKID ERIKA_h8_active_stack_id;  /* it is initialized by start.s */
#endif
