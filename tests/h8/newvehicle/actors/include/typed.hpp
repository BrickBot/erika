#ifndef _TYPED_H_
#define _TYPED_H_
#include "baseexc.hpp"


class TypedObj {
protected:
  typedef enum {
    NAMEDOBJ_TYPE_ID,
    CHANNEL_TYPE_ID,
    INPUT_PORT_TYPE_ID,
    OUTPUT_PORT_TYPE_ID,
    FIFORECEIVER_TYPE_ID,
    REWRITERECEIVER_TYPE_ID,
    MODEMANAGER_TYPE_ID,
    ACTOR_TYPE_ID
  } TypeID;
public:
  //overridden versions of this function do have
  //to call the overridden function of all
  //derived classes implementing the typed interface
  virtual bool checkType(TypeID ty) = 0;
};

//This template function assumes that
//class B has a static function called
//getTypeID()

template <class B, class A>
B * my_dynamic_cast(A * a) {
  TypedObj * p = a;
  if ((p!=0))
    if(p->checkType(B::getTypeID())){
      void * p = (void *) a;
      return (B*) p;
    };
  return (B*) 0;
};

#endif
