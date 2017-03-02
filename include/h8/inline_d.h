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
 * CVS: $Id: inline_d.h,v 1.8 2004/05/24 09:09:42 cirinei Exp $
 */

#include "h8/drivers.h"

/***********************
 * interrupt functions
 ***********************/

__INLINE__ void ERIKA_interrupt_on(void)
{
   ERIKA_hal_enableIRQ(); 
}

__INLINE__ void ERIKA_interrupt_off(void)
{
   ERIKA_hal_disableIRQ();
}

__INLINE__ void ERIKA_enable_irq(ERIKA_TYPEIRQ irq)
{   
  (* (volatile ERIKA_UINT8 *) (irq | 0xFF00)) |=  (1 << ((irq >>8) & 0x0007));
}

__INLINE__ void ERIKA_disable_irq(ERIKA_TYPEIRQ irq)
{ 
  (* (volatile ERIKA_UINT8 *) (irq | 0xFF00)) &=  ~(1 << ((irq >>8) & 0x0007));
}


/********************
 * button functions
 ********************/

__INLINE__ void ERIKA_button_irq_mode(ERIKA_UINT8 mode)
{ 
   ISCR = mode; 
}


__INLINE__ void ERIKA_ad_init(void)
{
    Port6DdrShadow |=0x07;
    Port6Ddr = Port6DdrShadow;
    ADCSR &= 0x00;
    ADCR   = 0x7F;    
}

__INLINE__ void ERIKA_ad_select(ERIKA_UINT8 c)    
{
    ADCSR &= 0xC0;
    ADCSR |= (c & 0x1F);
    /* bit 2,1,0 of port6(h8/3292) set to 1 if sensor (of port 1,2,3) is active, 0 if passive */
    Port6DdrShadow |= (c>>5);
    Port6Ddr=Port6DdrShadow;    	    
    Port6 &= 0xF8;
    Port6 |= (c>>5);
}

__INLINE__ void ERIKA_ad_start(void)
{
   ADCSR |= 0x20;
}

__INLINE__ void ERIKA_ad_start_fast(void)
{
   ADCSR |= 0x28;
}
__INLINE__ void ERIKA_ad_stop(void)
{
   ADCSR &= ~0x20;
}
__INLINE__ void ERIKA_ad_reset_end_flag(void)
{
   ADCSR &= 0x7F;
}

__INLINE__ ERIKA_UINT16 ERIKA_ad_get_port1(void)
{
   return ADADDRC;
}

__INLINE__ ERIKA_UINT16 ERIKA_ad_get_port2(void)
{
   return ADADDRB;
}

__INLINE__ ERIKA_UINT16 ERIKA_ad_get_port3(void)
{
   return ADADDRA;
}


/***********************
 * I/O ports functions
 ***********************/

__INLINE__ void ERIKA_ports_init (void)
{
  /* reset data direction (shadow) registers */
    Port1Ddr=Port1DdrShadow=0xFF;  /* output (address bus) */
    Port2Ddr=Port2DdrShadow=0xFF;  /* output (address bus) */
    Port3Ddr=Port3DdrShadow; 
    Port4Ddr=Port4DdrShadow;
    Port5Ddr=Port5DdrShadow=0x06;
    Port6Ddr=Port6DdrShadow;
}


/**************************
 * 16 bit timer functions
 **************************/

__INLINE__ void ERIKA_t16_init(void)
{
   T16TCR   = 0x02;     // set clock to f/32 (f=16Mhz)
   T16TIER  = 0x01;     // disable all interrupt source   
   T16TOCR  = 0xE0;	// disable output compare 
   T16TCSR  &=0x00;	// reset flag int. and disable match 
    /* NOTE: to write TCSR register, MUST read it before.(see hitachi manual) */   
}

__INLINE__ void ERIKA_t16_set_frc(ERIKA_UINT16 value)
{
   T16FRC =value;
}

__INLINE__ ERIKA_UINT16 ERIKA_t16_get_frc()
{
   return T16FRC;
}

__INLINE__ void ERIKA_t16_reset_frc_ovf(void)
{
   T16TCSR &= 0xFD;
}

__INLINE__ ERIKA_UINT16 ERIKA_t16_get_ocra(void)
{
  T16TOCR &=0xEF;   // select OCRA
  return T16OCRA;
}

