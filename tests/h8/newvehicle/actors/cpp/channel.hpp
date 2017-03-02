#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "namedobject.hpp"
#include "cfg.hpp"
#include "port.hpp"
#include "list.hpp"
#include "receiver.hpp"

template<typename Token>
class Channel : public NamedObj {

  List<Receiver<Token>*,maxPortsChannel> _sinks;

public:
  Channel(namedObjID id = nullObjID) : NamedObj(id) { };

  static TypeID getTypeID() {
    return CHANNEL_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==CHANNEL_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
    
#ifdef _EXC_
  void connectReceiver(Receiver<Token> * p) throw (Exc); 
#else
  void connectReceiver(Receiver<Token> * p);
#endif
  
#ifdef _EXC_
  namedObjID disconnectReceiver(Receiver<Token> * p) throw (Exc); 
#else
  namedObjID disconnectReceiver(Receiver<Token> * p) ;
#endif
  
#ifdef _EXC_
  void connectReceiver(namedObjID id) throw (Exc); 
#else
  void connectReceiver(namedObjID id) ;
#endif
  
#ifdef _EXC_
  void disconnectReceiver(namedObjID id) throw (Exc); 
#else
  void disconnectReceiver(namedObjID id); 
#endif
    
  List<Receiver<Token> *,maxPortsChannel> & getDestList()  {
    return _sinks;
  };
};

#include "channel.cpp"
#endif
