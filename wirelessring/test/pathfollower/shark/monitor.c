/*
 * Project: S.Ha.R.K.
 *
 * Coordinators: 
 *   Giorgio Buttazzo    <giorgio@sssup.it>
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *
 * Authors     : 
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *   (see the web pages for full authors list)
 *
 * ReTiS Lab (Scuola Superiore S.Anna - Pisa - Italy)
 *
 * http://www.sssup.it
 * http://retis.sssup.it
 * http://shark.sssup.it
 */

/*
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 Path Follower test with Wireless Ring Communication.

 Through the graphic interface you can send requests to a path follower or a 
 repeater (both with ERIKA OS), and visualize data about them. You can also
 send commands to the repeater which send them to the path follower.
*/

#include <kernel/kern.h>
#include <drivers/keyb.h>
#include <drivers/mouse.h>
#include <modules/hartport.h>
#include <drivers/glib.h>
#include "IL_layer_conf.h"
#include <drivers/IL_layer.h>

#include <stdlib.h>

#include <string.h>


/************************************************************************
* Definitions								*
************************************************************************/
/*********************************
* General definitions 
*********************************/
#define BUF_DIM 256
#define FREQ 500000	/* microseconds, to quote graph */

#define BG_COLOR BLACK
#define FG_COLOR WHITE

#define TIME_STEP 10
#define GRAPH_STEP 40
#define GRAPH_SCALER 1
#define XRES 1024
#define YRES 768

/*********************************
* Graphic
*********************************/
#define xminGRX 70	/* at least 50 */
#define xmaxGRX 899
#define yminGRX 20	/* at least 10 */
#define ymaxGRX (yminGRX+260*2-1)

#define xtotGRX (xmaxGRX-xminGRX+1)
#define ytotGRX (ymaxGRX-yminGRX+1)
#define ymidGRX (ytotGRX/2+yminGRX)

/*********************************
* Text 1 = received from car 1
*********************************/
#define xminCAR (xmaxGRX+11)
#define xmaxCAR 999
#define yminCAR yminGRX
#define ymaxCAR (((ymaxGRX-yminGRX)/2)+yminGRX-10)

#define xtotCAR (xmaxCAR-xminCAR+1)
#define ytotCAR (ymaxCAR-yminCAR+1)

/*********************************
* Text 2 = received from car 2
*********************************/
#define xminCAR2 (xmaxGRX+11)
#define xmaxCAR2 999
#define yminCAR2 ymaxCAR+20
#define ymaxCAR2 ymaxGRX

#define xtotCAR2 (xmaxCAR2-xminCAR2+1)
#define ytotCAR2 (ymaxCAR2-yminCAR2+1)

/*********************************
* Data sent
*********************************/
#define xminSNT (xmaxGRX+10)
#define xmaxSNT xmaxCAR
#define yminSNT (ymaxGRX+20)
#define ymaxSNT 759

#define xtotSNT (xmaxSNT-xminSNT+1)
#define ytotSNT (ymaxSNT-yminSNT+1)


/*********************************
* Variables rect
*********************************/
#define xVAR	4
#define yVAR	4

#define xminVAR xminGRX
#define xmaxVAR xminVAR+310
#define yminVAR (ymaxGRX+60)
#define ymaxVAR 750

#define xtotVAR (xmaxVAR-xminVAR+1)
#define ytotVAR (ymaxVAR-yminVAR+1)

/*********************************
* Single Variable Dimensions
*********************************/
//#define xtotSVD (xtotVAR/xVAR)
#define xtotSVD 150
#define ytotSVD (ytotVAR/yVAR)

#define xminSVD xminVAR
#define xmaxSVD (xminSVD+xtotSVD-1)
#define yminSVD yminVAR
#define ymaxSVD (yminSVD+ytotSVD-1)

/*********************************
* Periodic Get Frequency
*********************************/
#define xminFRQ xmaxVAR
#define xmaxFRQ xmaxVAR+100
#define yminFRQ yminVAR
#define ymaxFRQ ymaxVAR

