/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 */
 
#include "osekirq.h"
#include "code.h"

/****************************************************************************
  Periods
****************************************************************************/




/* this handler is executed when ON/OFF button falls down */
ISR(ONOFFbutton_handler)
{       
  /* the system goes in sleep state. to be woken up, it has to be
     reset, and this happens at the next irq., that causes the
     handler to execute the else branch of this if statement. */
  if (mode != MODE_SHUTDOWN) {
    mode_change(MODE_SHUTDOWN);

  }
  else {
    mode_change(MODE_POWERON);
  }
}




/* this handler is called every 2000us (when T16OCRB matchs FRC). 
   ERIKA uses this interrupt for:
   - handling motors (duty cicle)
   - handling sound (generated by output of 8bit timer0)
   - calling this handler :-)
*/
void ERIKA_timerB_user_function(void)
{
  ERIKA_counter_tick(counter0);
}




ISR(ADI_handler)
{
  ERIKA_UINT16 a, b;

  ERIKA_ad_stop();    

  a=(ERIKA_ad_get_port3()>>(6+LIGHT_TOLERANCE))+OFFSET_ERR;	
  // NOTE: one sensor read OFFSET_ERR units less than other.
  //       suppose it is S1, so offset is summed to it
  b=(ERIKA_ad_get_port2()>>(6+LIGHT_TOLERANCE)); 
    
  /* s1, s2 are integer, if a,b are unsigned so >> is logical */
  CTRL_readsensors(a,b);

  /* when AD conversion ended, activate thread for control (if setup done)*/
  if (mode==MODE_NORMAL ||
      mode==MODE_SLOWSPEED ||
      mode==MODE_FAULT1) {
    ActivateTask(thread1);
  }
    
  /* turn on red light for sensor */
  Port6 |= ~0xFC;
    
  /* reset A/D end-conversion flag */
  ERIKA_ad_reset_end_flag();
}






ISR(RUNbutton_handler)
{
  ERIKA_lcd_write_num(6668);
  switch (mode) {

  case MODE_SHUTDOWN:
#ifdef __PLAY_NOTE_SUPPORT__
    ERIKA_play_note(_DO0, 50);
#endif
    delete_firmware();	
    break;

  case MODE_SETUP1:
    /* reads black from the sensor */
    mode_change(MODE_SETUP2);
    break;

  case MODE_SETUP2:
    /* reads gray from the sensor */
    mode_change(MODE_SETUP3);
    break;

  case MODE_SETUP3:
    /* reads white from the sensor */
    mode_change(MODE_STOP);
    break;

  default:
    /* why are you still pressing the Run button like an idiot? */
    ERIKA_lcd_write_num(9000+mode);
    break;
  }
}



/****************************************************************************
  IR layer indication
****************************************************************************/

static ERIKA_UINT8 PER_IR_command = 0;

static int xxx=0;
void PER_IR_indication(ERIKA_UINT8 len, ERIKA_UINT8 *data)
{
  //  ERIKA_lcd_write_num(6000+ (xxx++)); // this shows that send.c sends more than 1 byte
  PER_IR_command = *data;
  ActivateTask(thread2);
  ERIKA_IR_receive_enable();
}

ERIKA_UINT8 PER_get_last_command(void)
{
  return PER_IR_command;
}


/****************************************************************************
  Peripheral initialization
****************************************************************************/

void PER_init(void)
{
  /* external interrupts generated once when you press the button (and not *
   * for all the time you are are pressing the button)                     */
  ERIKA_button_irq_mode(ONOFF_EDGE_RUN_EDGE);

  /* select A/D channel and mode */
  ERIKA_ad_select (AD_PORT32_SCAN_ACT_ACT);
    
  /* install IRQ handlers */
  ERIKA_set_handler(ONOFFBUTTON_IRQ, ONOFFbutton_handler);   
  ERIKA_set_handler(RUNBUTTON_IRQ, RUNbutton_handler);
  ERIKA_set_handler(ADI_IRQ, ADI_handler);
    
  /* enable external interrupt sources */ 
  ERIKA_enable_irq(ONOFFBUTTON_IRQ);  
  ERIKA_enable_irq(RUNBUTTON_IRQ);        
  ERIKA_enable_irq(ADI_IRQ);
    
  /* IR configuration */
  ERIKA_IR_result(PER_IR_indication);
  ERIKA_IR_start();
  ERIKA_IR_receive_enable();

  /* periodic tasks */
  //  remaining_period[0]=th_period[0];
}
