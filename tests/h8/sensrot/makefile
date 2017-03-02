# Sensrot Test
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
##ERIKAOPT += __NO_ROM_DATA__

## Stack
ERIKAOPT += __MULTI__
##ERIKAOPT += __MONO__

## Scheduling
ERIKAOPT += __FP__

## Architecture
ERIKAOPT += __H8__

## Debug
##ERIKAOPT += __DEBUG__

## Play melodies (buffer)
#ERIKAOPT += __PLAY_BUFFER_SUPPORT__

## Play notes
ERIKAOPT += __PLAY_NOTE_SUPPORT__

## motors with variable velox
##ERIKAOPT += __MOTOR_C_PWM_USED__

## motors with precise variable velox
##ERIKAOPT += __MOTOR_B_VP_USED__

## motors with fixed (maximum) velox
ERIKAOPT += __MOTOR_A_VP_USED__

## A/D converter
ERIKAOPT += __AD_SUPPORT__

## timer A user function 
##ERIKAOPT += __TIMER_A_USER_FUNCTION_USED__

## timer B user function 
ERIKAOPT += __TIMER_B_USER_FUNCTION_USED__

## use timer A to activate periodic tasks 
#ERIKAOPT += __TIMER_A_PERIODIC_TASK_USED__

## use signed fixed point number
##ERIKAOPT += __SIGNED_FIXED_USED__

## use unsigned fixed point number
ERIKAOPT += __UNSIGNED_FIXED_USED__

## use double fixed point number
##ERIKAOPT += __DOUBLE_FIXED_USED__

## use signed fixed trigonometric functions
##ERIKAOPT += __SF_TRIG_USED__

## use signed fixed trig. functions with no input range control (high speed)
##ERIKAOPT += __SF_HIGH_SPEED_TRIG_USED__

## use double fixed trigonometric functions
##ERIKAOPT += __DF_TRIG_USED__

## use double fixed trig. functions with no input range control (high speed)
##ERIKAOPT += __DF_HIGH_SPEED_TRIG_USED__

### Project name (produce $(PROJ).srec )
PROJ = sensrot

APP_SRCS = code.c inithal.c initkern.c set_hand.s handlers.c

include $(ERIKABASE)/config/config.mak





