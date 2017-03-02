#ifndef _FIFORECEIVER_HPP_
#define _FIFORECEIVER_HPP_
#include "include/list.hpp"
#include "include/namedobject.hpp"
#include "include/receiver.hpp"

template<typename Token, int width = 10>
class FifoReceiver : public NamedObj, public Receiver<Token>
{
protected:
  List<Token, width> _queue;
public:
  FifoReceiver(namedObjID id = nullObjID) : NamedObj(id) {};
  
  static TypeID getTypeID() {
    return FIFORECEIVER_TYPE_ID;
  };

  bool checkType(TypeID ty) {
    return ((ty==FIFORECEIVER_TYPE_ID)|
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
    return (! _queue.empty());
  };
  bool hasRoom() 
  {
    return (!_queue.full());
  };
};

#include "fiforeceiver.cpp"

#endif
