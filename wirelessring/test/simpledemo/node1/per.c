/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 */
 

#include "code.h"
#include "alarms/alarms.h"

#define IR_TX_TIMEOUT 5000

extern ERIKA_UINT8 sd;		/* sent data, number of bytes sent in a single message */
extern ERIKA_UINT8 sent;

/****************************************************************************
  Periods
****************************************************************************/

/* this handler is executed when ON/OFF button falls down */
void ONOFFbutton_handler(void)
{       
    delete_firmware();
}

void RUNbutton_handler(void)
{
    sd = 1;
    ERIKA_IRQ_make_ready(TID_thread0, 1);
}

void ERIKA_timerB_user_function(void)
{
    ERIKA_counter_tick(counter0);
}

/****************************************************************************
  IR Functions
****************************************************************************/

void IR_confirm(ERIKA_UINT8 msg)
{
    if (msg == ERIKA_COM_OK)
	ERIKA_IR_receive_enable();
    else
	ERIKA_lcd_write_num(5000 + sd + msg*100);
}

void IR_indication(ERIKA_UINT8 len, ERIKA_UINT8 data[])
{
    ERIKA_lcd_write_num(len+1000);
    sd = len;
    if (sent>0)
    {
	ERIKA_alarm_cancel(alarm_tx);
	sent = 0;
    }
    ERIKA_alarm_setrel(alarm_rx, 500, 0);
}


/****************************************************************************
  Peripheral initialization
****************************************************************************/

ERIKA_declare_stub(ONOFFbutton_handler);
ERIKA_declare_stub(RUNbutton_handler);

void PER_init(void)
{
    ERIKA_UINT16 IR_timeout = IR_TX_TIMEOUT;
    /* external interrupts generated once when you press the button (and not *
    * for all the time you are are pressing the button)                     */
    ERIKA_button_irq_mode(ONOFF_EDGE_RUN_EDGE);

    /* install IRQ handlers */
    ERIKA_set_handler(ONOFFBUTTON_IRQ, ONOFFbutton_handler);   
    ERIKA_set_handler(RUNBUTTON_IRQ, RUNbutton_handler);
  
    /* enable external interrupt sources */ 
    ERIKA_enable_irq(ONOFFBUTTON_IRQ);  
    ERIKA_enable_irq(RUNBUTTON_IRQ);
  
    /* IR configuration */
    ERIKA_IR_result(IR_confirm, IR_indication);
    ERIKA_IR_start(&IR_timeout);
    ERIKA_IR_receive_enable();
}
