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

#include <drivers/IL_layer_shark.h>

PID IL_PUT_pid;

int IL_alarm;

unsigned port = COM1;

void set_request()
{
    task_activate(IL_PUT_pid);
}

void reset_request()
{
    kern_event_delete(IL_alarm);
}

TASK IL_task(void)
{
    struct timespec frequency;
        
    BYTE msg[5];
    BYTE len;
    
    while(1)
    {
    	/* prepare the packet*/
	msg[0] = IL_put_data.node;
	msg[1] = IL_PUT;
	msg[2] = IL_put_data.index;
	
	if (IL_loc[IL_put_data.index].dim == 1)
	{
	    msg[3] = *(BYTE*)(IL_addr[IL_put_data.index]);
	    len = 4;
	}
	else
	{
	    msg[3] = (*(WORD*)(IL_addr[IL_put_data.index]) >> 8) & 0xFF;
	    msg[4] = *(WORD*)(IL_addr[IL_put_data.index]) & 0xFF;
	    len = 5;
	}
	
	/*send the packet*/
	IR_L_Data.request( len, msg);
	
	/*prepare for next wake up, if needed*/
	if (IL_put_data.freq > 0)
	{
	    sys_gettime(&frequency);
	    ADDUSEC2TIMESPEC( 1000*IL_put_data.freq, &frequency);
	    IL_alarm = kern_event_post( &frequency, set_request, NULL);
	}
	task_endcycle();
    }
}        

void IL_IR_init()
{
    HARD_TASK_MODEL IL_PUT_task;
    
    int CFC_set;
    
    hard_task_default_model(IL_PUT_task);
    hard_task_def_aperiodic(IL_PUT_task);
    hard_task_def_wcet(IL_PUT_task, 10000);
    hard_task_def_mit(IL_PUT_task, 100000);
    
    IL_PUT_pid = task_create("IL_task", IL_task, &IL_PUT_task, NULL);
    if (IL_PUT_pid == -1)
    {
	perror("IL_task");
	sys_end();
    }

    CFC_set = 500000;
    
    IR_init(port, ACK_REQ, &CFC_set);
}
