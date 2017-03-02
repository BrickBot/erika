## Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
##
## http://erika.sssup.it
##
## Copyright (C) 2001-2002 Alessandro Colantonio
##
## This program is distributed under the GPL license contained into the
## license.txt file on the E.R.I.K.A. root directory.
##
## CVS: $Id: check.mak,v 1.4 2004/07/13 12:19:10 cirinei Exp $

##
## Check default dependencies
##

#ifeq ($(findstring __MULTI__,$(ERIKAOPT)) , __MULTI__)
#ifeq ($(findstring __IRQ_STACK_NEEDED__,$(ERIKAOPT)) , __IRQ_STACK_NEEDED__)
#else
#ERIKAOPT += __IRQ_STACK_NEEDED__
#endif
#endif

ifeq ($(findstring __IRQ_STACK_NEEDED__,$(ERIKAOPT)) , __IRQ_STACK_NEEDED__)
ifeq ($(findstring __MULTI__,$(ERIKAOPT)) , __MULTI__)
else
ERIKAOPT += __MULTI__
endif
endif

ifeq ($(findstring __IL_SUPPORT__, $(ERIKAOPT)), __IL_SUPPORT__)
ifneq ($(findstring __IR_SUPPORT__, $(ERIKAOPT)), __IR_SUPPORT__)
ERIKAOPT += __IR_SUPPORT__
ERIKAOPT += __IR_ACK_REQ__
endif
endif

ifeq ($(findstring __IR_ACK_REQ__, $(ERIKAOPT)), __IR_ACK_REQ__)
ifneq ($(findstring __IR_SUPPORT__, $(ERIKAOPT)), __IR_SUPPORT__)
ERIKAOPT += __IR_SUPPORT__
endif
endif


chkerr:
ifeq ($(findstring __MONO__,$(ERIKAOPT)) , __MONO__)
ifeq ($(findstring __MULTI__,$(ERIKAOPT)) , __MULTI__)
	$(error __MULTI__ and __MONO__ options are not compatible)
endif
endif
ifeq ($(findstring __IRQ_STACK_NEEDED__,$(ERIKAOPT)) , __IRQ_STACK_NEEDED__)
ifeq ($(findstring __MONO__,$(ERIKAOPT)) , __MONO__)
	$(error __MONO__ and __IRQ_STACK_NEEDED__ options are not compatible)
endif
endif

ifeq ($(findstring __IL_SUPPORT__,$(ERIKAOPT)), __IL_SUPPORT__)
ifeq ($(findstring __IR_RX_SUPPORT__,$(ERIKAOPT)), __IR_RX_SUPPORT__)
	$(error __IL_SUPPORT__ and __IR_RX_SUPPORT__ options are not compatible)
endif
endif
ifeq ($(findstring __IR_ACK_REQ__,$(ERIKAOPT)), __IR_ACK_REQ__)
ifeq ($(findstring __IR_RX_SUPPORT__,$(ERIKAOPT)), __IR_RX_SUPPORT__)
	$(error __IR_ACK_REQ__ and __IR_RX_SUPPORT__ options are not compatible)
endif
endif

ifeq ($(findstring __IR_SUPPORT__,$(ERIKAOPT)), __IR_SUPPORT__)
ifeq ($(findstring __IR_RX_SUPPORT__,$(ERIKAOPT)), __IR_RX_SUPPORT__)
	$(error __IR_SUPPORT__ and __IR_RX_SUPPORT__ options are not compatible)
endif
endif
