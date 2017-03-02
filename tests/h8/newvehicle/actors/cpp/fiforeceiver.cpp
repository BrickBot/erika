
template<typename Token, int width>
#ifdef _EXC_
Token  FifoReceiver<Token,width>::get(int & sz) throw(Exc)
#else
Token  FifoReceiver<Token,width>::get(int & sz) 
#endif
{
  _checkCondObj(_queue.empty(), RECEIVER_EMPTY_BUFFER_EXC);
  Token r = _queue.front();
  _queue.pop_front();
  sz = 1;
  return r;
};

template<typename Token, int width>
#ifdef _EXC_
void FifoReceiver<Token,width>::put(Token t, int &sz) throw(Exc)
#else
void FifoReceiver<Token,width>::put(Token t, int &sz)
#endif
{
  _checkCondObj(_queue.full(), RECEIVER_FULL_BUFFER_EXC);
  _queue.push_back(t);
  sz = 1;
};


