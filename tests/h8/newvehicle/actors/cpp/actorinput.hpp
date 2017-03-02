#ifndef _ACTORINPUT_HPP_
#define _ACTORINPUT_HPP_
#include "baseexc.hpp"
#include "check.hpp"
#include "actor.hpp"
#include "port.hpp"
#ifdef _DEBUG_
#include <fstream>
#endif
#include <istream.h>

template<typename T, int output>
class ActorInput: public Actor, public NamedObj {
#ifdef _DEBUG_ 
  fstream & _f;
#endif
  OutputPort<T> * _out[output];
  istream &_source;
 public:
#ifdef _DEBUG_
  ActorInput (fstream & ifs, istream & s, namedObjID n = nullObjID): 
    NamedObj(n), _f(ifs), _source(s)   
  {
    for(int i = 0; i<output; i++)
      _out[i] = 0;
  };
#else
  ActorInput (istream & s, namedObjID n = nullObjID): 
    NamedObj(n), _source(s)   
  {
    for(int i = 0; i<output; i++)
      _out[i] = 0;
  };
#endif
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  
  void setOutputPort(int slot, OutputPort<T> * p) {
    _checkCondObj(_out[slot]!=0, ACTOR_SLOT_BUSY_EXC);
    _out[slot] = p;
  };

#ifdef _EXC_
  virtual void init() throw (Exc)
#else
  virtual void init()
#endif
  {
    for (int i = 0; i < output; i++)
      _checkCondObj(_out[i]==0, ACTOR_SLOT_EMPTY_EXC);
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
    for (int i = 0; i < output; i++) {
      //Init should have been called first!!!
      _checkCondObj(_out[i]==0,ASSERTION_FAILED_EXC);
      T value;
      _source>>value;
      _out[i]->broadcast(value,t);
#ifdef _DEBUG_
      _f<<t<<" tokens sent"<<endl;
      _f<<"Value: "<<value<<endl;
#endif
    };
  };
};





#endif
