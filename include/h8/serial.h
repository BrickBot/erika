/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * The serial communication library has been derived from a work
 * publicly available on the web without licensing
 * (http://www.legolab.daimi.au.dk/DigitalControl.dir/RCX/include/IR.h). We
 * think this file was originally done by Ole Caprani.
 *
 * Part of this file is 
 * Copyright (C) 2003 Michael Bosu, Davide Cantini, Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: serial.h,v 1.6 2004/07/13 12:19:11 cirinei Exp $
 */


#include "erika.h"

#ifndef __H8_SERIAL_H__
#define __H8_SERIAL_H__

#ifdef cplusplus
extern "C" {
#endif

/***********************************************
 *Infrared serial communication
 **********************************************/
#define MaxData 	17

#define ERIKA_COM_OK    	0	/* transmission OK */
#define ERIKA_COM_ERROR 	1	/* generic error */
#define ERIKA_COM_BUSY		2	/* transmission, impossible, channel busy */


#ifdef __IR_ACK_REQ__
    #define ERIKA_COM_NOACK 	3	/* received something while waiting for an ack => ack will never arrive */
    #define ERIKA_COM_TIMEOUT 	4	/* ack not received */
#endif

#ifdef __IR_SUPPORT__
    #define CFC_NO_TIMEOUT	0xFFFF
    #define CFC_NO_DELAY	0x0000
#endif

#define ENABLED 	1
#define DISABLED	0

struct L_Data_type{
#ifdef __IR_SUPPORT__
    void (*request)(ERIKA_UINT8 len, ERIKA_UINT8 *data);
    void (*confirm)(ERIKA_UINT8 msg_id);
#endif
    void (*indication)(ERIKA_UINT8 len, ERIKA_UINT8 *data);
};

extern struct L_Data_type L_Data;

extern ERIKA_UINT8 RX_user_set;

void ERIKA_IR_result(
#ifdef __IR_SUPPORT__
    void (*confirm)(ERIKA_UINT8 msg_status),
#endif
    void (*indication)(ERIKA_UINT8 len, ERIKA_UINT8 *data));

void ERIKA_IR_start(
#ifdef __IR_SUPPORT__
    ERIKA_UINT16* tx_timeout);
#else
    void);
#endif

__INLINE__ void ERIKA_IR_receive_enable(void)
{
    SISCR |= (1 << 6); /* Enable RXI */
    SISCR |= (1 << 4); /* Enable RE  */
    RX_user_set = ENABLED;
}

__INLINE__ void ERIKA_IR_receive_disable(void)
{
    SISCR &= ~(1 << 4); /* Disable RE  */
    SISCR &= ~(1 << 6); /* Disable RXI */
    RX_user_set = DISABLED;
}

#ifdef __IR_SUPPORT__
void ERIKA_IR_send(ERIKA_UINT8 len, ERIKA_UINT8 *m);
#endif

__INLINE__ void ERIKA_IR_Close(void)
{
    /* disable all interrupts, TX function and RX function */
    SISCR = 0;
}

void ERIKA_IR_disable_interrupts(); // ????

#ifdef cplusplus
}
#endif

#endif
