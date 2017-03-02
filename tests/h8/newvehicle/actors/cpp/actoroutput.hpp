#ifndef __ACTOROUTPUT_HPP_
#define __ACTOROUTPUT_HPP_
#include "baseexc.hpp"
#include "check.hpp"
#include "actor.hpp"
#include "port.hpp"

#ifdef _DEBUG_ 
#include <fstream>
#endif

#include <ostream.h>

template<typename T, int input>
class ActorOutput: public Actor, public NamedObj {
  #ifdef _DEBUG_
  fstream & _f;
  #endif

  InputPort<T,1> * _in[input];

  ostream &_target;
 public:
#ifdef _DEBUG_
  ActorOutput (fstream & ifs, ostream & t, namedObjID n = nullObjID): 
    NamedObj(n), _f(ifs), _target(t)   {
    for(int i = 0; i<input; i++)
      _in[i] = 0;
  };
#else
  ActorOutput (ostream & t, namedObjID n = nullObjID): 
    NamedObj(n), _f(ifs), _target(t)   {
    for(int i = 0; i<input; i++)
      _in[i] = 0;
  };
#endif
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  
  void setInputPort(int slot, InputPort<T,1> * p) {
    _checkCondObj(_in[slot]!=0, ACTOR_SLOT_BUSY_EXC);
    _in[slot] = p;
  };

#ifdef _EXC_
  virtual void init() throw (Exc)
#else
  virtual void init()
#endif
  {
    for (int i = 0; i < input; i++)
      _checkCondObj(_in[i]==0, ACTOR_SLOT_EMPTY_EXC);
  };
 #ifdef _EXC_
  virtual void fire() throw (Exc)
#else
  virtual void fire()
#endif
  {
    int t;
#ifdef _DEBUG_
    _f<<"Fire called by actor "<<getID()<<endl;
#endif
    for (int i = 0; i < input; i++) {
      //Init should have been called first!!!
      _checkCondObj(_in[i]==0,ASSERTION_FAILED_EXC);
      T value = _in[i]->receive(0,t);
#ifdef _DEBUG_
      _f<<t<<" tokens received"<<endl;
      _f<<"Value: "<<value<<endl;
#endif
      _target<<" "<<value;
    };
    _target<<endl;
  };
};







#endif
