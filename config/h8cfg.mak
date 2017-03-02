## Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
##
## http://erika.sssup.it
##
## Copyright (C) 2002 Davide Cantini
##
## This program is distributed under the GPL license contained into the
## license.txt file on the E.R.I.K.A. root directory.
##
## CVS: $Id: h8cfg.mak,v 1.7 2004/07/13 12:19:10 cirinei Exp $

## Global paths
##

# H8BASE is the base directory of development environment
# it should be defined in your .bashrc
# H8BASE = 

# ERIKABASE is the base directory of the kernel
# it should be defined in your .bash_profile
# ERIKABASE = 

# APPBASE is the base directory of the application
APPBASE = $(CURDIR)

# BINDIR is the directory of assembler, compiler, linker...
BINDIR = $(H8BASE)/h8300-hitachi-hms/bin

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
ALLINCPATH = -I$(ERIKABASE)/include -I$(APPBASE)

# LOADFILE is the path of configuration file for ld command
ifeq ($(findstring __NO_ROM_DATA__, $(ERIKAOPT)), __NO_ROM_DATA__)
LOADFILE = $(ERIKABASE)/etc/h8/h8cfgNRD.lds
else
LOADFILE = $(ERIKABASE)/etc/h8/h8cfgRD.lds
endif


# GNU compilers
H8LINK=$(BINDIR)/ld
H8ASM=$(BINDIR)/as
H8CC=$(BINDIR)/gcc
H8CPP=$(BINDIR)/gcc
H8AR=$(BINDIR)/ar

## OPTH8CC are the options for H8 gcc compiler invocation
# -O                = select the optimization level
#                     NOTE that -O0 can NOT be used because it not expands
#                     inline functions!!!!
# -c                = compile only (do not link)
# -Wall		    = all compiling warning
# -Winline	    = warn if a function does not expand inline
OPTH8CC = -O2 -Wall -Wno-unused -Winline -c $(ALLINCPATH)

# CPP: we MUST use -fno-rtti because if used the compiler generates a LOT of
# code :-(
OPTH8CPP = -O2 -Wall -Wno-unused -Winline -fno-rtti -fno-exceptions -c $(ALLINCPATH)

## OPTH8ASM are the options for H8 asm invocation
OPTH8ASM = 

# OPTH8LINK represents the options for H8 link invocation
OPTH8LINK =  -T$(LOADFILE) -u__start

# OPTH8LIBS is used to link additional libraries (e.g., for C++ support)
OPTH8LIBS = -lerika -L $(APPBASE)/out


# Defining ERIKAOPT Macros
# Each identifier that is listed in ERIKAOPT is also inserted as a 
# command-line macro in the compiler...
OPTH8ASM += $(foreach SYM, $(ERIKAOPT), --defsym $(SYM)=1)
OPTH8CC += $(addprefix -D, $(ERIKAOPT) )
OPTH8CPP += $(addprefix -D, $(ERIKAOPT) )


#
# Kernel source
#
H8_VPATH = $(ERIKABASE)/src/h8
H8_SRCS = hal.s start.s drivers.s func_d.c
ifeq ($(findstring TRIG_USED__, $(ERIKAOPT)), TRIG_USED__)
H8_SRCS += fmath.s
else
ifeq ($(findstring FIXED_USED__, $(ERIKAOPT)), FIXED_USED__)
H8_SRCS += fmath.s
endif
ifeq ($(findstring __IL_SUPPORT__, $(ERIKAOPT)), __IL_SUPPORT__)
H8_SRCS += IL_layer.c
H8_SRCS += IL_layer_conf.c
H8_SRCS += IL_layer_erika.c
H8_SRCS += serial.c
endif
ifeq ($(findstring __IR_RX_SUPPORT__, $(ERIKAOPT)), __IR_RX_SUPPORT__)
H8_SRCS += serial.c
endif
ifeq ($(findstring __IR_SUPPORT__, $(ERIKAOPT)), __IR_SUPPORT__)
H8_SRCS += serial.c
endif
ifeq ($(findstring __IR_ACK_REQ__, $(ERIKAOPT)), __IR_ACK_REQ__)
H8_SRCS += serial.c
endif
endif



OPTH8ASM += -I$(ERIKABASE)/src/h8 # for symbols.s
OPTH8ASM += -I$(ERIKABASE)/include/h8 # for handstub.s


# FP
ifeq ($(findstring __FP__,$(ERIKAOPT)) , __FP__)
KERN_VPATH += $(ERIKABASE)/src/fp
KERN_SRCS += gettime.c idle.c irq_act.c irq_sc.c panic.c reboot.c \
	rq_exchg.c rq_inser.c schedule.c thact.c thendin.c thmkrdy.c

  ifneq ($(findstring __FP_NO_MUTEX__,$(ERIKAOPT)) , __FP_NO_MUTEX__)
  KERN_SRCS += mutex.c
  endif

