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
 * CVS: $Id: func_d.c,v 1.8 2004/07/13 12:19:11 cirinei Exp $
 */

#include "erikacfg.h"
#include "erika.h"
#include "h8/drivers.h"

/***************************
 *  DATA 		   *
 ***************************/

/* data for nested primitives */
ERIKA_UINT16 ERIKA_nested_primitive_counter=0;

/* data for nested irq */
/* It can assume only two values: 0 or 1 (since nested irq not allowed) */
ERIKA_WORD ERIKA_IRQ_nesting_level=0;
 
/* data for handling sound */
#ifdef __PLAY_BUFFER_SUPPORT__
    ERIKA_UINT8  play_mode, note_num, playing, note_next, note2play, note_length, pause_length;    
    struct ERIKA_note_type * buffer_addr;
#endif /* __PLAY_BUFFER_SUPPORT__ */

#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_UINT8 play_note_state;
#endif  /* __PLAY_NOTE_SUPPORT__ */

/* data for handling motors */
#if defined(__MOTOR_A_VP_USED__)
    #define A_float_on  0x00
    #define A_go0_on	0x40
    #define A_go1_on	0x80
    #define A_brake_on	0xC0
    static ERIKA_UINT8 motorA_period_on[4]=     
	{ A_float_on, A_go0_on, A_go1_on, A_brake_on};

    #define A_float_off	0x00
    #define A_go0_off	0xC0
    #define A_go1_off	0xC0
    #define A_brake_off	0x00
    static ERIKA_UINT8 motorA_period_off[4]=     
	{ A_float_off, A_go0_off, A_go1_off, A_brake_off};
#endif

#if defined(__MOTOR_B_VP_USED__) || defined(__MOTOR_B_PVP_USED__)
    #define B_float_on	0x00
    #define B_go0_on	0x04
    #define B_go1_on	0x08
    #define B_brake_on	0x0C
    static ERIKA_UINT8 motorB_period_on[4]=     
	{ B_float_on, B_go0_on, B_go1_on, B_brake_on };    
    #define B_float_off 0x00
    #define B_go0_off	0x0C
    #define B_go1_off	0x0C
    #define B_brake_off	0x00
    static ERIKA_UINT8 motorB_period_off[4]=     
	{ B_float_off, B_go0_off, B_go1_off, B_brake_off}; 
#endif

#if defined(__MOTOR_C_VP_USED__)
    #define C_float_on	0x00
    #define C_go0_on	0x01
    #define C_go1_on	0x02
    #define C_brake_on	0x03
    static ERIKA_UINT8 motorC_period_on[4]=     
	{ C_float_on, C_go0_on, C_go1_on, C_brake_on };        

    #define C_float_off 0x00
    #define C_go0_off	0x03
    #define C_go1_off	0x03
    #define C_brake_off 0x00
    static ERIKA_UINT8 motorC_period_off[4]=     
	{ C_float_off, C_go0_off, C_go1_off, C_brake_off}; 
#endif

#ifdef  __ERIKA_PWM_USED__
#ifndef __IR_SUPPORT__    
    ERIKA_UINT8 ERIKA_PWM_ovi1_value=0, 	
		ERIKA_PWM_cmi1a_value=0, 
		ERIKA_PWM_cmi1b_value=0;
#else
    ERIKA_UINT8 ERIKA_PWM_ovi0_value=0, 	
		ERIKA_PWM_cmi0a_value=0, 
		ERIKA_PWM_cmi0b_value=0;
