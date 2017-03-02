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
 * CVS: $Id: serial.c,v 1.9 2004/07/13 12:19:11 cirinei Exp $
 */

/*** contains functions for infrared serial communication **/
#include "erikacfg.h"
#include "erika.h"
#include "h8/serial.h"

/************************************************************************
* Data Types and variables						*
************************************************************************/

#define bit0    (1 << 0)
#define bit1    (1 << 1)
#define bit2    (1 << 2)
#define bit3    (1 << 3)
#define bit4    (1 << 4)
#define bit5    (1 << 5)
#define bit6    (1 << 6)
#define bit7    (1 << 7)

#define LongRange   Port4 &= ~bit0
#define ShortRange  Port4 |=  bit0

#define Header		0x55
#define FF		0xFF
#define Zero		0x00
#define MSG		0xFA
#define Lock		0x1E
#define Unlock		0xA5

#define NO_CFC		0xFFFF

#ifdef __IR_ACK_REQ__
    #define ACK		0xF3
#endif

/*
We have to distinguish between enable and disable called by
user and by system, so system uses these two macros while
user uses inline function declared in serial.h
*/

#define RX_ENABLE	(SISCR |= bit4, SISCR |= bit6)
#define RX_DISABLE	(SISCR &= ~bit4, SISCR &= ~bit6)

#define RX_TIMEOUT	8
ERIKA_UINT8 RX_timeout;

#ifdef __IR_ACK_REQ__
    /************************************************
    * Message-Acknowledgement Protocol = MAP	*
    ************************************************/
    #define ACK_TIMEOUT 100
    ERIKA_UINT8 ACK_req;
    ERIKA_UINT8 ACK_timeout;
#endif

/************************************************
* Channel Free Control = CFC			*
************************************************/
ERIKA_UINT16 CFC_TIMEOUT;	//set by user in ERIKA_IR_start
ERIKA_UINT16 CFC_timeout;
#define CFC_DELAY	20
ERIKA_UINT16 CFC_delay;

struct {ERIKA_UINT8 len; ERIKA_UINT8 buf[MaxData];} CFC_msg;

/****************************************
* Other Data Types and Variables	*
****************************************/
#define Lock_TIMEOUT	100
ERIKA_UINT8 Lock_timeout;

struct Buffer { ERIKA_UINT8 type, len, buf[MaxData], check; };

#ifdef __IR_SUPPORT__
    enum Tstate_t  { Idle, HeaderSent, FFSent, ZeroSent, 
	             typeSent, ntypeSent, lenSent, nlenSent, bufSent, nbufSent, checkSent, ncheckSent };
    typedef enum Tstate_t Tstate;

    volatile Tstate TransmitState; 
    struct Buffer TransmitBuffer;
#endif

ERIKA_UINT8 BufferPos;

enum Rstate_t  { LookingForHeader, HeaderReceived, LockReceived, UnlockReceived, FFReceived, ZeroReceived, 
                 typeReceived, ntypeReceived, lenReceived, nlenReceived, bufReceived, nbufReceived, checkReceived };
typedef enum Rstate_t Rstate;

volatile Rstate ReceiveState;
struct Buffer ReceiveBuffer;

/*
   Error = bit5 + bit4 + bit3 >> 3. 
   Error = 0 means reception of data byte ok. Otherwise the errorvalue 
   can be interpreted according to: 
   bit3 ,  parity error,
   bit4 ,  frame error,
   bit5 ,  overrun error.
 */

ERIKA_UINT8 Error;

struct L_Data_type L_Data;

ERIKA_UINT8 RX_user_set;
ERIKA_UINT8 TX_lock;

/************************************************
* Declare interrupt handlers			*
************************************************/

#ifdef __IR_SUPPORT__
    ERIKA_declare_stub(ERIKA_TXI_handler);
    ERIKA_declare_stub(ERIKA_TEI_handler);
#endif

ERIKA_declare_stub(ERIKA_RXI_handler);
ERIKA_declare_stub(ERIKA_ERI_handler);

/************************************************
* InfraRed Serial Communication Functions	*
************************************************/