else
KERN_VPATH +=
KERN_SRCS +=
endif


#
# --------------------------------------------------------------------------
#

# Libraries

ifeq ($(findstring __CPP__,$(ERIKAOPT)) , __CPP__)
OPTH8LIBS += -L $(H8BASE)/lib/gcc-lib/h8300-hitachi-hms/2.95.2 -lgcc
endif

ifeq ($(findstring __LIBC__,$(ERIKAOPT)) , __LIBC__)
ALLINCPATH += -I$(ERIKABASE)/include/libc
#LIB_SRCS += abort.c memset.c strcmp.c
LIB_SRCS += abort.c free.c malloc.c memset.c strcmp.c
LIB_VPATH += $(ERIKABASE)/src/libc/string $(ERIKABASE)/src/libc/stdlib
endif

ifeq ($(findstring __CABS__,$(ERIKAOPT)) , __CABS__)
LIB_SRCS += cabs.c
LIB_VPATH += $(ERIKABASE)/src/cabs
endif

ifeq ($(findstring __SEM__,$(ERIKAOPT)) , __SEM__)

ifeq ($(findstring __MONO__,$(ERIKAOPT)) , __MONO__)
all::
	$(error Semaphores and Mono Stack HAL are not compatible!!!)
clean::
	$(error Semaphores and Mono Stack HAL are not compatible!!!)
endif

LIB_SRCS += swait.c spost.c sirqpost.c sgetvalue.c strywait.c
LIB_VPATH += $(ERIKABASE)/src/sem
endif

ifeq ($(findstring __ALARMS__,$(ERIKAOPT)) , __ALARMS__)
LIB_SRCS += alcancel.c altick.c alget.c alsetabs.c alsetrel.c
LIB_VPATH += $(ERIKABASE)/src/alarms
endif



#
# --------------------------------------------------------------------------
#

##
## Common Rules
##

SRCS = $(APP_SRCS)
OBJS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SRCS))))

LIBSRCS = $(H8_SRCS) $(KERN_SRCS) $(LIB_SRCS)
LIBOBJS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(LIBSRCS))))

vpath %.o out
vpath %.srec out
vpath %.c $(H8_VPATH) $(KERN_VPATH) $(LIB_VPATH) $(APPBASE)
vpath %.s $(H8_VPATH) $(KERN_VPATH) $(LIB_VPATH) $(APPBASE)


%.o: %.s
	@test -d out || mkdir out
	@echo
	$(H8ASM) $(OPTH8ASM) $< -o out/$(notdir $@) 

%.o: %.c 
	@test -d out || mkdir out
	@echo
	$(H8CC) $(OPTH8CC) $< -o out/$(notdir $@)
ifeq ($(findstring DEBUG,$(ERIKAOPT)) , DEBUG)
	$(H8CC) $(OPTH8CC) $< -S -o out/$(notdir $(patsubst %.o,%.S,$@))
endif

%.o: %.cpp 
	@test -d out || mkdir out
	@echo
	$(H8CPP) $(OPTH8CPP) $< -o out/$(notdir $@)
ifeq ($(findstring DEBUG,$(ERIKAOPT)) , DEBUG)
	$(H8CPP) $(OPTH8CPP) $< -S -o out/$(notdir $(patsubst %.o,%.S,$@))
endif


##
## Main rules: all clean
##

.PHONY: all clean chkerr

all:: chkerr $(PROJ).srec

$(PROJ).srec: $(OBJS) out/liberika.a
	$(H8LINK) $(OPTH8LINK) -o out/$@ -Map out/$(PROJ).MAP $(addprefix out/, $(OBJS)) --start-group $(OPTH8LIBS) --end-group

clean::
	-rm -rf out

##
## ERIKA Library
##

out/liberika.a: $(LIBOBJS)
	$(H8AR) rs out/liberika.a $(addprefix out/,$(LIBOBJS))



## Check default dependencies
include $(ERIKABASE)/config/check.mak



## Check H8 specific dependencies and errors

##ifeq ($(findstring __PLAY_BUFFER_SUPPORT__,$(ERIKAOPT)), __PLAY_BUFFER_SUPPORT__)
##ifeq ($(findstring __PLAY_NOTE_SUPPORT__,$(ERIKAOPT)), __PLAY_NOTE_SUPPORT__)
##    $(error don't use __PLAY_BUFFER_SUPPORT__ and __PLAY_NOTE_SUPPORT__ together)
##endif
##endif

ifeq ($(findstring __MOTOR_B_PWM_USED__,$(ERIKAOPT)), __MOTOR_B_PWM_USED__)
    $(error Solo i motori A e C possono essere controllati con tecnica PWM)
endif