#endif

    #define MIN_DUTY_CYCLE 1    
    
    
    ERIKA_UINT8 temp, tempA, tempB;
    
    void ERIKA_PWM_set(ERIKA_UINT8 duty_cycleA,
    				   ERIKA_UINT8 duty_cycleC 
				   #ifdef __ERIKA_PWM_FAST__
				   )
				   #else
				   , ERIKA_UINT8 polarityA,  ERIKA_UINT8 polarityC)
				   #endif
				   
				   
    {
	#ifdef __ERIKA_PWM_FAST__
	
	// two handler raises simultaneously but handler cmi1b is pending
	// until cmi1a handler ends: this is not a problem, becouse
	// handler's execution time is shorter than 8 bit timer increment (4us)
	// and so that delay can be negleted.
	ERIKA_interrupt_off();
#ifndef	__IR_SUPPORT__
	ERIKA_t81_set_cora(duty_cycleA);
	ERIKA_t81_set_corb(duty_cycleC);
#else
	ERIKA_t80_set_cora(duty_cycleA);
	ERIKA_t80_set_corb(duty_cycleC);
#endif
	ERIKA_interrupt_on();	    	
	#else	
	// prepare value than handlers will use to set output ports register		
	if (duty_cycleA>duty_cycleC)
	{   	    
	    #ifdef __ERIKA_PWM_TOFF_BRAKE__	    
	    tempA = 0xC3;		// A off C off
	    tempB = polarityA | 0x03;	// A on  C off
	    #else
	    tempA = 0x00;		// A off C off
	    tempB = polarityA | 0x00;	// A on  C off
	    #endif
	}
	else
	if (duty_cycleA<duty_cycleC)
	{
	    #ifdef __ERIKA_PWM_TOFF_BRAKE__
	    tempA = polarityC | 0xC0;	// A off C on 
	    tempB = 0xC3;		// A off C off
	    #else
	    tempA = polarityC | 0x00;	// A off C on
	    tempB = 0x00;		// A off C off
	    #endif
	}	
	else
	{
	    #ifdef __ERIKA_PWM_TOFF_BRAKE__
	    tempA = 0xC3;	// A off C off
	    tempB = 0xC3;	// A off C off
	    #else
	    tempA = 0x00;	// A off C off
	    tempB = 0x00;	// A off C off
	    #endif
	}		
	#endif
			
	// prepare ERIKA_PWM_ovi_value
        if (duty_cycleA<MIN_DUTY_CYCLE) 
	{
	    // port A must not be powered
	    if (duty_cycleC<MIN_DUTY_CYCLE)
		// port C unpowered
		#ifdef __ERIKA_PWM_TOFF_BRAKE__
		    #ifdef __ERIKA_PWM_FAST__		
	    		temp = 0xC3; 
		    #else
			temp = 0xC3;
		    #endif
		#else
		    #ifdef __ERIKA_PWM_FAST__			
			temp = 0x00;
		    #else
			temp = 0x00;
		    #endif
		#endif	    		
	    else
		// portC powered
		#ifdef __ERIKA_PWM_TOFF_BRAKE__		
    		    #ifdef __ERIKA_PWM_FAST__			
			temp = 0xC2; 
		    #else
	    		temp = (0xC0 | polarityC); 
		    #endif
		#else
    		    #ifdef __ERIKA_PWM_FAST__			
			temp = 0x02; 
		    #else		
		        temp = polarityC;
		    #endif
		#endif	    		
	}
	else
	{
	    // port A powered
	    if (duty_cycleC<MIN_DUTY_CYCLE)
		// portC unpowered
		#ifdef __ERIKA_PWM_TOFF_BRAKE__		
    		    #ifdef __ERIKA_PWM_FAST__			
			temp = 0x83; 
		    #else
	    		temp = (0x03 | polarityA); 
		    #endif
		#else
    		    #ifdef __ERIKA_PWM_FAST__			
			temp = 0x80; 
		    #else		
		        temp = polarityA;
		    #endif
		#endif	    		
	    else
		// portC powered
	        #ifdef __ERIKA_PWM_FAST__			
		    temp = 0x82; 
		#else
	    	    temp = (polarityA | polarityC); 
		#endif		
	}

	ERIKA_interrupt_off();
	// set duty-cycle
#ifndef	__IR_SUPPORT__
	ERIKA_t81_set_cora(duty_cycleA);
	ERIKA_t81_set_corb(duty_cycleC);	    
	// set values to write
	ERIKA_PWM_cmi1a_value=tempA;
	ERIKA_PWM_cmi1b_value=tempB;	
	ERIKA_PWM_ovi1_value=temp;
#else
	ERIKA_t80_set_cora(duty_cycleA);
	ERIKA_t80_set_corb(duty_cycleC);	    
	// set values to write
	ERIKA_PWM_cmi0a_value=tempA;
	ERIKA_PWM_cmi0b_value=tempB;	
	ERIKA_PWM_ovi0_value=temp;
#endif
        ERIKA_interrupt_on();	    	
    }
