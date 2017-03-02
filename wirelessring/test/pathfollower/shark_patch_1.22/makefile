# The Char Devices (mouse, serial ports, keyboard)

ifndef BASE
BASE=../..
endif

include $(BASE)/config/config.mk

LIBRARY       = oldch

OBJS_PATH     = $(BASE)/drivers/oldchar

OBJS  = rtc.o keyb.o scom.o crtwin.o mouse.o sermouse.o ps2mouse.o 8042.o \
	mcurtxt.o mcurgrx.o scomirq.o IR_layer.o IL_layer.o IL_layer_shark.o

OTHERINCL += -I$(BASE)/drivers/oldchar/include -I$(BASE)/drivers/grx/include

include $(BASE)/config/lib.mk

