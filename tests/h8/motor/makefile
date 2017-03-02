# Semaphore Test
# 
# by Davide Cantini 2002
#
# This file is distributed under the GPL license, see GPL.TXT for details
#
# Rules Usage: all (build into the out dir), clean (remove the out dir) 

###
### Setting ERIKAOPT variable:
###

ERIKAOPT =

## ROM data
#ERIKAOPT += __NO_ROM_DATA__

## Stack
ERIKAOPT += __MONO__

## debug
##ERIKAOPT += __DEBUG__

## Scheduling
ERIKAOPT += __FP__

## Architecture
ERIKAOPT += __H8__

## Play notes
ERIKAOPT += __PLAY_NOTE_SUPPORT__

## Play buffer
##ERIKAOPT += __PLAY_BUFFER_SUPPORT__

## PWM motor control 
ERIKAOPT += __MOTOR_A_VP_USED__

## PWM motor control
ERIKAOPT += __MOTOR_C_VP_USED__

## A/D converter
##ERIKAOPT += __AD_SUPPORT__

## 8bit timer channel 1 support
##ERIKAOPT += __T81_SUPPORT__

## use double fixed point number
##ERIKAOPT += __DOUBLE_FIXED_USED__

## this application use timer B user function for periodic thread activation
ERIKAOPT += __TIMER_B_USER_FUNCTION_USED__

### Project name (produce $(PROJ).srec)
PROJ = motor.aaa

APP_SRCS = code.c inithal.c initkern.c set_hand.s handlers.c

include $(ERIKABASE)/config/config.mak