__INLINE__ void ERIKA_t16_set_ocra(ERIKA_UINT16 value)
{
  T16TOCR &=0xEF;   // select OCRA
  T16OCRA = value;
}

__INLINE__ void ERIKA_t16_reset_ocra_match_flag(void)
{
   T16TCSR &=0xF7;
}

__INLINE__ ERIKA_UINT16 ERIKA_t16_get_ocrb(void)
{
  T16TOCR |=0xF0;   // select OCRB
  return T16OCRB;
}

__INLINE__ void ERIKA_t16_set_ocrb(ERIKA_UINT16 value)
{
  T16TOCR |=0xF0;   // select OCRB
  T16OCRB = value;
}

__INLINE__ void ERIKA_t16_reset_ocrb_match_flag(void)
{
   T16TCSR &=0xFB;
}


/**************************
 * 8 bit timers functions
 **************************/

#ifdef __T80_SUPPORT__

    __INLINE__ void ERIKA_t80_init(void)
    {
      // following statements are to be used to allow 
      // IR communication and motors' PWM management at the same time
#if defined (__IR_SUPPORT__) && defined(__ERIKA_PWM_USED__)
        T80TCR   = 0x00; 
	/* reset counter */
   	T80TCNT  = 0;    
	/* output to 0 'cause no notes are to be played */
	T80TCSR  = 0x15; 
	/* set group clock (increment timer) */ 
        T8STCR  &= 0xF8;   // 4us*/
    #else
	/* stop timer */
	T80TCR   =0x00; 
	/* reset counter */
	T80TCNT  =0;    
	/* clear previous match; matchA=>output=1; matchB=>output=0; */	
	T80TCSR  =0x16; 
	/* set clock (4us period) */ 
	T8STCR  &=0xFE; 
#endif
    }

    __INLINE__ void ERIKA_t80_start(void)
    {
#if defined (__IR_SUPPORT__) && defined(__ERIKA_PWM_USED__) 
      // using T80 for PWM
      /* MatchA, MatchB, Overflow interrupts; clock 4us */		
      T80TCR = 0xE2; 
#else   
      /* no interrupt; clear on tcroa match; set clock 500kHz (2us) */	
      T80TCR = 0x0A; 
#endif
    }
    
    __INLINE__ void ERIKA_t80_stop(void)
    {
#if defined (__IR_SUPPORT__) && defined(__ERIKA_PWM_USED__)
      T80TCR = 0x00;
#else
      /* no interrupt; clear on match tcora; stop clock */
      T80TCR = 0x08; 
#endif
    }
    
    __INLINE__ void ERIKA_t80_set_cora(ERIKA_UINT8 value)
    {
	T80TCORA = value; 
    }
    
    __INLINE__ void ERIKA_t80_set_corb(ERIKA_UINT8 value)
    {
	T80TCORB = value; 
    }

#endif /* __T80_SUPPORT__ */


#ifdef __T81_SUPPORT__

__INLINE__ void ERIKA_t81_init(void)
{
  /* stop timer */
  T81TCR   =0x00; 
  /* reset counter */
  T81TCNT  =0;    
  /* clear previous match */	
  T81TCSR  =0x10; 
  /* set group clock (increment timer) */ 
  T8STCR  &=0xF8;   // 4us*/
}

    __INLINE__ void ERIKA_t81_start(void)
    {
#if defined (__ERIKA_PWM_USED__) && !defined (__IR_SUPPORT__)
      /* MatchA, MatchB, Overflow interrupts; clock 4us */		
      T81TCR = 0xE2;  
#else
      /* No Interrupt; clock 8us */		
      /* It could be usefull to define some constant for 	*
       * specific application 				*/
      T81TCR = 0x02;
#endif
    }
    
    __INLINE__ void ERIKA_t81_stop(void)
    {
	/* no interrupt; stop clock */
	T81TCR = 0x00; 
    }
    
    __INLINE__ void ERIKA_t81_set_cora(ERIKA_UINT8 value)
    {
	T81TCORA = value; 
    }
    
    __INLINE__ void ERIKA_t81_set_corb(ERIKA_UINT8 value)
    {
	T81TCORB = value; 
    }

#endif /* __T81_SUPPORT__ */
/**********************
 * motors functions
 **********************/

