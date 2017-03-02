# Vehicle Test with Wireless Communication
# 
# by Michele Cirinei, Antonio Mancina, Davide Cantini, Paolo Gai 2001-2004
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
#ERIKAOPT += __PLAY_NOTE_SUPPORT__

## Play buffer
#ERIKAOPT += __PLAY_BUFFER_SUPPORT__

## motors with precise variable power
#ERIKAOPT += __MOTOR_C_PWM_USED__

## motors with precise variable power
#ERIKAOPT += __MOTOR_A_PWM_USED__


## motors with precise variable power
ERIKAOPT += __ERIKA_PWM_USED__
ERIKAOPT += __ERIKA_PWM_TOFF_BRAKE__
ERIKAOPT += __ERIKA_PWM_FAST__

## A/D converter
ERIKAOPT += __AD_SUPPORT__

## use double fixed point number
ERIKAOPT += __DOUBLE_FIXED_USED__


## time support
ERIKAOPT += __TIME_SUPPORT__

## timerB user function
ERIKAOPT += __TIMER_B_USER_FUNCTION_USED__

ERIKAOPT += __IL_SUPPORT__

# alarms
ERIKAOPT += __ALARMS__

### Project name (produce $(PROJ).srec)
PROJ = pathfollower

APP_SRCS = code.c inithal.c initkern.c set_hand.s per.c control.c mode.c

include $(ERIKABASE)/config/config.mak