#define xtotFRQ (xmaxFRQ-xminFRQ+1)
#define ytotFRQ (ymaxFRQ-yminFRQ+1) 

/*********************************
* Commands
*********************************/
#define xminCMD xmaxFRQ
#define xmaxCMD xminCMD+100
#define yminCMD yminFRQ
#define ymaxCMD ymaxFRQ

#define xtotCMD (xmaxCMD-xminCMD+1)
#define ytotCMD (ymaxCMD-yminCMD+1) 
/************************************************************************
* Variabili								*
************************************************************************/
#define CAR 	1 	/* repeater */
#define CAR2 	2 	/* path follower */
#define COMMAND 0

extern BYTE TX_lock;

PID getchar_pid;
PID graph_pid;
PID gest_pid;

int user_command = -1;	/* last command requested via mouse */

BYTE sent_command = 1;
BYTE freq_val = 0;

BYTE buffer[]={0x05, 0x10, 0x15};

BYTE *img;
BYTE *txt[3];

struct rx{
    short buf;
    BYTE var;
};

struct rx rx1[BUF_DIM];
struct rx rx2[BUF_DIM];

int buf_r1_ptr = 0;
int buf_w1_ptr = 0;

int buf_r2_ptr = 0;
int buf_w2_ptr = 0;

char* VAR_name[] = { 
    "Right Motor 1", 
    "Left Motor 1", 
    "Difference 1", 
    "Send Command",
    "Right Motor 2",
    "Left Motor 2",
    "Difference 2"
};

char* CMD_name[] = {
    "STOP",
    "SLOW",
    "FAST",
    "FAULT",
    "FAULT 2",
    "RECOVERY"
};

char* freq_name[] = {
    "ONE-SHOT",
    "0.25 sec",
    "0.50 sec",
    "0.75 sec",
    "1.00 sec",
    "1.25 sec"
};

#define W FG_COLOR

