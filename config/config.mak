## Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
##
## http://erika.sssup.it
##
## Copyright (C) 2001-2002 Paolo Gai, Enrico Bini, Alessandro Colantonio
##
## This program is distributed under the GPL license contained into the
## license.txt file on the E.R.I.K.A. root directory.
##
## CVS: $Id: config.mak,v 1.2 2004/06/01 16:46:27 vestrini Exp $


# H8/300 - GNU/Linux version 
ifeq ($(findstring __H8__,$(ERIKAOPT)), __H8__) 
  ifeq ($(findstring __GCC3__,$(ERIKAOPT)), __GCC3__) 
    include $(ERIKABASE)/config/h8cfg3.mak
  else
    include $(ERIKABASE)/config/h8cfg.mak
  endif
endif 
