#!/usr/bin/make -f
## Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
##
## http://erika.sssup.it
##
## Copyright (C) 2004
##
## This program is distributed under the GPL license contained into
## the license.txt file on the E.R.I.K.A. root directory.
##

# this makefile is used to build librtw.a 
# set Matlab root dir
#MATLAB_ROOT=/usr/local/matlab
#MATLAB_ROOT=/opt/matlab
MATLAB_ROOT=/home/riccardo/local/opt/matlab




AR=h8300-hms-ar
CC=h8300-hms-gcc

CFLAGS=-O2 -DRT -I$(MATLAB_ROOT)/rtw/c/libsrc \
-I$(MATLAB_ROOT)/rtw/c/src -I$(MATLAB_ROOT)/extern/include \
-I$(MATLAB_ROOT)/simulink/include -I$(MATLAB_ROOT)/rtw/c/ert 

ALLRTWLIBSRCS = $(wildcard $(MATLAB_ROOT)/rtw/c/libsrc/*.c) 
#$(wildcard $(MATLAB_ROOT)/extern/src/*.c) \
#$(wildcard $(MATLAB_ROOT)/simulink/src/*.c)
RTWLIBSRCS = $(ALLRTWLIBSRCS:rt_logging.c=)
RTWLIBOBJS = $(notdir $(patsubst %.c,%.o,$(RTWLIBSRCS))) 

vpath %.c $(MATLAB_ROOT)/rtw/c/libsrc
vpath %.c $(MATLAB_ROOT)/extern/src
vpath %.c $(MATLAB_ROOT)/simulink/src

librtw.a: $(RTWLIBOBJS)
	$(AR) -rc librtw.a $(RTWLIBOBJS)

clean: 
	rm -f $(RTWLIBOBJS) *~

purge: clean
	rm -f librtw.a