void ERIKA_IR_start(
		#ifdef __IR_SUPPORT__
		    ERIKA_UINT16* CFC_set
		#endif
		)
{ 
    
    ERIKA_UINT8 i = 0;
    
    #ifdef __IR_SUPPORT__
	ERIKA_set_handler(TXI_IRQ,ERIKA_TXI_handler);
        ERIKA_set_handler(TEI_IRQ,ERIKA_TEI_handler);
    #endif

    ERIKA_set_handler(RXI_IRQ,ERIKA_RXI_handler);
    ERIKA_set_handler(ERI_IRQ,ERIKA_ERI_handler);

    #ifdef __IR_SUPPORT__
	TransmitState = Idle;
    #endif

    ReceiveState = LookingForHeader;
    Error = 0;

    #ifdef __IR_SUPPORT__
	/* Initialize Timer 1 */
        T81TCR   =  ( bit3 | bit0 );  /* Timer cleared on compare-match A,
	//                    	   internal clock rate 0.5 usec if SISTCR = 0xF8, 0.125 if SISTCR = 0xFA */
        T81TCSR  =  ( bit1 | bit0 );  /* Toggle on compare-match A,
	//                               Port 6, pin 7 set to output */
        SISTCR   = 0xFA;		/* Internal clock rate 0.125 usec */
        T81TCORA = 105;		/* Toggle every 105*0.125 usec, if SISTCR = 0xFA
        //				   freq. 38.1 khz */

	/* Initialize Port4, pin 0 as output and set it to short range */
        Port4DdrShadow |= bit0;	
	Port4Ddr  = Port4DdrShadow;
        ShortRange; 
    #endif		 

    /************************************************
    * Initialize Serial Communication Interface 	*
    ************************************************/
  
    /* Disable interrupts, disable transmit and receive, internal clock 
	and clock signal not output */
    SISCR = 0;

    /* Asynchronous communication, 8 bit data, odd parity, 
	one stop bit, internal clock source = 16 MHz */
    SISMR = ( bit5 | bit4 );
 
    /* Bit rate 2400 bit/sec */
    SISBRR = 207;	
  
    /* Enable transmit, do not enable receive until explicitly done with ERIKA_IR_receive_enable, to avoid problems */
    SISCR |= bit5;
  
    RX_timeout = 0;
    
    #ifdef __IR_ACK_REQ__
	/* MAP */
        ACK_req = 0;
	ACK_timeout = ACK_TIMEOUT;
    #endif
    
    /* CFC */
    if (CFC_set == NULL)
	CFC_TIMEOUT = NO_CFC;
    else
    {
	CFC_TIMEOUT = *CFC_set;
	CFC_timeout = CFC_TIMEOUT;
    }
    CFC_delay = CFC_DELAY;
    for(i=0; i < MaxData; i++)
	CFC_msg.buf[i] = 0;
    TX_lock = 0;
}



#ifdef __IR_SUPPORT__
void ERIKA_IR_send(ERIKA_UINT8 len, ERIKA_UINT8 *m) /* len = 0 => ACK, if __IR_ACK_REQ__ */
{
    ERIKA_UINT8 i=0; 

//    LongRange;	

    #ifdef __IR_ACK_REQ__
	if (len != 0)
	{
	    if (TransmitState != Idle || ReceiveState != LookingForHeader || ACK_req || TX_lock)
	    {
    		CFC_msg.len = len;
		for (i = 0; i < len; i++)
    	    	    CFC_msg.buf[i] = m[i];
		if (CFC_timeout >= CFC_TIMEOUT)
	    	    CFC_timeout = 0;
		return;
	    }
	}
	else
	    ACK_req = 1;
    #else
	if (TransmitState != Idle || ReceiveState != LookingForHeader || TX_lock)
	{
    	    CFC_msg.len = len;
	    for (i = 0; i < len; i++)
    	        CFC_msg.buf[i] = m[i];
	    if (CFC_timeout >= CFC_TIMEOUT)
	        CFC_timeout = 0;
	    return;
	}
    #endif    

    RX_DISABLE;

    /* Control and set message lenght */
    if (len >= MaxData)
	len = MaxData;

    /* Enter word into TransmitBuffer */
    
    #ifdef __IR_ACK_REQ__  
	if (len == 0)
	    TransmitBuffer.type = ACK;
        else
	{
	    TransmitBuffer.type = MSG;
	    CFC_timeout = CFC_TIMEOUT;
	}
    #else
	TransmitBuffer.type = MSG;
	CFC_timeout = CFC_TIMEOUT;
    #endif
    

    TransmitBuffer.len = len;

    TransmitBuffer.check = ((TransmitBuffer.type + TransmitBuffer.len) & 0xff);

    for (BufferPos = 0; BufferPos < TransmitBuffer.len; BufferPos++) 
    {
	TransmitBuffer.buf[BufferPos] = m[BufferPos];
	TransmitBuffer.check = ((TransmitBuffer.check + m[BufferPos]) & 0xff);
    }
  
    /* To avoid blocks in trasmitting 0xff as last byte */
    if (TransmitBuffer.check == 0x00) 
	TransmitBuffer.check = 0x0F;
     
    /* Transmit first byte of message  */
    SISTDR  = Header;
    SISSSR  &= ~bit7;            /* Clear TDRE flag and hence TEND flag */

    TransmitState = HeaderSent;        
  
    SISCR |=  ( bit7 | bit2 );   /* Enable TXI and TEI */   
}
#endif



