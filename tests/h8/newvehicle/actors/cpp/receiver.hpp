#ifndef _RECEIVER_HPP_
#define _RECEIVER_HPP_

template<typename Token>
class Receiver {
public:
  virtual Token  get(int & sz) = 0;
  virtual void put(Token, int &sz) = 0;
  virtual bool hasToken() = 0;
  virtual bool hasRoom() = 0;
};

#endif

