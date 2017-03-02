#
#
#

ifndef BASE
BASE=../..
endif
include $(BASE)/config/config.mk

PROGS = monitor

include $(BASE)/config/example.mk

monitor:
	make -f $(SUBMAKE) APP=monitor INIT= OTHEROBJS="initfile.o IL_layer_conf.o" OTHERINCL= SHARKOPT="__OLDCHAR__ __GRX__"

