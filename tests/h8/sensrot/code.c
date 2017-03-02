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
 * CVS: $Id: code.c,v 1.2 2003/10/20 15:02:37 cantini Exp $
 */

/* Sensrot test: 
 * connect a motor to port A;
 * connect rotation sensor to port 3;
 * connect motor axe to rotation sensor.
 * Rotation sensor will misur motor velox which is displayed on LCD
 * OnOff Button turn on/off RCX.
 * Note that when RCX is turned off Run button has no effect
 * while when RCX is on pressing Run button causes a beep 
 */

#include "code.h"

const ERIKA_UINT16 th_period[PERIODIC_THREAD_MAX]={
  5,   // =10 ms (2ms per cycle)
  4000 // 8s
  };

ERIKA_UINT16 remaining_period[PERIODIC_THREAD_MAX];

void wait(void)
{    
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
}

ERIKA_UINT16 s2, s1;

ERIKA_UINT16 round=0, prec_round=0;

ERIKA_UINT8 mot_pow;

/* ************************
 *     periodic threads 
 * ************************/


THREAD thread0(void)
{              
    Port6 &= 0xFC;  /* port3 e port2 uses bit 0 and bit 6 of port6 */
    
    ERIKA_ad_start();    
} 


THREAD thread1(void)
{         
    mot_pow +=30;
    round=0;    
    if (mot_pow>VP_MAX_MOTOR_POWER)    
    {
	mot_pow=VP_MIN_MOTOR_POWER;
	prec_round=0;
	ERIKA_lcd_clear();    		
    }	
    ERIKA_motorA_power(mot_pow);
    
    ERIKA_play_note(_DO0,100);    
}


/* ************************
 * aperiodic threads
 * ************************/

THREAD thread2(void)
{ 
    if (round>=prec_round)
    {
	prec_round=round;
	ERIKA_lcd_write_num(prec_round<<2);    	
    }
        
    round=0;
}

HANDLER ADI_handler (void)
{
    int i;
    static ERIKA_UINT16 ref_pos=0, pos, prec_pos, num=0, first=1, num_tick=0,conv=0;

    ERIKA_ad_stop();    

    s1=(ERIKA_ad_get_port3()>>12);
        
    if (!first)
    {    
	/* first time it returns always 5 so we ignore first read */

	if (ref_pos==0)
	{	
	    ref_pos=s1;	// referemet position (every 4 time it appears => 1 round done)
	    pos=s1;	// initial position
	    prec_pos=s1; 
	}
	else
	{
	    prec_pos=pos;
	    pos=s1;	
    
	    if (pos!=prec_pos)
	    {
		num_tick++;		    
		
		if (pos==ref_pos)
		{
		    num++;		// num=0..4
		    if (num==4)
		    {
			num=0;
			round++;
		    }
		    
		}
		
	    }
	
	    conv++;    	
	    /* when 150 AD conversions ended, activate thread 2 */
	    if (conv==150)
	    {
		ERIKA_IRQ_make_ready(TID_thread2,1);
		num_tick=0;
		conv=0;    
		ref_pos=0;		
		first=1;	
	    }
	}
	    
    }
    else
    {
	first=0;
    }
	
    Port6 |= ~0xFC;
    
    ERIKA_ad_reset_end_flag();
}




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

/* call this macro for each handler you need to install */
ERIKA_declare_stub(ONOFFBUTTON_handler);
ERIKA_declare_stub(RUNBUTTON_handler);
ERIKA_declare_stub(ADI_handler);



THREAD ERIKA_dummy(void)
{                 
    /* initialization */
    ERIKA_init();     

    /* external interrupts on falling edge */
    ERIKA_button_irq_mode(ONOFF_EDGE_RUN_EDGE);

    /* select mode for A/D converter */
    ERIKA_ad_select(AD_PORT3_SCAN_ACT);        

    /* select motor action and power */
    mot_pow=VP_MIN_MOTOR_POWER; 
    ERIKA_motorA_action(MotorGo0);
    ERIKA_motorA_power(mot_pow);

    /* install handlers */
    ERIKA_set_handler(ONOFFBUTTON_IRQ, ONOFFBUTTON_handler);   
    ERIKA_set_handler(RUNBUTTON_IRQ, RUNBUTTON_handler);
    ERIKA_set_handler(ADI_IRQ, ADI_handler);
    
    /* enable external interrupt sources */ 
    ERIKA_enable_irq(ONOFFBUTTON_IRQ);  
    ERIKA_enable_irq(RUNBUTTON_IRQ);        
    
    /* enable AD interrupt */
    ERIKA_enable_irq(ADI_IRQ);
        
    /* Activate threads and call scheduler */
    thread_activation(); 	         
        
    for (;;)
    {     	    
	/* sleep mode: wait for interrupt (internal or external) */      
	ERIKA_sys_idle();
    }
}