BYTE mouse_cursor[16*16] = {
W,W,W,W,W,W,W,W,0,0,0,0,0,0,0,0,
W,W,W,W,W,W,0,0,0,0,0,0,0,0,0,0,
W,W,W,W,0,0,0,0,0,0,0,0,0,0,0,0,
W,W,W,W,0,0,0,0,0,0,0,0,0,0,0,0,
W,W,0,0,W,0,0,0,0,0,0,0,0,0,0,0,
W,W,0,0,0,W,0,0,0,0,0,0,0,0,0,0,
W,0,0,0,0,0,W,0,0,0,0,0,0,0,0,0,
W,0,0,0,0,0,0,W,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,W,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,W,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,W,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,W,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

#define BG 0xff

BYTE mouse_mask[16*16] = {
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,BG,
};

/************************************************************************
* Funzioni								*
************************************************************************/
    
void buf_init(void)
{
    /* uses buf_w1_ptr for both because is only to initialize them */
    
    for (buf_w1_ptr = 0; buf_w1_ptr <BUF_DIM; buf_w1_ptr++)
    {
	rx1[buf_w1_ptr].buf = 0;				
	rx1[buf_w1_ptr].buf = 0;
	rx2[buf_w1_ptr].buf = 0;
	rx2[buf_w1_ptr].buf = 0;
    }
    
    buf_w1_ptr = 0;
    buf_r1_ptr = 0;
    buf_w2_ptr = 0;
    buf_r2_ptr = 0;
    

}

void task_name(char *name)					/* scrive il nome del task attivo */
{
    grx_box(xmaxGRX+11,yminGRX,xmaxGRX+81,yminGRX+7,BG_COLOR);
    grx_text(name, xmaxGRX+11, yminGRX, FG_COLOR, BG_COLOR);
}

void grx_num(int *num, WORD x, WORD y, DWORD fg, DWORD bg)	/* scrive un intero in modalita' grafica */
{
    char *str=NULL;

    sprintf(str,"%4i",*num);
    grx_text(str, x, y, fg, bg);
}

void confirm(BYTE msg_id)
{
    switch (msg_id)
    {
	case 0: grx_text("TX OK", xminSNT+10, yminSNT+10, FG_COLOR, BG_COLOR);
		break;
	case 1: grx_text("TX ERROR", xminSNT+10, yminSNT+10, FG_COLOR, BG_COLOR);
		break;
	case 2: grx_text("IR BUSY", xminSNT+10, yminSNT+10, FG_COLOR, BG_COLOR);
		break;
	case 3: grx_text("NO ACK", xminSNT+10, yminSNT+10, FG_COLOR, BG_COLOR);
		break;
	case 4: grx_text("TIMEOUT", xminSNT+10, yminSNT+10, FG_COLOR, BG_COLOR);
		break;
    }		
}

void indication(BYTE msg)
{
    if (IL_loc[IL_received.index].node == CAR)
    {
	rx1[buf_w1_ptr].var = IL_received.index;
	rx1[buf_w1_ptr++].buf = IL_received.buffer[0];
    }
    if (IL_loc[IL_received.index].node == CAR2)
    {
	rx2[buf_w2_ptr].var = IL_received.index;
	rx2[buf_w2_ptr++].buf = IL_received.buffer[0];
    }
    
    if (buf_w1_ptr == BUF_DIM)
        buf_w1_ptr = 0;
    if (buf_w2_ptr == BUF_DIM)
        buf_w2_ptr = 0;
}

void init_screen(void)						/* disegna la schermata grafica */
{
    int x, y;
    int val;
    
    mouse_off();
    
    /* graph */
    grx_text("IR RX GRAPH", xminGRX-40, yminGRX-10, FG_COLOR, BG_COLOR);
    grx_rect(xminGRX-1,yminGRX-1,xmaxGRX+1,ymaxGRX+1,FG_COLOR);
    grx_rect(xminGRX-2,yminGRX-2,xmaxGRX+2,ymaxGRX+2,FG_COLOR);

    /* quote graph */
    val = 0;
    for (y=ymidGRX; y<ymaxGRX; y+=GRAPH_STEP)
    {
	grx_num(&val, (xminGRX-50), (y - 3), FG_COLOR, BG_COLOR);
	val += GRAPH_STEP * GRAPH_SCALER;
	grx_line( xminGRX-5, y, xminGRX, y, FG_COLOR);	
	grx_line( xminGRX, y, xmaxGRX, y, BLUE);
    }
    val = -GRAPH_STEP;
    for (y=ymidGRX-GRAPH_STEP; y>yminGRX; y-=GRAPH_STEP)
    {
	grx_num(&val, (xminGRX-50), (y - 3), FG_COLOR, BG_COLOR);
	val -= GRAPH_STEP * GRAPH_SCALER;
	grx_line( xminGRX-5, y, xminGRX, y, FG_COLOR);	
	grx_line( xminGRX, y, xmaxGRX, y, BLUE);
    }
    
    for (x=xminGRX+TIME_STEP;x<xmaxGRX;x+=2*TIME_STEP)
	grx_line(x, ymaxGRX+2, x, ymaxGRX+5, FG_COLOR);
    
    img = malloc(xtotGRX*ytotGRX);
    grx_getimage(xminGRX, yminGRX, xmaxGRX, ymaxGRX, img);

    /* Data received from car 1*/
    grx_text("Car 1", xminCAR, yminCAR-10, FG_COLOR, BG_COLOR);
    grx_rect(xminCAR-1,yminCAR-1,xmaxCAR+1,ymaxCAR+1,FG_COLOR);
    grx_rect(xminCAR-2,yminCAR-2,xmaxCAR+2,ymaxCAR+2,FG_COLOR);

    txt[0] = malloc(xtotCAR*ytotCAR);
    grx_getimage(xminCAR, yminCAR, xmaxCAR, ymaxCAR, txt[0]);

    /* Data received from car 2*/
    grx_text("Car 2", xminCAR2, yminCAR2-10, FG_COLOR, BG_COLOR);
    grx_rect(xminCAR2-1,yminCAR2-1,xmaxCAR2+1,ymaxCAR2+1,FG_COLOR);
    grx_rect(xminCAR2-2,yminCAR2-2,xmaxCAR2+2,ymaxCAR2+2,FG_COLOR);

    txt[1] = malloc(xtotCAR2*ytotCAR2);
    grx_getimage(xminCAR2, yminCAR2, xmaxCAR2, ymaxCAR2, txt[1]);

    /* Data sent to car */
    grx_text("SENT", xminSNT, yminSNT-10, FG_COLOR, BG_COLOR);
    grx_rect(xminSNT-1,yminSNT-1,xmaxSNT+1,ymaxSNT+1,FG_COLOR);
    grx_rect(xminSNT-2,yminSNT-2,xmaxSNT+2,ymaxSNT+2,FG_COLOR);

    txt[2] = malloc(xtotSNT*ytotSNT);
    grx_getimage(xminSNT, yminSNT, xmaxSNT, ymaxSNT, txt[2]);

    /* vars */
    grx_text("Variables", xminVAR, yminVAR-10, FG_COLOR, BG_COLOR);
    x = 0;
    y = 0;
    for (val = 0; val < 7; val++)
    {
	grx_rect(xminSVD + xtotSVD * x, yminSVD + ytotSVD * y, xmaxSVD + xtotSVD * x, ymaxSVD + ytotSVD * y, FG_COLOR);
	grx_text(VAR_name[val], xminSVD + xtotSVD * x + 15, yminSVD + ytotSVD * y + 15, FG_COLOR, BG_COLOR);
	if (ymaxSVD + ytotSVD * (y+1) < YRES) /* c'e' ancora posto sotto alla variabile precedente */
	    y++;
	else
	{
	    if (xmaxSVD + xtotSVD * (x+1) < xmaxGRX) /* c'e' ancora posto accanto alla colonna precedente */
	    {
		x++;
		y=0;
	    }
	    else
	    {
		grx_text("Troppe variabili richieste, verificare i file di configurazione.", 0,760,FG_COLOR,BG_COLOR);
		val = IL_MAX_VAR;
	    }
	}
    }
    
    /* commands */
    grx_text("Commands", xminCMD+10, yminCMD-10, FG_COLOR, BG_COLOR);

    for (val = 0; val <= 5; val++)
    {
        grx_rect(xminCMD, yminCMD + (ytotCMD / 6) * val, xmaxCMD, yminCMD + (ytotCMD / 6) * (val+1), FG_COLOR);
	grx_text(CMD_name[val], xminCMD + 10, yminCMD + (ytotCMD / 6) *val + 10, FG_COLOR, BG_COLOR);
        grx_rect(xmaxCMD-20, yminCMD + (ytotCMD / 6) * val + 5, xmaxCMD-5, yminCMD + (ytotCMD / 6) * (val+1) - 5, FG_COLOR);
    }

    /* frequency */
    grx_text("Period", xminFRQ+10, yminFRQ-10, FG_COLOR, BG_COLOR);
    
    for (val = 0; val <= 5; val++)
    {
        grx_rect(xminFRQ, yminFRQ + (ytotFRQ / 6) * val, xmaxFRQ, yminFRQ + (ytotFRQ / 6) * (val+1), FG_COLOR);
	grx_text(freq_name[val], xminFRQ + 10, yminFRQ + (ytotFRQ / 6) *val + 10, FG_COLOR, BG_COLOR);
        grx_rect(xmaxFRQ-20, yminFRQ + (ytotFRQ / 6) * val + 5, xmaxFRQ-5, yminCMD + (ytotFRQ / 6) * (val+1) - 5, FG_COLOR);
    }
    
    mouse_on();
}

BYTE mouse_pressed = 0;

void mouse_hand(MOUSE_EVT *event)			/* handler del mouse */
{
    int x = (*event).x;
    int y = (*event).y;
    int index = 0;

    if (isLeftButton(*event))
    {	
	if (!mouse_pressed)
	{
	    mouse_pressed = 1;
    	    if (x > xminVAR && x < xmaxVAR && y > yminVAR && y < ymaxVAR)	/* variables */
	    {
		index = ((int)((x-xminVAR)/xtotSVD))*yVAR;	/* column */
		index += (int)((y-yminVAR)/ytotSVD);
		switch (index)
		{
		    case 0: user_command = 1;
			break;
		    case 1: user_command = 2;
			break;
		    case 2: user_command = 3;
			break;
		    case 3: user_command = 0;
			break;
		    case 4: user_command = 5;
			break;
		    case 5: user_command = 6;
			break;
		    case 6: user_command = 7;
			break;
		    default:user_command = -1;
			break;
		}
	    }
	    else
	    {
		if (x > xminCMD && x < xmaxCMD && y > yminCMD && y < ymaxCMD)	/* commands */
		    sent_command = (int)( (y-yminCMD)/(ytotCMD/6) );
		if (x > xminFRQ && x < xmaxFRQ && y > yminFRQ && y < ymaxFRQ)	/* frequency */
		    freq_val = (int)( (y-yminFRQ)/(ytotFRQ/6) );

	    }
	}
    }
    else
    {
	mouse_pressed = 0;
    }
}

void init_mouse(void)					/* inizializzazione del mouse */
{
    mouse_grxshape(mouse_cursor, mouse_mask);
    mouse_grxcursor(ENABLE);
    mouse_grxlimit(XRES,YRES);
    mouse_position(100,750);
    mouse_on();
    mouse_hook(mouse_hand);
}

void byebye(void * arg)					/* funzione di sysend */
{
    IL_end();
    grx_close();
    mouse_end();
    cprintf("\nEnd of test.\n");
}

/************************************************************************
* Task									*
************************************************************************/

TASK getchar(void)					/* legge i caratteri da tastiera */
{
    BYTE c;
    while(1)
    {
	c = keyb_getch(BLOCK);
	if (c == 27) /*ESC*/
	    sys_end();
	else         /* gestiti come ricezioni*/
	{
	    rx1[buf_w1_ptr++].buf = (signed char) c;
	    if (buf_w1_ptr == BUF_DIM)
		buf_w1_ptr = 0;
	}
    }
}

TASK graph(void)
{
    int y;

    char rcv[5]="     ";    

    int index1 = 0;
    int pos1 = 0;
    int value1;
    int flag1 = 0;

    int index2 = 0;
    int pos2 = 0;
    int value2;
    int flag2 = 0;

    rcv[9] = 0;
    
    while (1)
    {
	mouse_off();

	/* prepare new graph image */
	grx_getimage(xminGRX, yminGRX, xmaxGRX, ymaxGRX, img);
        for (y=0; y<ytotGRX; y++)
	    memmove( &img[y*xtotGRX], &img[y*xtotGRX+TIME_STEP], xtotGRX-TIME_STEP);

	grx_putimage(xminGRX, yminGRX, xmaxGRX, ymaxGRX, img);    	

	grx_box( xmaxGRX-TIME_STEP, yminGRX, xmaxGRX, ymaxGRX, BG_COLOR);
	for(y = ymidGRX; y<ymaxGRX; y+=GRAPH_STEP)
	    grx_line(xmaxGRX-TIME_STEP, y, xmaxGRX, y, BLUE);
	for(y = ymidGRX-GRAPH_STEP; y>yminGRX; y-=GRAPH_STEP)
	    grx_line(xmaxGRX-TIME_STEP, y, xmaxGRX, y, BLUE);
	
	
	/* prepare new text image 1*/
	grx_getimage(xminCAR, yminCAR, xmaxCAR, ymaxCAR, txt[0]);
	memmove( &txt[0][11*xtotCAR], txt[0], xtotCAR*(ytotCAR-11));
	grx_putimage(xminCAR, yminCAR, xmaxCAR, ymaxCAR, txt[0]);    	
	grx_box( xminCAR, yminCAR, xmaxCAR, yminCAR+11, BG_COLOR);

	/* prepare new text image 2*/
	grx_getimage(xminCAR2, yminCAR2, xmaxCAR2, ymaxCAR2, txt[1]);
	memmove( &txt[1][11*xtotCAR2], txt[1], xtotCAR2*(ytotCAR2-11));
	grx_putimage(xminCAR2, yminCAR2, xmaxCAR2, ymaxCAR2, txt[1]);    	
	grx_box( xminCAR2, yminCAR2, xmaxCAR2, yminCAR2+11, BG_COLOR);

	/* prepare new text image 3*/
	grx_getimage(xminSNT, yminSNT, xmaxSNT, ymaxSNT, txt[2]);
	memmove( &txt[2][11*xtotSNT], txt[2], xtotSNT*(ytotSNT-11));
	grx_putimage(xminSNT, yminSNT, xmaxSNT, ymaxSNT, txt[2]);    	
	grx_box( xminSNT, yminSNT, xmaxSNT, yminSNT+11, BG_COLOR);

	/* process new data, only last is significant */
	value1 = 0;
	value2 = 0;

	while ((buf_r1_ptr != buf_w1_ptr))
	{
	    index1 = rx1[buf_r1_ptr].var;
	    value1 = rx1[buf_r1_ptr++].buf;
	    flag1 = 1;
	}
	while ((buf_r2_ptr != buf_w2_ptr))
	{
	    index2 = rx2[buf_r2_ptr].var;
	    value2 = rx2[buf_r2_ptr++].buf;
	    flag2 = 1;
	}

	if ((ymidGRX-value1/GRAPH_SCALER) > ymaxGRX)
	    value1 = (ymidGRX - ymaxGRX) * GRAPH_SCALER + 10;
	if ((ymidGRX-value1/GRAPH_SCALER) < yminGRX)
	    value1 = (ymidGRX - yminGRX) * GRAPH_SCALER - 10;

	if ((ymidGRX-value2/GRAPH_SCALER) > ymaxGRX)
	    value2 = (ymidGRX - ymaxGRX) * GRAPH_SCALER + 10;
	if ((ymidGRX-value2/GRAPH_SCALER) < yminGRX)
	    value2 = (ymidGRX - yminGRX) * GRAPH_SCALER - 10;

	/* graph line 1 */
	grx_line(xmaxGRX-TIME_STEP,ymidGRX-pos1/GRAPH_SCALER,xmaxGRX,ymidGRX-value1/GRAPH_SCALER,RED);
	pos1 = value1;

	/* text line 1 */
	if ( flag1 )
	{
	    flag1 = 0;
 	    grx_num(&value1, xminCAR+55, yminCAR+2, FG_COLOR, BG_COLOR);
	    switch (index1)
	    {
		case 1:grx_text("R-1: ",xminCAR+5, yminCAR+2, FG_COLOR, BG_COLOR);
		break;
		case 2:grx_text("L-1: ",xminCAR+5, yminCAR+2, FG_COLOR, BG_COLOR);
		break;
		case 3:grx_text("D-1: ",xminCAR+5, yminCAR+2, FG_COLOR, BG_COLOR);
		break;
	    }		
	}

	/* graph lines 2*/
	grx_line(xmaxGRX-TIME_STEP,ymidGRX-pos2/GRAPH_SCALER,xmaxGRX,ymidGRX-value2/GRAPH_SCALER,GREEN);
	pos2 = value2;

	/* text line 2 */
	if ( flag2)
	{
	    flag2 = 0;
	    grx_num(&value2, xminCAR2+55, yminCAR2+2, FG_COLOR, BG_COLOR);
	    switch (index2)
	    {
		case 5:grx_text("R-2: ",xminCAR2+5, yminCAR2+2, FG_COLOR, BG_COLOR);
		break;
		case 6:grx_text("L-2: ",xminCAR2+5, yminCAR2+2, FG_COLOR, BG_COLOR);
		break;
		case 7:grx_text("D-2: ",xminCAR2+5, yminCAR2+2, FG_COLOR, BG_COLOR);
		break;
	    }		
	}
	
	/* command */
	for( y = 0; y <= 5; y++)
	{
	    if (y == sent_command)
    		grx_box(xmaxCMD-19, yminCMD + (ytotCMD / 6) * y + 6, xmaxCMD-6, yminCMD + (ytotCMD / 6) * (y+1) - 6, RED);
	    else
    		grx_box(xmaxCMD-19, yminCMD + (ytotCMD / 6) * y + 6, xmaxCMD-6, yminCMD + (ytotCMD / 6) * (y+1) - 6, BG_COLOR);
        }

	/* period */
	for( y = 0; y <= 5; y++)
	{
	    if (y == freq_val)
    		grx_box(xmaxFRQ-19, yminFRQ + (ytotFRQ / 6) * y + 6, xmaxFRQ-6, yminFRQ + (ytotFRQ / 6) * (y+1) - 6, RED);
	    else
    		grx_box(xmaxFRQ-19, yminFRQ + (ytotFRQ / 6) * y + 6, xmaxFRQ-6, yminFRQ + (ytotFRQ / 6) * (y+1) - 6, BG_COLOR);
        }

	mouse_on();     
	task_endcycle();
    }
}

TASK gest(void)
{
    int result = 0;
    while(1)
    {
	switch(user_command)
	{
	    case 1: case 2: case 3: case 5: case 6: case 7:
		    result = IL_get(user_command, freq_val*(FREQ/1000)/2);
		    user_command = -1;
		    break;
	    case 0: result = IL_put(CAR, COMMAND, 0);
		    user_command = -1;
		    break;
	    default: user_command = -1;
		    break;

	}
	task_endcycle();
    }

}
/************************************************************************
* Main									*
************************************************************************/

int main(int argc, char **argv)
{
    NRT_TASK_MODEL getchar_task;	/* simula la ricezione */
    SOFT_TASK_MODEL graph_task;		/* task grafico, riporta i dati sullo schermo */
    HARD_TASK_MODEL gest_task;		/* task di gestione delle richieste utente */
    
    srand(kern_gettime(NULL));
    
    buf_init();
    
    sys_atrunlevel(byebye, NULL, RUNLEVEL_BEFORE_EXIT);
    
    nrt_task_default_model(getchar_task);
    nrt_task_def_group(getchar_task, 1);

    soft_task_default_model(graph_task);
    soft_task_def_met(graph_task,  100000);
    soft_task_def_period(graph_task, FREQ);
    soft_task_def_group(graph_task, 1);

    hard_task_default_model(gest_task);
    hard_task_def_wcet(gest_task,100000);
    hard_task_def_mit(gest_task, 300000);
    hard_task_def_group(gest_task, 1);
    
    getchar_pid = task_create("Getchar", getchar, &getchar_task, NULL);
    if (getchar_pid == -1)
    {
	perror("gettask");
	sys_end();
    }

    graph_pid = task_create("Graph", graph, &graph_task, NULL);
    if (graph_pid == -1)
    {
	perror("graph");
	sys_end();
    }

    gest_pid = task_create("Gest", gest, &gest_task, NULL);
    if (gest_pid == -1)
    {
	perror("gest");
	sys_end();
    }
    
    IL_init();
    IL_set_confirm(confirm, IL_FILTER_ALL);    
    IL_set_indication(indication);

    IL_pic_config();

    /* Graphic init */
    grx_init();
    grx_open(1024,768,8);
    grx_clear(BG_COLOR);
    init_mouse();
    init_screen();

    /* Task activate */
    task_activate(getchar_pid);
    task_activate(graph_pid);
    task_activate(gest_pid);

    return 0;
}