ifeq ($(findstring __IR_SUPPORT__, $(ERIKAOPT)), __IR_SUPPORT__)
ifeq ($(findstring __ERIKA_PWM_USED__, $(ERIKAOPT)), __ERIKA_PWM_USED__)
ifeq ($(findstring __PLAY_NOTE_SUPPORT__, $(ERIKAOPT)), __PLAY_NOTE_SUPPORT__)
    $(error don't use __IR_SUPPORT__, __ERIKA_PWM_USED__ and __PLAY_NOTE_SUPPORT__ together)
endif
ifeq ($(findstring __PLAY_BUFFER_SUPPORT__, $(ERIKAOPT)), __PLAY_BUFFER_SUPPORT__)
    $(error don't use __IR_SUPPORT__, __ERIKA_PWM_USED__ and __PLAY_BUFFER_SUPPORT__ together)
endif
endif
endif	    




###########################################
##### AUTO GENERATED MACRO
###########################################

# the following lines automatically add a set of symbols to the main
# ERIKAOPT symbol

ifeq ($(findstring __ERIKA_PWM_USED__,$(ERIKAOPT)), __ERIKA_PWM_USED__)
ifeq ($(findstring __IR_SUPPORT__,$(ERIKAOPT)), __IR_SUPPORT__) 
    ERIKAOPT += __T80_SUPPORT__
endif
    ERIKAOPT += __T81_SUPPORT__
endif

ifeq ($(findstring VP_USED__,$(ERIKAOPT)), VP_USED__)
    ERIKAOPT += __MOTOR_VP_SUPPORT__
endif 

ifeq ($(findstring __PLAY_BUFFER_SUPPORT__,$(ERIKAOPT)), __PLAY_BUFFER_SUPPORT__)
    ERIKAOPT += __T80_SUPPORT__
else
ifeq ($(findstring __PLAY_NOTE_SUPPORT__,$(ERIKAOPT)), __PLAY_NOTE_SUPPORT__)
    ERIKAOPT += __T80_SUPPORT__
endif 
endif

ifeq ($(findstring __PLAY_BUFFER_SUPPORT__,$(ERIKAOPT)), __PLAY_BUFFER_SUPPORT__)
ERIKAOPT += __TIMER_B_SUPPORT__
else
ifeq ($(findstring __PLAY_NOTE_SUPPORT__,$(ERIKAOPT)), __PLAY_NOTE_SUPPORT__)
ERIKAOPT += __TIMER_B_SUPPORT__
else
ifeq ($(findstring __MOTOR_VP_SUPPORT__,$(ERIKAOPT)), __MOTOR_VP_SUPPORT__)
ERIKAOPT += __TIMER_B_SUPPORT__
else
ifeq ($(findstring __TIMER_B_USER_FUNCTION_USED__,$(ERIKAOPT)), __TIMER_B_USER_FUNCTION_USED__)
ERIKAOPT += __TIMER_B_SUPPORT__
endif
endif
endif
endif


ifeq ($(findstring __TIMER_A_USER_FUNCTION_USED__,$(ERIKAOPT)), __TIMER_A_USER_FUNCTION_USED__)
ERIKAOPT += __TIMER_A_SUPPORT__
endif


ifeq ($(findstring __SF_HIGH_SPEED_TRIG_USED__,$(ERIKAOPT)), __SF_HIGH_SPEED_TRIG_USED__)
ifneq ($(findstring __SF_TRIG_USED__,$(ERIKAOPT)), __SF_TRIG_USED__)
ERIKAOPT += __SF_TRIG_USED__ 
endif
endif

ifeq ($(findstring __DF_HIGH_SPEED_TRIG_USED__,$(ERIKAOPT)), __DF_HIGH_SPEED_TRIG_USED__)
ifneq ($(findstring __DF_TRIG_USED__,$(ERIKAOPT)), __DF_TRIG_USED__)
ERIKAOPT += __DF_TRIG_USED__
endif
endif

ifeq ($(findstring __SF_TRIG_USED__,$(ERIKAOPT)), __SF_TRIG_USED__)
ifneq ($(findstring __SIGNED_FIXED_USED__,$(ERIKAOPT)), __SIGNED_FIXED_USED__)
ERIKAOPT += __SIGNED_FIXED_USED__ 
endif
endif

ifeq ($(findstring __DF_TRIG_USED__,$(ERIKAOPT)), __DF_TRIG_USED__)
ifneq ($(findstring __DOUBLE_FIXED_USED__,$(ERIKAOPT)), __DOUBLE_FIXED_USED__)
ERIKAOPT += __DOUBLE_FIXED_USED__ 
endif
endif

ifeq ($(findstring _FIXED_USED__,$(ERIKAOPT)), _FIXED_USED__)
ERIKAOPT += __FMATH_SUPPORT__
endif


######  END OF AUTO GENERATED MACRO  ##############
