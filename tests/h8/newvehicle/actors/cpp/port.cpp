////////////////////////////////////////////////////////////////////////
//                         InputPort METHODS                         //
//////////////////////////////////////////////////////////////////////

template<typename Token , int maxChannelsPort>
InputPort<Token,maxChannelsPort>::InputPort(namedObjID id = nullObjID): NamedObj(id) 
{
    for (int i = 0; i < maxChannelsPort; i++)
	_receivers[i] = 0;
};

template<typename Token, int maxChannelsPort>
#ifdef _EXC_
void InputPort<Token, maxChannelsPort>::addReceiver(const int slot,  Receiver<Token> * ch) throw (Exc)
#else
  void InputPort<Token, maxChannelsPort>::addReceiver(const int slot,  Receiver<Token> * ch) 
#endif
{
    _checkCondObj(slot >= maxChannelsPort, PORT_SLOT_OUT_OF_RANGE_EXC);
    _checkCondObj(_receivers[slot]!=0, PORT_SLOT_BUSY_EXC);
    _receivers[slot]=ch;    
};


template<typename Token, int maxChannelsPort>
#ifdef _EXC_
inline  void InputPort<Token, maxChannelsPort>::addReceiver(const int slot, const namedObjID &ch) throw (Exc)
#else
    inline void InputPort<Token, maxChannelsPort>::addReceiver(const int slot, const namedObjID &ch) 
#endif
{
    Receiver<Token> * c = my_dynamic_cast< Receiver<Token> >(NamedObj::_find(ch));
    _checkCondObj(c==0,PORT_WRONG_OBJ_FOR_RECEIVER_EXC);
    attachReceiver(slot, c);
};

template<typename Token, int maxChannelsPort>
#ifdef _EXC_
inline namedObjID InputPort<Token, maxChannelsPort>::removeReceiver(const int slot) throw (Exc)
#else 
    inline namedObjID InputPort<Token, maxChannelsPort>::removeReceiver(const int slot)
#endif
{
    _checkCondObj(slot >= maxChannelsPort, PORT_SLOT_OUT_OF_RANGE_EXC);
    namedObjID id = _receivers[slot];
    _checkCondObj(_receivers[slot]==0, PORT_SLOT_EMPTY_EXC);
    _receivers[slot]=0;
    return id;
};

template<typename Token, int maxChannelsPort>
#ifdef _EXC_
Token  InputPort<Token, maxChannelsPort>::receive(const int slot, int & receivedTokens) throw(Exc)
#else
    Token  InputPort<Token, maxChannelsPort>::receive(const int slot, int & receivedTokens)
#endif
{
    _checkCondObj(_receivers[slot]==0, 
	       PORT_ABSENT_RECEIVER_EXC); 
    return _receivers[slot]->get(receivedTokens);
};


////////////////////////////////////////////////////////////////////////
//                         OuputPort METHODS                         //
//////////////////////////////////////////////////////////////////////
template<typename Token, int maxChannelsPort>
OutputPort<Token,maxChannelsPort>::OutputPort(namedObjID id = nullObjID) : NamedObj(id),_linkedChannels(0) {
  for (int i = 0; i < maxChannelsPort; i++)
    _channels[i] = 0;
};



template<typename Token, int maxChannelsPort>   
#ifdef _EXC_
 void OutputPort<Token,maxChannelsPort>::addChannel(const int slot,  Channel<Token> * ch) throw (Exc)
#else
  void OutputPort<Token,maxChannelsPort>::addChannel(const int slot,  Channel<Token> * ch) 
#endif
{
  _checkCondObj(slot >= maxChannelsPort, PORT_SLOT_OUT_OF_RANGE_EXC);
  _checkCondObj(_channels[slot]!=0, PORT_SLOT_BUSY_EXC);
  //The following should be implied by the test immediately before
  _checkCondObj(_linkedChannels==maxChannelsPort, ASSERTION_FAILED_EXC);
  _channels[slot] = ch;
  _linkedChannels++;
};


template<typename Token, int maxChannelsPort>   
#ifdef _EXC_
void OutputPort<Token,maxChannelsPort>::addChannel(const int slot, const namedObjID &ch) throw (Exc)
#else
    void OutputPort<Token,maxChannelsPort>::addChannel(const int slot, const namedObjID &ch) 
#endif
{
    Channel<Token> * c = my_dynamic_cast< Channel<Token> >(NamedObj::_find(ch));
    _checkCondObj(c==0,PORT_WRONG_OBJ_FOR_CHANNEL_EXC);
    addChannel(slot, c);
};

  
template<typename Token, int maxChannelsPort>   
#ifdef _EXC_
namedObjID OutputPort<Token,maxChannelsPort>::removeChannel(const int slot) throw (Exc)
#else
    namedObjID OutputPort<Token,maxChannelsPort>::removeChannel(const int slot)
#endif
{
    _checkCondObj(slot >= maxChannelsPort, PORT_SLOT_OUT_OF_RANGE_EXC);
    namedObjID id = _channels[slot];
    _checkCondObj(_channels[slot]==0, PORT_SLOT_EMPTY_EXC);
    _channels[slot]=0;
    _linkedChannels--;
    return id;   
};

template<typename Token, int maxChannelsPort>   
#ifdef _EXC_
void OutputPort<Token,maxChannelsPort>::send(const int slot, Token  m, int &sentTokens) throw (Exc) 
#else
    void OutputPort<Token,maxChannelsPort>::send(const int slot, Token  m, int &sentTokens) 
#endif
{
  if(_channels[slot]!=0) {
    List<Receiver<Token> *,maxPortsChannel> &l = _channels[slot]->getDestList();
    sentTokens = 0;
    for (int i = 0; i<l.length(); i++) {
	int sz=0;
	l[i]->put(m, sz);
	sentTokens += sz;
    };
  };
};

template<typename Token, int maxChannelsPort>
void  OutputPort<Token,maxChannelsPort>::broadcast(Token  m, int & sentTokens)
{
  sentTokens = 0;
  int channels=0;
  for (int i = 0; (channels <= _linkedChannels) &&(i < maxChannelsPort);i++) {
    int sz = 0;
    if(_channels[i]!=0) {
      send(i,m, sz);
      sentTokens += sz;
      channels++;
    };
  };
}
