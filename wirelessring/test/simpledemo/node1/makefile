# Serial Communication Test
# 
# by Michele Cirinei, Antonio Mancina 2003-2004
#
# This file is distributed under the GPL license, see GPL.TXT for details
#
# Rules Usage: all (build into the out dir), clean (remove the out dir) 

###
### Setting ERIKAOPT variable:
###

ERIKAOPT =

## ROM data
## ERIKAOPT += __NO_ROM_DATA__

## Stack
ERIKAOPT += __MONO__

## debug
ERIKAOPT += __DEBUG__

## Scheduling
ERIKAOPT += __FP__

## Architecture
ERIKAOPT += __H8__

## Play notes
##ERIKAOPT += __PLAY_NOTE_SUPPORT__

## Play buffer
#ERIKAOPT += __PLAY_BUFFER_SUPPORT__

## time support
ERIKAOPT += __TIME_SUPPORT__

## timerB user function
ERIKAOPT += __TIMER_B_USER_FUNCTION_USED__

## IR support
ERIKAOPT += __IR_ACK_REQ__

##alarms
ERIKAOPT += __ALARMS__

### Project name (produce $(PROJ).srec)
PROJ = ir_test1

APP_SRCS = code.c inithal.c initkern.c set_hand.s per.c

include $(ERIKABASE)/config/config.mak