void ERIKA_IR_result(
	    #ifdef __IR_SUPPORT__
		void (*confirm)(ERIKA_UINT8 msg_status),
	    #endif
		void (*indication)(ERIKA_UINT8 len, ERIKA_UINT8 *data))
{
    #ifdef __IR_SUPPORT__
	L_Data.confirm    = confirm;
        L_Data.request    = ERIKA_IR_send;
    #endif
    L_Data.indication = indication;
}



#ifdef __IR_SUPPORT__
void ERIKA_TXI_handler ( void ) /* Transmit Data Register Ready for next byte */
{ 
    switch (TransmitState) {

    case Idle:       break;

    case HeaderSent: 
	SISTDR = FF; 
	SISSSR &= ~bit7; /* clear TDRE */
	TransmitState = FFSent;
    break;

    case FFSent:
	SISTDR = Zero;
        SISSSR &= ~bit7;  /* clear TDRE */ 
	TransmitState = ZeroSent;
    break;

    case ZeroSent:
	SISTDR = TransmitBuffer.type;
        SISSSR &= ~bit7;  /* clear TDRE*/ 
	TransmitState = typeSent;
    break;

    case typeSent:
	SISTDR = (~TransmitBuffer.type) & 0xff;
        SISSSR &= ~bit7; /*  clear TDRE */
	#ifdef __IR_ACK_REQ__
	    if (TransmitBuffer.type == ACK)
		TransmitState = nbufSent;
	    else
    		TransmitState = ntypeSent;
	#else
	    TransmitState = ntypeSent;
	#endif
    break;

    case ntypeSent:
	SISTDR = TransmitBuffer.len;	/* ERIKA IR */
	SISSSR &= ~bit7;/* clear TDRE*/ 
	BufferPos = 0;
	TransmitState = lenSent;
    break;

    case lenSent:
	SISTDR = (~TransmitBuffer.len) & 0xff;
	SISSSR &= ~bit7;
        if (TransmitBuffer.len > 0)
    	    TransmitState = nlenSent;
	else
    	    TransmitState = nbufSent;
    break;
    
    case nlenSent:
	SISTDR = TransmitBuffer.buf[BufferPos];
	SISSSR &= ~bit7;
        TransmitState = bufSent;
    break;
    
    case bufSent:
	SISTDR = (~TransmitBuffer.buf[BufferPos++]) & 0xff;
	SISSSR &= ~bit7;  /* clear TDRE */
	if (BufferPos < TransmitBuffer.len)
    	    TransmitState = nlenSent;
	else
    	    TransmitState = nbufSent;
    break;

    case nbufSent:
	SISTDR = TransmitBuffer.check;
        SISSSR &= ~bit7;  /* clear TDRE */
	TransmitState = checkSent;
    break;

    case checkSent:
	SISTDR = (~TransmitBuffer.check) & 0xff;
        SISSSR &= ~bit7;
	TransmitState = ncheckSent;
    break;
    
    case ncheckSent:  
	SISCR &= ~bit7;  /* Disable X TXI */
    break;    
  }
}



