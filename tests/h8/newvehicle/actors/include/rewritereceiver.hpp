#ifndef _FIFORECEIVER_HPP_
#define _FIFORECEIVER_HPP_
#include "include/list.hpp"
#include "include/namedobject.hpp"
#include "include/receiver.hpp"

template<typename Token>
class RewriteReceiver : public NamedObj, public Receiver<Token>
{
protected:
  Token _t;
  bool valid;
public:
  RewriteReceiver(namedObjID id = nullObjID) : NamedObj(id), valid(false) {};
  
  static TypeID getTypeID() {
    return REWRITERECEIVER_TYPE_ID;
  };

  bool checkType(TypeID ty) {
    return ((ty==REWRITERECEIVER_TYPE_ID)|
	    NamedObj::checkType(ty));
  };

#ifdef _EXC_
  Token get(int & sz) throw (Exc);
#else
  Token get(int & sz);
#endif

#ifdef _EXC_
  void put(Token, int &sz) throw (Exc);
#else
  void put(Token, int &sz);
#endif
  bool hasToken() 
  {
    return valid;
  };
  bool hasRoom() 
  {
    return true;
  };
};

#include "rewritereceiver.cpp"

#endif
