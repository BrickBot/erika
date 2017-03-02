#ifndef _ONETYPEACTOR_HPP_
#define _ONETYPEACTOR_HPP_

/* 
   There is an evident ownership problem here: ports should belong
   to the Actor, so we should use some kind of "factory" scheme. 
   However, we cannot rely on dynamic creation of objects.....
*/

template<typename Token, int inputs=1, int outputs=1>
class OneTypeActor : public NamedObj, public Actor {
protected:
  InputPort<Token> * _inputports[inputs];
  OutputPort<Token> * _ouputports[outputs];
  
public:
  OneTypeActor(namedObjID id = nullObjID);
  
  static TypeID getTypeID() {
    return ONETYPEACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ONETYPEACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
 
#ifdef _EXC_
  void OneTypeActor<Token,inputs,outputs>::addInputPort(const int slot, InputPort<Token> * p) throw (Exc); 
#else
  void void OneTypeActor<Token,inputs,outputs>::addInputPort(const int slot, InputPort<Token> * p); 
#endif

#ifdef _EXC_
  void addOutputPort(const int slot, OutputPort<Token> * p) throw (Exc); 
#else
  void addOutputPort(const int slot, OutputPort<Token> * p); 
#endif


#ifdef _EXC_
  void addPort(const int slot, namedObjID id) throw (Exc);
#else
  void addPort(const int slot, namedObjID id);
#endif
  
#ifdef _EXC_
  namedObjID removeInputPort(const int slot) throw (Exc); 
#else
  namedObjID removeInputPort(const int slot); 
#endif 

#ifdef _EXC_
  namedObjID removeOutputPort(const int slot) throw (Exc); 
#else
  namedObjID removeOutputPort(const int slot); 
#endif 

  virtual ~OneTypeActor() {};
}



#endif