#ifdef __MOTOR_VP_SUPPORT__
    struct motor_state { 
	ERIKA_UINT8 power;  /* MIN_MOTOR_POWER, ..., MAX_MOTOR_POWER */ 
        ERIKA_UINT8 action; /* brake, go0, go1, float */ 
    };
#endif


#ifdef __MOTOR_A_VP_USED__
    struct motor_state motorA_state;

    __INLINE__ void ERIKA_motorA_power(ERIKA_UINT8 pow)
    {	    
	motorA_state.power=pow;
    }    

    __INLINE__ void ERIKA_motorA_action(ERIKA_UINT8 action)
    {
	motorA_state.action=action;
    }
#endif

#ifdef __MOTOR_B_VP_USED__
    struct motor_state motorB_state;

    __INLINE__ void ERIKA_motorB_power(ERIKA_UINT8 pow)
    {
	motorB_state.power=pow;
    }

    __INLINE__ void ERIKA_motorB_action(ERIKA_UINT8 action)
    {
	motorB_state.action=action;
    }            
#endif

#ifdef __MOTOR_C_VP_USED__
    struct motor_state motorC_state;

    __INLINE__ void ERIKA_motorC_power(ERIKA_UINT8 pow)
    {
	motorC_state.power=pow;
    }        

    __INLINE__ void ERIKA_motorC_action(ERIKA_UINT8 action)
    {
	motorC_state.action=action;
    }    
#endif

#if defined(__MOTOR_VP_SUPPORT__) || defined(__ERIKA_PWM_USED__)
     
    __INLINE__ void ERIKA_motor_init(void)
    {	
	#ifdef __MOTOR_A_VP_USED__
	    ERIKA_motorA_power(VP_MIN_MOTOR_POWER);
	    ERIKA_motorA_action(MotorFloat);
	#endif

	#ifdef __MOTOR_B_VP_USED__
	    ERIKA_motorB_power(VP_MIN_MOTOR_POWER);	    
	    ERIKA_motorB_action(MotorFloat);	    
	#endif

	#ifdef __MOTOR_C_VP_USED__
	    ERIKA_motorC_power(VP_MIN_MOTOR_POWER);	
	    ERIKA_motorC_action(MotorFloat);	    
	#endif
	
	MotorDevReg=0x00;    
					
	/************** NEW PWM MANAGEMENT **************/
#ifdef __ERIKA_PWM_USED__	
#ifndef __IR_SUPPORT__
    	    /* all ports not powered */
	    MotorDevReg=0x00;    	  
            /* Init timer */
	    ERIKA_t81_init();
    	    /* Optimized handler installation */

/*********************************************************************** 
*The following instructions must not be executed because an optimized  *
*   version of the handlers was written. 	                       *
***********************************************************************/
//	    ERIKA_set_handler(OVI1_IRQ, OVI1_handler); 
//	    ERIKA_set_handler(CMI1A_IRQ, CMI1A_handler);	    
//	    ERIKA_set_handler(CMI1B_IRQ, CMI1B_handler);	    

/* Writing directly handlers' stub code at following addresses */
	    *(void (**) (void)) (0xFDAE)=CMI1A_handler;
	    *(void (**) (void)) (0xFDB0)=CMI1B_handler;	    
	    *(void (**) (void)) (0xFDB2)=OVI1_handler;	    
	    /* Enable timer interrupt */
	    ERIKA_enable_irq(CMI1A_IRQ);
            ERIKA_enable_irq(CMI1B_IRQ);	    
	    ERIKA_enable_irq(OVI1_IRQ);	    
            /* Start timer */
	    ERIKA_t81_start();
#else
	        	    /* all ports not powered */
	    MotorDevReg=0x00;    	  
            /* Init timer */
	    ERIKA_t80_init();
	    T80TCORA = 0;
	    T80TCORB = 0;
    	    /* Optimized handler installation */
	    *(void (**) (void)) (0xFDA8)=CMI0A_handler;
	    *(void (**) (void)) (0xFDAA)=CMI0B_handler;	    
	    *(void (**) (void)) (0xFDAC)=OVI0_handler;	    
	    /* Enable timer interrupt */
	    ERIKA_enable_irq(CMI0A_IRQ);
            ERIKA_enable_irq(CMI0B_IRQ);	    
	    ERIKA_enable_irq(OVI0_IRQ);	    
            /* Start timer */
	      ERIKA_t80_start();	    	    

#endif
/************************************************/	
#endif
    }    
