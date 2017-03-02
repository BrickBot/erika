#ifndef _PORT_H_
#define _PORT_H_
#include "cfg.hpp"
#include "namedobject.hpp"
#include "check.hpp"
#include "channel.hpp"
#include "receiver.hpp"

template<typename Token, int maxChannelsPort=maxCh>
class InputPort : public NamedObj {
protected:
  Receiver<Token> * _receivers[maxChannelsPort];
public:
  InputPort(namedObjID id = nullObjID);

  static TypeID getTypeID() {
    return INPUTPORT_TYPE_ID;
  };

  bool checkType(TypeID ty) {
    return ((ty==INPUT_PORT_TYPE_ID)|
	    NamedObj::checkType(ty));
  };

#ifdef _EXC_
  void addReceiver(const int slot, Receiver<Token> * ch) throw (Exc);
#else
  void addReceiver(const int slot,  Receiver<Token> * ch); 
#endif


#ifdef _EXC_
  void addReceiver(const int slot, const namedObjID &ch) throw (Exc);
#else
  void addReceiver(const int slot, const namedObjID &ch); 
#endif
 
#ifdef _EXC_
  namedObjID removeReceiver(const int slot) throw (Exc);
#else 
  namedObjID removeReceiver(const int slot);
#endif
  
#ifdef _EXC_
  virtual Token  receive(const int slot, int & receivedTokens) throw(Exc);
#else
  virtual Token  receive(const int slot, int & receivedTokens);
#endif
  bool hasToken(const int slot) {
    _checkCondObj(_receivers[slot]==0, 
		  PORT_ABSENT_RECEIVER_EXC);
    return _receivers[slot]->hasToken();
  };
  virtual ~InputPort() {};
};


template<typename Token, int maxChannelsPort=maxCh>
class OutputPort : NamedObj {
  Channel<Token> * _channels[maxChannelsPort];
  int _linkedChannels;
public:
  OutputPort(namedObjID id = nullObjID);
  
  static TypeID getTypeID() {
    return OUTPUT_PORT_TYPE_ID;
  };

  bool checkType(TypeID ty) {
    return ((ty==OUTPUT_PORT_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
    
#ifdef _EXC_
  void addChannel(const int slot,  Channel<Token> * ch) throw (Exc);
#else
  void addChannel(const int slot,  Channel<Token> * ch); 
#endif
  
#ifdef _EXC_
  void addChannel(const int slot,  const namedObjID &ch) throw (Exc);
#else
  void addChannel(const namedObjID &ch); 
#endif
  
#ifdef _EXC_
  namedObjID removeChannel(const int slot) throw (Exc);
#else
  namedObjID removeChannel(const int slot);
#endif

#ifdef _EXC_
  virtual  void send(const int slot, Token  m, int & sentTokens) throw (Exc); 
#else
  virtual void send(const int slot, Token  m, int & sentTokens) ;
#endif

#ifdef _EXC_
  void broadcast(Token  m, int & sentTokens);
#else
  void broadcast(Token  m, int & sentTokens) throw(Exc);
#endif

  virtual ~OutputPort() {};
};


//FUNCTIONS
#include "port.cpp"


#endif
