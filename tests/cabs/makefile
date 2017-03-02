# CABS Test Makefile
# 
# by Paolo Gai 2001-2002
#
# This file is distributed under the GPL license, see GPL.TXT for details
#
# Rules Usage: all (build into the out dir), clean (remove the out dir) 

# Compiling options
ERIKAOPT = __H8__ __TIMER_A_USER_FUNCTION_USED__ __LCD_USED__

ERIKAOPT += __MONO__ __FP__ __CABS__

# The name of the project 
PROJ    = cabs

#
# --------------------------------------------------------------------------
#

# architecture independent part
APP_SRCS = code.c initkern.c mycabs.c

ifeq ($(findstring __H8__,$(ERIKAOPT)) , __H8__)
APP_SRCS += inith8.c irq_h8.c set_hand.s
endif

#
# --------------------------------------------------------------------------
#

include $(ERIKABASE)/config/config.mak