#endif



    
/********************
 * sound functions
 ********************/

#ifdef __PLAY_BUFFER_SUPPORT__  
    struct ERIKA_note_type {
        ERIKA_UINT8 n;  	/* tcroa value to obtain the frequency of that note */ 
	ERIKA_UINT8 n_len; 	/* note length */
	ERIKA_UINT8 p_len; 	/* pause length (after that note has been played) */
    };
               
    __INLINE__ void ERIKA_play_buffer_init(void)
    {
	/* data used by timer handler */
	playing=0;

	/* set channel 0 of 8 bit timer to drive speaker */
	ERIKA_t80_init();
    }

    __INLINE__ void ERIKA_play_buffer_stop(void)
    {
	playing=0;            	
	ERIKA_t80_stop();
    }

    __INLINE__ ERIKA_UINT8 ERIKA_is_playing_buffer(void)
    {
	return playing;
    }

#endif

#ifdef __PLAY_NOTE_SUPPORT__
__INLINE__ void ERIKA_play_note_init(void)
{
    /* data used by timer handler */
    play_note_state=0;
    
    /* set channel 0 of 8 bit timer to drive speaker */
    ERIKA_t80_init();
}

__INLINE__ void ERIKA_play_note_stop(void)
{
    play_note_state=0;    
    ERIKA_t80_stop();        
}
#endif


/***********************
 * initialize function 
 ***********************/

__INLINE__ void ERIKA_init(void)
{
   /* Note that at this point I=1 (interrupt disabled) */
   
   /* sleep must cause sleep mode.
    * software standby mode would hack the system since all on-chip   *
    * modules would be halted and this is not good for real time!     *
    * Furthermore, settling time (~8ms) is needed to recover from.    *
    * So, this is why sleep mode is selected and not sw standby mode. */
    SYSCR= 0x0B; 

    /* clear pending IRQ (button) interrupt */
    ERIKA_disable_irq(ONOFF_BUTTON);        
    ERIKA_disable_irq(RUN_BUTTON);    
    ERIKA_button_irq_mode(ONOFF_LEVEL_RUN_LEVEL);
    
    /* init 16bit timer: always neeed (ROM use it before downloading firmaware) */
    ERIKA_t16_init();       
   
    /* init play buffer */
    #ifdef __PLAY_BUFFER_SUPPORT__   
	ERIKA_play_buffer_init(); 
    #endif
    
    /* init play note */
    #ifdef __PLAY_NOTE_SUPPORT__
	ERIKA_play_note_init();    
    #endif   
    
    /* init motors */
    #if defined(__MOTOR_VP_SUPPORT__) || defined(__ERIKA_PWM_USED__)
	ERIKA_motor_init();
    #endif   
   
    /* init a/d converter */
    #ifdef __AD_SUPPORT__
	ERIKA_ad_init();
    #endif
    
    /* init ports */
	//    ERIKA_ports_init(); REALLY NEEDED???

    /* system handler installation */
  #ifdef __TIMER_A_SUPPORT__	       
	/* set handler to activate periodic task or to call user function */
	ERIKA_set_handler(OCIA_IRQ, ERIKA_timerA_handler);
	ERIKA_t16_set_ocra(ERIKA_t16_get_frc() + TIMER_A_PERIOD); /* next match */        
	ERIKA_t16_reset_ocra_match_flag();    
	ERIKA_enable_irq(OCIA_IRQ);
    #endif    

    #ifdef __TIMER_B_SUPPORT__
	/* set handler for sound, motors, user function, periodic task(if 	*
	 * timerA is not used for that purpose)					*/
	ERIKA_set_handler(OCIB_IRQ, ERIKA_timerB_handler);
	ERIKA_t16_set_ocrb(ERIKA_t16_get_frc() + TIMER_B_PERIOD); /* next match */        
	ERIKA_t16_reset_ocrb_match_flag();    
	ERIKA_enable_irq(OCIB_IRQ);
    #endif
}


/***************************
 * delete firmware function
 ***************************/            
__INLINE__ void delete_firmware(void)
{          
#if defined(__IR_RX_SUPPORT__) || defined(__IR_SUPPORT__)
   SISTCR = 0xF8;
#endif
   *(char *) 0xFFCC=1;	/* prevents RCX from turning off when reset */
   
   (*(void (**) (void))0) ();	/* jumps to the reset interrupt vector */
}