#endif

/****************************
 *   FUNCTIONS              *
 ****************************/

/* utility function */
ERIKA_UINT8 ERIKA_button_pressed(ERIKA_UINT8 button)
{								
  switch (button)
  {
    case RUN_BUTTON   : return !(Port4 & 0x04); break;// bit2 (=0: pressed)
    case ONOFF_BUTTON : return !(Port4 & 0x02); break;// bit1
    case VIEW_BUTTON  : return !(Port7 & 0x40); break;// bit 6
    case PRGM_BUTTON  : return !(Port7 & 0x80); break;// bit 7
    default:
       return 0;
  }  
}


void ERIKA_lcd_write_num(ERIKA_INT16 num)
{   
    ERIKA_UINT16 p6_ddr;
    ERIKA_UINT8 p6;

    p6_ddr = Port6DdrShadow;
    p6 = Port6;
    rom_func3 ((ERIKA_ADDR) 0x1ff2,0x3001,num,0); /* output "num". see ROM func. 0x1ff2 */
    rom_func0 ((ERIKA_ADDR) 0x27c8);              /* refresh (needed) */  
    Port6Ddr=Port6DdrShadow=p6_ddr;
    Port6=p6;
    
}

void ERIKA_lcd_clear(void)
{
    ERIKA_UINT16 p6_ddr;
    ERIKA_UINT8 p6;
    p6_ddr = Port6DdrShadow;
    p6 = Port6;
    rom_func0((ERIKA_ADDR) 0x27AC);               	/* lcd clear. see ROM func. 0x27AC */
    rom_func0((ERIKA_ADDR) 0x27C8);			/* refresh (needed) */
    Port6Ddr=Port6DdrShadow=p6_ddr;
    Port6=p6;    
}


/****************************
 * PERIODIC TASK ACTIVATION *
 ****************************/
/* Removed from here. Now that is application dependent */



#ifdef __TIMER_A_SUPPORT__
     
    void ERIKA_timerA_handler(void)
    {
	ERIKA_UINT8 i; 		    

	/******************************* 
	 * set next handler activation 
         *******************************/	
	ERIKA_t16_set_ocra(ERIKA_t16_get_ocra() + TIMER_A_PERIOD); /* next match */        
	ERIKA_t16_reset_ocra_match_flag();        
    
	#ifdef __TIMER_A_USER_FUNCTION_USED__
	/************************
         * user defined function 
	 ************************/		
	    
	    ERIKA_timerA_user_function();
	    
	#endif /* __TIMER_A_USER_FUNCTION_USED__ */
	
    }
#endif /* __TIMER_A_SUPPORT__ */


