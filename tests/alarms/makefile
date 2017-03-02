# ALARMS Test Makefile
# 
# by Paolo Gai 2002
#
# Rules Usage: all (build into the out dir), clean (remove the out dir) 

# Compiling options

ERIKAOPT = __H8__ __LCD_USED__ __PLAY_NOTE_SUPPORT__

ERIKAOPT += __MONO__ __FP__ __ALARMS__ __FP_NO_MUTEX__

# The name of the project 
PROJ    = alarms

#
# --------------------------------------------------------------------------
#

# architecture independent part
APP_SRCS = code.c initkern.c

# architecture dependent part

ifeq ($(findstring __H8__,$(ERIKAOPT)) , __H8__)
APP_SRCS += inith8.c set_hand.s
endif

#
# --------------------------------------------------------------------------
#

include $(ERIKABASE)/config/config.mak
