/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: cabs.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */


#ifndef __INCLUDE_CABS_CABS_H__
#define __INCLUDE_CABS_CABS_H__

/* 

Cyclic Asynchronous Buffers (CABs) for the E.R.I.K.A. Project
-------------------------------------------------------------
CABs are a way to exchange data between different threads.

In particular, every thread always read the latest version of the data
that has been put in the CAB.

The number of buffers that have to be allocated into the CAB are given
by the following formula:

1 + number of readers + number of writers

-------------------------------------------------------------

To limit the overall usage of RAM memory, the CAB descriptor 
has been divided in two parts:

- ROM descriptor
	That contains the minimum set of references to the RAM data,
	plus the informations on the dimension and the number of buffers
- RAM descriptor
	That is composed by a set of arrays that contains the data to be
	exchanged, and of a few other variables to store the CAB status.

-------------------------------------------------------------

Usage:

First,  the CAB types have to be defined. These types are:
ERIKA_CAB_INDEX  An integer type that is used to index a single buffer in the CAB
ERIKA_CAB_DATA   A type used for storing data; usually it is a WORD

Every CAB buffer must be defined. There are two ways to define the CAB buffer:

- using the macro DEFINE_CAB
	That macro can be used to declare all the data structures needed.
	Note that after defining it, it must be initialized calling
	cab_init()
- statically initializing the data structure
	At the end of the development process, the macro can be substituted
	with its preprocessor initialization, allowing a proper separation
	between ROM and RAM parts; this way of initializing a CAB do not need
	the call to cab_init, since the data structures can be initialized
	inline.

Then you can use the cabs with the two pairs of functions that can be used to write
a message in the buffer / read the message in the buffer.


-------------------------------------------------------------

Compiling

To compile the CABS you should add __CABS__ in the ERIKAOPT variable of the makefile.

-------------------------------------------------------------

Optimized implementations:

Optimized implementations can be done. As usual, for each optimized version 
the corresponding symbol should be defined.

(Valid symbols are: __PRIVATE_CAB_INIT__ __PRIVATE_CAB_RESERVE__ __PRIVATE_CAB_PUTMES__
                    __PRIVATE_CAB_GETMES__ __PRIVATE_CAB_UNGET__ )

*/

struct ERIKA_cab_RAM_desc {
	ERIKA_CAB_INDEX mrd;		/* most recent data written on the CAB */
	ERIKA_CAB_INDEX free;		/* free buffer on the top of the stack */
};

struct ERIKA_cab_ROM_desc {
  struct ERIKA_cab_RAM_desc *ram_desc;	/* Most recent data / free message pointer */
  ERIKA_CAB_INDEX    *freestk;		/* Free list pointer */
  ERIKA_CAB_INDEX    *used;			/* CAB usage (pointer) */
  ERIKA_CAB_DATA     *msgs;			/* Message buffers (pointer) */

  ERIKA_CAB_INDEX    dim_msg;			/* dimension of each buffer in the CAB */
  ERIKA_CAB_INDEX    n_msg;			/* number of messages in the CAB */
};


#ifndef __PRIVATE_CAB_INIT__
/* These macros can be used to initalize a cab 
   That contains a message. The cab is initialized with a starting
   message (the index 0.
   Note that freestk has a dimension of the number of messages-1,
   since a message (the mrd) is always allocated.
*/

/* use this to define a CAB */
#define ERIKA_DEFINE_CAB(name, nmsg, dimmsg)                        \
	struct ERIKA_cab_RAM_desc name##_CABram = {nmsg-1, nmsg-1}; \
	ERIKA_CAB_INDEX name##_CABfreestk[nmsg-1];	              \
	ERIKA_CAB_INDEX name##_CABused[nmsg];		              \
	ERIKA_CAB_DATA name##_CABmsgs[nmsg*dimmsg];		      \
	const struct ERIKA_cab_ROM_desc name = {&name##_CABram,name##_CABfreestk,name##_CABused,name##_CABmsgs,dimmsg,nmsg}

/* this to refer it as an extern variable */
#define ERIKA_EXTERN_CAB(name) extern const struct ERIKA_cab_ROM_desc name

/* after defining a CAB, you must initialize it */
void ERIKA_cab_init(const struct ERIKA_cab_ROM_desc *c);
#endif

/* Writing a message on a CAB */
#ifndef __PRIVATE_CAB_RESERVE__
void ERIKA_cab_reserve(const struct ERIKA_cab_ROM_desc *c, void **msg, ERIKA_CAB_INDEX *msg_num);
#endif
#ifndef __PRIVATE_CAB_PUTMES__
void ERIKA_cab_putmes(const struct ERIKA_cab_ROM_desc *c, ERIKA_CAB_INDEX msg_num);
#endif



/* Reading a message from a CAB */
#ifndef __PRIVATE_CAB_GETMES__
void ERIKA_cab_getmes(const struct ERIKA_cab_ROM_desc *c, void **msg, ERIKA_CAB_INDEX *msg_num);
#endif
#ifndef __PRIVATE_CAB_UNGET__
void ERIKA_cab_unget(const struct ERIKA_cab_ROM_desc *c, ERIKA_CAB_INDEX msg_num);
#endif

#endif