void ERIKA_TEI_handler ( void ) /* Last byte of message transmitted */
{
    TransmitState  = Idle;
    SISCR &=  ~ bit2 ; 			/* Disable TEIE X */
    #ifdef __IR_ACK_REQ__
	if (ACK_req)	/* message sent was an ACK */
        {
	    if (RX_user_set == ENABLED)
		RX_ENABLE;			/* to reset RX status */
	    ACK_timeout = ACK_TIMEOUT;		/* stop timeout */
    	    ACK_req = 0;			
	    L_Data.indication(ReceiveBuffer.len, ReceiveBuffer.buf);
	}
	else		/* message sent was a MSG */
	{
	    RX_ENABLE;	    		/* to receive ACK */
	    ACK_req = 1;			/* waiting for an ACK */
	    ACK_timeout = 0;	/* start timeout */
	}
    #else
	L_Data.confirm(ERIKA_COM_OK);
    #endif
}
#endif



ERIKA_UINT8   b;



void ERIKA_RXI_handler ( void ) /* Receive Data Register Full */
{
    b = SISRDR;
    SISSSR &= ~bit6;  /* clear Receive Data Register Full flag */  

    switch ( ReceiveState ){

    case LookingForHeader:
	if ( b == Header )
	    ReceiveState = HeaderReceived;
    break;

    case HeaderReceived:
	switch ( b )
	{
	    case FF:
		ReceiveState = FFReceived;
	    break;
	
	    case Lock:
		if (!TX_lock)
		    ReceiveState = LockReceived;
		else
		{
    		    RX_timeout = RX_TIMEOUT;
		    ReceiveState = LookingForHeader;
		}
	    break;
	    
	    case Unlock:
		if (TX_lock)
		    ReceiveState = UnlockReceived;
		else
		{
    		    RX_timeout = RX_TIMEOUT;
		    ReceiveState = LookingForHeader;
		}
	    break;
	    
	    case Header:
		/* nothing to do */
	    break;
	    
	    default:
    		RX_timeout = RX_TIMEOUT;
		ReceiveState = LookingForHeader;
	    break;
	}
    break;

    case LockReceived:
	if ( b == (~Lock & 0xFF))
	    TX_lock = 1;
	Lock_timeout = 0;	/* timeout to unlock RCX after Lock_TIMEOUT ms */		    
	RX_timeout = RX_TIMEOUT;
	ReceiveState = LookingForHeader;
    break;
    
    case UnlockReceived:
	if ( b == (~Unlock & 0xFF))
	    TX_lock = 0;
	Lock_timeout = Lock_TIMEOUT;
	RX_timeout = RX_TIMEOUT;
	ReceiveState = LookingForHeader;
    break;

    case FFReceived:
	if ( b == Zero ) 		// 00
	    ReceiveState = ZeroReceived;
	else if (b != Header)		// not 55
	{
	    RX_timeout = RX_TIMEOUT;
	    ReceiveState = LookingForHeader;
	}
	else				// 55
	    ReceiveState = HeaderReceived;
    break;

    case ZeroReceived:
	BufferPos = 0;
	ReceiveBuffer.type = b;
	ReceiveBuffer.check = (ReceiveBuffer.type & 0xff);
	ReceiveState = typeReceived;
    break;

    case typeReceived:
	#ifdef __IR_ACK_REQ__
	    if (ReceiveBuffer.type == ACK)		/* ACK received */
	    {
		if (ACK_req)
		{
		    ReceiveState = nbufReceived;
		}
		else					/*  waiting for no ACK */
		{
		    RX_timeout = RX_TIMEOUT;
		    ReceiveState = LookingForHeader;
		}
	    }		
	    else if (ReceiveBuffer.type == MSG)		/* MSG received */
	    {
		if (ACK_req)				/* waiting for an ACK */
		{	    
		    ACK_timeout = ACK_TIMEOUT;		/* stop timeout */
		    ACK_req = 0;
		    L_Data.confirm(ERIKA_COM_NOACK);
		}
	        ReceiveState = ntypeReceived;
	    }	    
	#else
	    ReceiveState = ntypeReceived;
	#endif
    break;

    case ntypeReceived:
        ReceiveBuffer.len = b;
	ReceiveBuffer.check = ((ReceiveBuffer.check + ReceiveBuffer.len) & 0xff);
        ReceiveState = lenReceived; 
    break;
    
    case lenReceived:
	if (ReceiveBuffer.len > 0)
	    ReceiveState = nlenReceived;
	else
	    ReceiveState = nbufReceived;
    break;

    case nlenReceived:
	ReceiveBuffer.buf[BufferPos] = b;
        ReceiveBuffer.check = ((ReceiveBuffer.check + ReceiveBuffer.buf[BufferPos++]) & 0xff);
	ReceiveState = bufReceived;
    break;
    
    case bufReceived:
	if (BufferPos < ReceiveBuffer.len)
	    ReceiveState = nlenReceived;
	else
	    ReceiveState = nbufReceived;
    break;

    case nbufReceived:       
        if (ReceiveBuffer.check == 0x00)	/* To avoid blocks in receiving 0xff as last byte (~check)*/
	    ReceiveBuffer.check = 0x0f;
	ReceiveState = checkReceived;
    break;

    case checkReceived:
	if ( b == ((~ReceiveBuffer.check) & 0xff) )
	{
	    #ifdef __IR_ACK_REQ__
		if (ReceiveBuffer.type == ACK)
		{
		    ACK_req = 0;
		    ACK_timeout = ACK_TIMEOUT;		/* stop timeout */
		    L_Data.confirm(ERIKA_COM_OK);	/* ACK received */
		}
		else
		    L_Data.request(0, NULL);	/* request ACK */
	    #else
    		L_Data.indication(ReceiveBuffer.len, ReceiveBuffer.buf);
	    #endif
	}
	ReceiveState = LookingForHeader;
    break;
    }    
    
    if (ReceiveState != LookingForHeader)
	RX_timeout = 0;
}



