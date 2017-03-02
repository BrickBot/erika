
template<typename Token>
#ifdef _EXC_
void Channel<Token>::connectReceiver(Receiver<Token> * p) throw (Exc) 
#else
  void Channel<Token>::connectReceiver(Receiver<Token> * p)
#endif
{
  _sinks.push_back(p);
};

template<typename Token>
#ifdef _EXC_
namedObjID Channel<Token>::disconnectReceiver(Receiver<Token> * p) throw (Exc) 
#else
  namedObjID Channel<Token>::disconnectReceiver(Receiver<Token> * p) 
#endif
{
  int pos = _sinks.find(p);
  _checkCondObj(pos==NOT_FOUND,
	     CHANNEL_ABSENT_RECEIVER_EXC);
  namedObjID id = _sinks[pos]->getID();
  _sinks.remove(pos);
  return id;
};


template<typename Token>
#ifdef _EXC_
void Channel<Token>::connectReceiver(namedObjID id) throw (Exc) 
#else
  void Channel<Token>::connectReceiver(namedObjID id) 
#endif
{
  Receiver<Token> * p = my_dynamic_cast< Receiver<Token> >(NamedObj::_find(id));
  _checkCondObj(p==0,CHANNEL_WRONG_OBJ_FOR_CHANNEL_EXC);
  connectReceiver(p);
};

template<typename Token>
#ifdef _EXC_
void Channel<Token>::disconnectReceiver(namedObjID id) throw (Exc) 
#else
  void Channel<Token>::disconnectReceiver(namedObjID id) 
#endif
{
  Receiver<Token> * p = my_dynamic_cast< Receiver<Token> >(NamedObj::_find(id));
  _checkCondObj(p==0,ASSERTION_FAILED_EXC);
  disconnectReceiver(p);
};
