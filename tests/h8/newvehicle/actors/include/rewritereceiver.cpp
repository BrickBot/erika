
template<typename Token>
#ifdef _EXC_
Token  RewriteReceiver<Token>::get(int & sz) throw(Exc)
#else
Token  RewriteReceiver<Token>::get(int & sz) 
#endif
{
  valid = false;
  sz = 1;
  return _t;
};

template<typename Token>
#ifdef _EXC_
void RewriteReceiver<Token>::put(Token t, int &sz) throw(Exc)
#else
void RewriteReceiver<Token>::put(Token t, int &sz)
#endif
{
  _t = t;
  sz = 1;
  valid = true;
};