#ifdef __TIMER_B_SUPPORT__

    void ERIKA_timerB_handler(void)  /* 16bit timer matchB handler */
    {
     /* this handler runs every 2000us (T16OCRB matchs FRC). Its purposes are:
       - handling motors (duty cicle)
       - handling sound (generated by output of 8bit timer0)
     */   

        ERIKA_UINT8 i;

	#ifdef __MOTOR_VP_SUPPORT__
	    ERIKA_UINT8  mask;       // used for handling motors
	    static ERIKA_UINT8 x = 0; // used for handling motors	    
	#endif    
    

	/******************************* 
	 * set next handler activation 
         *******************************/
	ERIKA_t16_reset_ocrb_match_flag();
	ERIKA_t16_set_ocrb(ERIKA_t16_get_ocrb() + TIMER_B_PERIOD); /* (next match) */    

	#ifdef __MOTOR_VP_SUPPORT__     /* some PVP or VP motors is used */
	/************************************************
         * motors with (precise) variable power handling 
         ************************************************/
        
	    mask = MotorDevReg;
    
	    #ifdef __MOTOR_A_VP_USED__	
		mask &=0x3F;
		if (x<motorA_state.power)
		    mask |= motorA_period_on[motorA_state.action];
//    		#ifdef __MOTOR_A_PVP_USED__	
		else
		    /* when == motor is keeped float to avoid rough movement */
		    if (x>motorA_state.power)
    			mask |= motorA_period_off[motorA_state.action];
//    		#endif
	    #endif

	    #ifdef __MOTOR_B_VP_USED__	
		mask &=0xF3;
		if (x<motorB_state.power)
		    mask |= motorB_period_on[motorB_state.action];
//    		#ifdef __MOTOR_B_PVP_USED__	
		else
		    /* when == motor is keeped float to avoid rough movement */
		    if (x>motorB_state.power)		  
    			mask |= motorB_period_off[motorB_state.action];
//    		#endif
	    #endif
    
	    #ifdef __MOTOR_C_VP_USED__	
    		mask &=0xFC;
		if (x<motorC_state.power)
		    mask |= motorC_period_on[motorC_state.action];
//    		#ifdef __MOTOR_C_PVP_USED__	
		else
		    /* when == motor is keeped float to avoid rough movement */
		    if (x>motorC_state.power)
		  	mask |= motorC_period_off[motorC_state.action];
//    		#endif
	    #endif    

	    x++;    
	    if (x >= VP_MOTOR_PERIOD)
    		x = 0;        
	
	    MotorDevReg=mask;

	#endif /* __MOTOR_VP_SUPPORT__ */
    

	/******** ********* 
	* sound handling 
	*****************/
	#ifdef __PLAY_BUFFER_SUPPORT__
    
	    if (playing)
		for (;;)
		{
		    if (note_length>0)
		    {
			note_length--;
			break;
		    }
		    if (pause_length>0)
		    {
			pause_length--;
			ERIKA_t80_stop();
			break;
		    }
		    if (note_next>=note_num)
		    {
			playing=play_mode;
			note_next=0;
			ERIKA_t80_stop();
			
			break;
		    }

		    note2play=buffer_addr[note_next].n;
		    note_length=buffer_addr[note_next].n_len;
		    pause_length=buffer_addr[note_next].p_len;	    

		    ERIKA_t80_set_cora(note2play);
		    ERIKA_t80_set_corb(note2play>>1); /* duty cycle 50% */
		    ERIKA_t80_start();		
	    	
		    note_next++;	
		}	    
        
	#endif /* __PLAY_BUFFER_SUPPORT__ */

	#ifdef __PLAY_NOTE_SUPPORT__     	
	    if (play_note_state>0)
		play_note_state--;
	    else 
		ERIKA_t80_stop();
	#endif /* __PLAY_NOTE_SUPPORT__ */

	#ifdef __IR_SUPPORT__
	/***********************
	* timeout for ir transmission
	***********************/
	    ERIKA_IR_timer();
	#endif
	
	#ifdef __IL_SUPPORT__
	/***********************
	* timer for periodic send of Interaction Layer
	***********************/
	    ERIKA_IL_timer();
	#endif
	
	#ifdef __TIMER_B_USER_FUNCTION_USED__
	/***********************
	* user defined function 
	***********************/
    
	    /* call user function */
	    ERIKA_timerB_user_function();

	#endif /* __TIMER_B_USER_FUNCTION_USED__ */    
    }
    
#endif /* __TIMER_B_SUPPORT__ */


#ifdef __PLAY_BUFFER_SUPPORT__
    void ERIKA_play_buffer (struct ERIKA_note_type *  b_addr, ERIKA_UINT8 n_note, ERIKA_UINT8 mode)
    {    
	if (!playing)  /* wait that precedent buffer ends before accept another buffer */
	{
	    buffer_addr=(struct ERIKA_note_type *) b_addr;	/* buffer that contains notes */    
	    note_num=n_note; 	 				/* number of notes to play    */
	
	    play_mode=mode;				/* 0: once, 1: continuos (background) */
	    note_next=0;
	    note_length=0;
	    pause_length=0;
	    playing=1; 		
	}
    }
#endif /* __PLAY_BUFFER_SUPPORT__ */


#ifdef __PLAY_NOTE_SUPPORT__
    void ERIKA_play_note(ERIKA_UINT8 note, ERIKA_UINT8 length)
    { 
	if (play_note_state==0)
	{
	    play_note_state=length;
	    ERIKA_t80_set_cora(note);
	    ERIKA_t80_set_corb(note>>1); /* 50% duty cycle */
	    ERIKA_t80_start();
	}
    }
#endif /* __PLAY_NOTE_SUPPORT__ */
