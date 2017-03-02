#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "baseexc.hpp"

class Actor {
public:
  //Initialises actor on application startup
#ifdef _EXC_
  virtual void init() throw (Exc)= 0;
#else
  virtual void init() = 0;
#endif
 #ifdef _EXC_
  virtual void fire() throw (Exc)= 0;
#else
  virtual void fire() = 0;
#endif
};

#endif
