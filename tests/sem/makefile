# Semaphore Test
# 
# by Paolo Gai 2001-2002
#
# This file is distributed under the GPL license, see GPL.TXT for details
#
# Rules Usage: all (build into the out dir), clean (remove the out dir) 

# Compiling options
ERIKAOPT = __H8__ __MULTI__ __FP__ __SEM__ __TIMER_A_USER_FUNCTION_USED__ \
	    __LCD_USED__

# The name of the project 
PROJ    = sem

# architecture independent part
APP_SRCS= irq_tim.c    \
          code.c       \
          initkern.c

ifeq ($(findstring __H8__,$(ERIKAOPT)) , __H8__)
APP_SRCS+= inith8hal.c set_hand.s
endif

#
# --------------------------------------------------------------------------
#

include $(ERIKABASE)/config/config.mak









