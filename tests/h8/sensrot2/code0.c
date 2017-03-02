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
 * CVS: $Id: code0.c,v 1.1 2003/10/20 13:00:58 pj Exp $
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

const ERIKA_UINT16 th_period[PERIODIC_THREAD_MAX]={
  3,   // =6 ms (2ms per cycle)
  1000    // 2000ms
  };

ERIKA_UINT16 remaining_period[PERIODIC_THREAD_MAX];

void wait(void)
{    
    while (!ERIKA_button_pressed(VIEW_BUTTON));
    while (ERIKA_button_pressed(VIEW_BUTTON));
}

ERIKA_UINT16 s2, s1=0, s1_old=0;

ERIKA_UINT16 round=0, prec_round=0, quarter=0, tick=0; // tick is 1/16 of round (22.5 degrees)

ERIKA_UINT16 stat[35]={0,0,0,0,0,
                       0,0,0,0,0,
		       0,0,0,0,0,
		       0,0,0,0,0,
		       0,0,0,0,0,
		       0,0,0,0,0,
		       0,0,0,0,0};

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

    static int i=0;
/*    
    if (i==0)
	ERIKA_play_note(_DO0, 200);
    else 
	ERIKA_play_note(_SI0, 20);
	
    ERIKA_lcd_write_num(stat[i]);
    
    if (++i==35)
	i=0;    
*/

	
//    ERIKA_motorA_power(mot_pow);
    
//    ERIKA_play_note(_DO0,100);    
}


/* ************************
 * aperiodic threads
 * ************************/

THREAD thread2(void)
{ 
//    if (round>=prec_round)
//    {
//	prec_round=round;
//	ERIKA_lcd_write_num(prec_round<<2);    	
//    }
        
//    round=0;
}

HANDLER ADI_handler (void)
{
    int i;
    static ERIKA_UINT16 ref_pos=0, pos=0, prec_pos=0, first=1, conv=0;

    s1=(ERIKA_ad_get_port3()>>10);

//    if (abs(s1-s1_old)>5)
//    {
//	s1_old=s1;
//	ERIKA_lcd_write_num(s1);
//    }
            


//    stat[s1]++;
    
    if (!first)
    {    
	// first time it returns always 5 so we ignore first read 

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
    
	    
	    if (abs(pos-prec_pos)>5)    	    
	    {
		tick++;		    // 1/16 of round is completed
		ERIKA_lcd_write_num(tick);
				
		if (pos==ref_pos)
		{
		    quarter++;		    // 1/4 of round is completed
		    if (quarter==4)
			round++;	    // a round is completed		    
		}
		
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
    ERIKA_ad_select(AD_PORT3_SINGLE_ACT);        

    /* select motor action and power */
    mot_pow=PWM_MIN_MOTOR_POWER; 
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
	if (ERIKA_button_pressed(PRGM_BUTTON))
	    delete_firmware();
	ERIKA_sys_idle();
    }
}
