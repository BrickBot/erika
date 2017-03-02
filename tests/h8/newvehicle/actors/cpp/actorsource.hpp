#ifndef _ACTOR_SOURCE_HPP_
#define _ACTOR_SOURCE_HPP_
#include "namedobject.hpp"

#include "baseexc.hpp"
#include "check.hpp"
#include "actor.hpp"
#include "port.hpp"

#ifdef _DEBUG_
#include <fstream>
#endif

template<typename T, int outNum>
class ActorConstSource: public Actor, public NamedObj {
#ifdef _DEBUG_
  fstream & _f;
#endif
  T _value;
  OutputPort<T,outNum> * _out;
  
public:
#ifdef _DEBUG_
  ActorConstSource (fstream & ifs, const T v, namedObjID n = nullObjID): 
     NamedObj(n),_f(ifs), _value(v),  _out(0) {};
#else
  ActorConstSource (const T v, namedObjID n = nullObjID): 
    NamedObj(n), _value(v),  _out(0) {};
#endif

  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  
  void setPort(OutputPort<T,outNum> * p) {
    _out = p;
  };

#ifdef _EXC_
  virtual void init() throw (Exc)
#else
  virtual void init()
#endif
  {
    _checkCondObj(_out==0, ACTOR_SLOT_EMPTY_EXC);
  };
#ifdef _EXC_
  virtual void fire() throw (Exc)
#else
  virtual void fire()
#endif
  {
    //Init should be called first!!!
    int t;
    _checkCondObj(_out==0,ASSERTION_FAILED_EXC);
    _out->broadcast(_value,t);
#ifdef _DEBUG_
    _f<<"Fire called by actor "<<getID()<<endl;
    _f<<t<<" tokens sent"<<endl;
#endif
  };
};





#endif
