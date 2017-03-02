#ifndef _ACTOR_ACCUMULATOR_HPP_
#define _ACTOR_ACCUMULATOR_HPP_
#include "baseexc.hpp"
#include "check.hpp"
#include "actor.hpp"
#include "port.hpp"
#include "namedobject.hpp"

template<typename T, int outNum>
class ActorAccumulator: public Actor, public NamedObj {
  fstream & _f;
  T _value;
  OutputPort<T,outNum> * _out;
  InputPort<T,1> * _in;
  ActorAccumulator();
public:
  ActorAccumulator (fstream & ifs, namedObjID n = nullObjID): 
    NamedObj(n), _f(ifs), _value(0), _out(0), _in(0) {};
  
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  
  void setOutputPort(OutputPort<T,outNum> * p) {
    _out = p;
  };

  void setInputPort(InputPort<T,1> * p) {
    _in = p;
  };


#ifdef _EXC_
  virtual void init() throw (Exc)
#else
    virtual void init()
#endif
  {
    _checkCondObj(_out==0, ACTOR_SLOT_EMPTY_EXC);
    _checkCondObj(_in==0, ACTOR_SLOT_EMPTY_EXC);
  };
#ifdef _EXC_
  virtual void fire() throw (Exc)
#else
    virtual void fire()
#endif
  {
    int t;
    T nv;
    _f<<"Fire called by actor "<<getID()<<endl;
    //Init should have been called first!!!
    _checkCondObj(_out==0,ASSERTION_FAILED_EXC);
    _checkCondObj(_in==0,ASSERTION_FAILED_EXC);
    nv= (_in->receive(0,t));
    _f<<t<<" tokens received"<<endl;
    _f<<"Value: "<<nv<<endl;
    _value += nv;
    _f<<"New accumulate value: "<<_value<<endl;
    _out->broadcast(_value,t);
    _f<<t<<" tokens sent"<<endl;
  };
};




#endif
