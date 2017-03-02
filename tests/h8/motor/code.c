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
 * CVS: $Id: code.c,v 1.2 2003/10/20 15:02:36 cantini Exp $
 */


/* Motor test:
 * it shows increasing power for motorA and motorC (VP control). 
 * Display shows motorA power.
 * Hint: modify code just to switch motorA power and motorB
 *       power by pressing a button.
 */
  
#include "code.h"

#define PERIODIC_THREAD_MAX 1

/* used by first_thread_activation() */
const ERIKA_UINT16 th_period[PERIODIC_THREAD_MAX]={
  4000 // 8000 ms
  };

ERIKA_UINT16 remaining_period[PERIODIC_THREAD_MAX];
 
ERIKA_UINT8 mA_pow, mC_pow;

void wait(void)
{    
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
}

THREAD thread0(void)
{          
    ERIKA_play_note(_DO0, 10);

    ERIKA_motorA_power(mA_pow);
    ERIKA_motorC_power(mC_pow);    	
        
    ERIKA_lcd_write_num(mA_pow);

    mA_pow += 1;
    mC_pow += 1;
    
    if (mA_pow>VP_MAX_MOTOR_POWER)
	mA_pow=VP_MIN_MOTOR_POWER;

    if (mC_pow>VP_MAX_MOTOR_POWER)
	mC_pow=VP_MIN_MOTOR_POWER;	
} 

THREAD thread1(void)         
{                     
    ERIKA_motorA_action(MotorGo0);
    ERIKA_motorC_action(MotorGo1);        
}


/* call this macro for each handler you need to install */
ERIKA_declare_stub(ONOFF_handler);

void ERIKA_timerB_user_function(void)
{
  ERIKA_UINT8 i;

	for (i=0; i<PERIODIC_THREAD_MAX; i++)
    	{
	    remaining_period[i]--;	/* periods of threads are multiple of timer period */	
 
	    if (remaining_period[i]==0)
	    {				
		remaining_period[i]=th_period[i];
	    		
		ERIKA_IRQ_make_ready(i, 1);   // N.B. this function is called into an IRQ!!!! 
	    }	 
	}
}

void thread_activation(void)
{
    ERIKA_UINT8 i;

    /* used by first_thread_activation() */
  
    for (i=0; i<PERIODIC_THREAD_MAX; i++)
	remaining_period[i]=th_period[i];

    for (i=0; i<THREAD_MAX; i++)	
	ERIKA_thread_make_ready(i, 1);      
 
   /* call scheduler (Note: after this call, I=0 (Interrupt On) */    
    ERIKA_sys_scheduler();        
}

THREAD ERIKA_dummy(void)
{                 
    /* initialization */
    ERIKA_init();     

    /* external interrupts generated once when you press the button (and not *
     * for all the time you are are pressing the button)                     */
    ERIKA_button_irq_mode(ONOFF_EDGE_RUN_EDGE);

    /* set velox of motors */
    mA_pow=VP_MIN_MOTOR_POWER;    
    mC_pow=VP_MIN_MOTOR_POWER;        

    /* set handler */
    ERIKA_set_handler(ONOFFBUTTON_IRQ, ONOFF_handler);   
    
    /* enable external interrupt sources */
    ERIKA_enable_irq(ONOFFBUTTON_IRQ);  
        
    /* activate threads and call scheduler */
    thread_activation(); 
	                          
    for (;;)
    {     	    
	/* sleep mode: wait for interrupt (internal or external) */      
	ERIKA_sys_idle();
        while (!ERIKA_button_pressed(PRGM_BUTTON));	    
        delete_firmware();	
    }    
}
