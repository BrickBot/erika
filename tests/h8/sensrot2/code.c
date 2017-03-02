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
 * CVS: $Id: code.c,v 1.1 2003/10/20 13:00:58 pj Exp $
 */


/* Sensrot test: 
 *
 * Release 1.0
 *
 * connect a motor to port A;
 * connect rotation sensor to port 3;
 * connect motor axe to rotation sensor.
 * Rotation sensor will misur motor speed which is displayed on LCD
 * OnOff Button turn on/off RCX.
 * Note that when RCX is turned off Run button has no effect
 * while when RCX is on pressing Run button causes a beep 
 */

/* Release 2.0 
 * 
 * It's needed that sampling is done every T ms just to guarantee that
 * T is less than the time needed for the rotation sensor to make a 16th of round.
 * That condition should not be a limitation if the sensor is connected to an RCX motor
 * which can reach a rotation speed of 6 rps: so it takes 1/96 s > 10 ms to do a 16th of a round.
 * So we can choose any  T < 10 ms, let's say T=4ms
 */

#include "code.h"

ERIKA_UINT8 state=0;

const ERIKA_UINT16 th_period[PERIODIC_THREAD_MAX]={
  2,   // =4 ms (2ms per cycle)
  500    // 1s
  };

ERIKA_UINT16 remaining_period[PERIODIC_THREAD_MAX];

void wait(void)
{    
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
}

ERIKA_UINT16 s2, s1=0, s1_old=0, ready_to_convert=1;

ERIKA_UINT16 round=0, num_ad_conv=0, tick=0; // tick is 1/16 of round (22.5 degrees)

ERIKA_UINT8 mot_pow;

/* ************************
 *     periodic threads 
 * ************************/


THREAD thread0(void)
{              
    Port6 &= 0xFC;  	// port3 e port2 use bit0 and bit6 of port6: disable before convertion
    
    if (ready_to_convert)
    {
	ready_to_convert=0;
        ERIKA_ad_start();  
    }  
} 


THREAD thread1(void)
{         

   ERIKA_UINT16 timer;
   if (state==0)
   {
    /* halt motors and show results */
	ERIKA_play_note(_DO0, 20);
//	ERIKA_play_note(_DO0, 50);
	state=99; /* pressing run button will set state=1 */
	/* stop motors */
	ERIKA_PWM_set(0,0 
			#ifndef __ERIKA_PWM_FAST__ 
			,ERIKA_PWM_PORT_A_PWR00, ERIKA_PWM_PORT_C_PWR00 
			#endif 
			); /* brake motors */
	
	/* tick/2 is the number of tick during a state interval 
	 * In particular we obtain the average value for first and 
	 * second speed detection. A previous state is needed to
	 * let the sistem reach correct speed. */
	ERIKA_lcd_write_num(tick>>1);       			
   }
   else
   if (state==1)
   {
	/* do nothing: just to obtain a longer stop interval */
	state=2;
   }
   if (state==2)
   {
    /* show current motor power and start motors */
	ERIKA_play_note(_DO1, 10);
	state=3;	              
	ERIKA_PWM_set(mot_pow, mot_pow 
				    #ifdef __ERIKA_PWM_FAST__ 
				    #else
				    ,ERIKA_PWM_PORT_A_PWR10, ERIKA_PWM_PORT_C_PWR10 
				    #endif
				    );	
	ERIKA_lcd_write_num(mot_pow);		
   }
   else
   if (state==3)
   {
    /* first speed detection */		
	tick=0;
	state=4;
   }
   else
   if (state==4)
   {
    /* second (and last) speed detection */			
	state=0;
	/* set new speed for next period */
	if (mot_pow==255)
    	    mot_pow=0;	
	else   	
	if (mot_pow>=250)
	    mot_pow=255;
	else	    
	    mot_pow += 10;		
   }   	      
}


/* ************************
 * aperiodic threads
 * ************************/

THREAD thread2(void)
{ 
}

HANDLER ADI_handler (void)
{
    static ERIKA_UINT16 prev_pos=0;

    s1=(ERIKA_ad_get_port3()>>10);    
    
    if (num_ad_conv==0)
	/* fisrt convertion returns always 0 => ignored */
    	num_ad_conv++;
    else
    if (num_ad_conv==1)
    {
	/* second convertion gives the actual position */
	num_ad_conv++;	
	prev_pos=s1;
    }
    else	
	if (abs(s1-prev_pos)>5)    	    
	{
	    prev_pos=s1;	

	    tick++;		    		// 1/16 of round is completed
	    //ERIKA_lcd_write_num(tick);	    			
	    if ((tick & 0x000F)==0)             // tick is multiple than 16 so a round is completed
	    {
		//tick=0;			// you can choose to reset tick or not....
		round++;	    	
//		ERIKA_play_note(_DO1, 30);
	    }					        			    
	}	

    Port6 |= ~0xFC;			// active sensors need to be powered         
    ERIKA_ad_reset_end_flag();

    ready_to_convert=1;
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
  
    /* thread0 and thread 1 are periodic */
    for (i=0; i<PERIODIC_THREAD_MAX; i++)
	remaining_period[i]=th_period[i];

    /* Insert thread0 and thread1 into ready queue, 
     * while thread2 will be inserted by RUNButton_handler */
    ERIKA_thread_make_ready(0, 1);
    ERIKA_thread_make_ready(1, 1); 
    
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
    ERIKA_ad_select(AD_PORT3_SINGLE_ACT);        

    /* select motor action and power */
    mot_pow=30; 

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
    
    // Try to define __MOTOR_B_FP_USED__ when you use PWM in fast mode:
    // motor B will not run. Instead, if you don't use fast mode 
    // (that is, don't define __ERIKA_PWM_FAST__) motor B will run
    #ifdef __MOTOR_B_FP_USED__        
    ERIKA_motorB_action(MotorGo0);
    #endif
    for (;;)
    {     	    
	/* sleep mode: wait for interrupt (internal or external) */      
	ERIKA_sys_idle();
    }
}