#ifdef __IR_SUPPORT__
void ERIKA_IR_timer( void )
{
    /* timeout between 2 bytes received */
    if (RX_timeout < RX_TIMEOUT)
    {
	RX_timeout += 2;
	if (RX_timeout >= RX_TIMEOUT)
	    ReceiveState = LookingForHeader;
    }
	
    #ifdef __IR_ACK_REQ__
	/* MAP timeout */
	if (ACK_timeout < ACK_TIMEOUT)
        {
    	    ACK_timeout += 2;
    	    if (ACK_timeout >= ACK_TIMEOUT)
	    {
		ReceiveState = LookingForHeader;
		ACK_req = 0;
		L_Data.confirm(ERIKA_COM_TIMEOUT);
	    }
	}
    #endif
	
    /* CFC timeout */
    if (CFC_timeout < CFC_TIMEOUT)
    {
	CFC_timeout += 2;
	if (CFC_TIMEOUT != NO_CFC && CFC_timeout >= CFC_TIMEOUT)
	{
	    CFC_delay = CFC_DELAY;
	    L_Data.confirm(ERIKA_COM_BUSY);
	}
	else
	{
	    #ifdef __IR_ACK_REQ__
		if (TransmitState == Idle && ReceiveState == LookingForHeader && ~ACK_req && ~TX_lock)
		{
		    if (CFC_delay >= CFC_DELAY)
			CFC_delay = 0;
		}
		else
		    CFC_delay = CFC_DELAY;
	    #else	
		if (TransmitState == Idle && ReceiveState == LookingForHeader && ~TX_lock)
		{
		    if (CFC_delay >= CFC_DELAY)
			CFC_delay = 0;
		}
		else
		    CFC_delay = CFC_DELAY;
	    #endif
	}
    }
    
    /* CFC delay */
    if (CFC_delay < CFC_DELAY)
    {
	CFC_delay += 2;
	if (CFC_delay >= CFC_DELAY)
	    L_Data.request(CFC_msg.len, CFC_msg.buf);
    }		
    
    if (Lock_timeout < Lock_TIMEOUT)
    {
	Lock_timeout += 2;
	if (Lock_timeout >= Lock_TIMEOUT)
	    TX_lock = 0;
    }	
}
#endif



void ERIKA_ERI_handler ( void )    /* Overrun, Frame or Parity Error */
{
    Error = ( SISSSR & ( bit5 | bit4 | bit3 ) ) >> 3;

    /* Clear Receive Data Register Full and error flags */
    SISSSR &= ~( bit6 | bit5 | bit4 | bit3 );  

    #ifdef __IR_SUPPORT__
	TransmitState = Idle; 
    #endif
    ReceiveState  = LookingForHeader;

    SISCR &= ~bit7; /* Disable TXI X */
    SISCR &= ~bit2; /* Disable TEIE X */

    #ifdef __IR_SUPPORT__
	ERIKA_IR_receive_disable();
	L_Data.confirm(ERIKA_COM_ERROR);  
    #endif
}
