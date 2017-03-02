/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: code.c,v 1.1 2004/07/13 13:13:21 cirinei Exp $
 * 
 * The RCX receives a packet with x data bytes and sends a packet with 
 * (x+1) data bytes. Pressing RUN, the RCX starts (or restarts) 
 * sending a packet with only 1 data byte. Pressing VIEW, the RCX stop
 * sending and return to the initial situation. You must first of all press
 * PRGM to initialize the wireless gateway.
 */

#include "code.h"
#include "alarms/alarms.h"

#define NODE_NUM 1
#define MAX_NODE 1
#define PIC_TIMEOUT ((0x40 / (MAX_NODE + 1)) * NODE_NUM)

ERIKA_UINT8 sd=1;
ERIKA_UINT8 buffer[MaxData+1];
ERIKA_UINT8 sent;
ERIKA_UINT8 wg_init[3];
ERIKA_UINT8 init_flag=0;

/****************************************************************************
  Other functions
****************************************************************************/

void init_buffer( void )
{
    ERIKA_INT8 i=0;
    buffer[0] = 0x10;		/* index = source, destination */
    for (i = 0; i < MaxData; i++)
	buffer[i+1] = i;
}

/****************************************************************************
  Application threads
****************************************************************************/


void thread0(void)
{
    ERIKA_alarm_setrel(alarm_tx, 4000, 0);	/* if retransmission needed */
    sent++;
    ERIKA_lcd_write_num(sd + sent * 100);
    L_Data.request(sd, buffer);
}


void thread1(void)
{   
    sd++;
    if (sd > MaxData)
	sd = 2;
    ERIKA_lcd_write_num(sd+2000);
    ERIKA_alarm_setrel(alarm_tx, 500, 0);
} 

/****************************************************************************
  Boot and initialization
****************************************************************************/

void ERIKA_dummy(void)
{                 

  /* H8 basic initialization */
  ERIKA_init();     

  /* initialization of the peripherals used in the application */
  PER_init();
  ERIKA_lcd_write_num(123);
  
  ERIKA_hal_enableIRQ();

#ifdef __PLAY_NOTE_SUPPORT__
  ERIKA_play_note(_FA0, 40);
  ERIKA_play_note(_LA0, 25);
#endif    

  /* values to initialize wireless gateway */
  wg_init[0] = NODE_NUM;
  wg_init[1] = MAX_NODE;
  wg_init[2] = PIC_TIMEOUT;

  init_buffer();
    
  for (;;)
  {
    if (ERIKA_button_pressed(ONOFF_BUTTON))
        delete_firmware();			
    if (ERIKA_button_pressed(PRGM_BUTTON) && init_flag == 0)
    {
	L_Data.request(3, wg_init);		/* initialize wireless gateway */
	init_flag = 1;
    }
    if (ERIKA_button_pressed(VIEW_BUTTON))
    {
	sd = 2;
	init_flag = 0;
	if (sent > 0)
	{
	    ERIKA_alarm_cancel(alarm_tx);
	    sent = 0;
	}
	ERIKA_lcd_write_num(123);
    }
  } 
}

