#ifndef __IL_LAYER_DEF_H__
#define __IL_LAYER_DEF_H__

/****************************************************************
* Constants							*
****************************************************************/
#define MIN_FREQ		100
#define ERR_INDEX_RANGE		-1
#define ERR_NODE		-2
#define ERR_FREQ		-3

#define IL_FILTER_OK		(1 << COM_OK)
#define IL_FILTER_ERROR		(1 << COM_ERROR)
#define IL_FILTER_BUSY		(1 << COM_BUSY)
#define IL_FILTER_NOACK		(1 << COM_NOACK)
#define IL_FILTER_TIMEOUT	(1 << COM_TIMEOUT)
#define	IL_FILTER_ALL		(IL_FILTER_OK | IL_FILTER_ERROR | IL_FILTER_BUSY | IL_FILTER_NOACK | IL_FILTER_TIMEOUT)
#define IL_FILTER_ALL_ERRORS	(IL_FILTER_ERROR | IL_FILTER_BUSY | IL_FILTER_NOACK | IL_FILTER_TIMEOUT)
#define IL_FILTER_NONE		0

#define IL_STOP			0xFF

#define IL_GET_ONCE		(1 << 0)
#define IL_GET_PER		(1 << 1)
#define IL_PUT			(1 << 2)
#define IL_AUTO_GET_ONCE	(1 << 3)
#define IL_AUTO_GET_PER		(1 << 4)
#define IL_INDICATION_ALL	(IL_GET_ONCE | IL_GET_PER | IL_PUT)
#define IL_AUTO_ALL		(IL_AUTO_GET_ONCE | IL_AUTO_GET_PER)

/****************************************************************
* Data 								*
****************************************************************/
struct IL_received_t{
    ONE_BYTE index;
    TWO_BYTES buffer[MaxData-3];
};

struct IL_data_t{
    ONE_BYTE node;
    ONE_BYTE index;
    TWO_BYTES freq;
};

struct IL_loc_t{
    ONE_BYTE node;
    ONE_BYTE dim;
};

extern struct IL_data_t IL_put_data;

extern struct IL_loc_t IL_loc[];

extern void* IL_addr[];

extern ONE_BYTE IL_flag[];

#endif
