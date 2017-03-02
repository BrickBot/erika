#ifndef _LIST_H_
#define _LIST_H_
//#include <iostream>
#include "baseexc.hpp"
#include "cfg.hpp"
#include "check.hpp"

template<class T>
void swap(T &o1, T &o2) {
  T tmp = o1;
  o1 = o2;
  o2 = tmp;
};

template<typename T, int size = 10> 
class List {
  T _list[size];
  int bottom, elems;
public:
  List(): bottom(0), elems(0) {};

  int length() const {return elems;};
  
  bool empty() const {return (elems==0);};

  bool full() const {return (elems==size);};
#ifdef _EXC_
  void push_back(T  elem) throw (Exc) 
#else
    void push_back(T  elem)  
#endif
  {
    _checkCond(elems==size, LIST_FULL_EXC);
    _list[(bottom+elems)%size] = elem;
    elems ++;
  };
#ifdef _EXC_
  T back() throw (Exc) 
#else
    T back() 
#endif
  {
    _checkCond(elems==0, LIST_EMPTY_EXC);
    return _list[(bottom+elems-1)%size];
  };

#ifdef _EXC_
  T front() throw (Exc) 
#else
    T front() 
#endif
  {
    _checkCond(elems==0, LIST_EMPTY_EXC);
    return _list[bottom];
  }
#ifdef _EXC_
  void pop_back() throw (Exc) 
#else
    void pop_back() 
#endif
  {
    _checkCond(elems==0, LIST_EMPTY_EXC);
    elems --;
  };
#ifdef _EXC_
  void push_front(T  elem) throw (Exc)
#else
    void push_front(T  elem) 
#endif
  {
    _checkCond(elems==size, LIST_FULL_EXC);
    if (bottom == 0)
      bottom = size-1;
    else
      bottom -- ;    _list[bottom] = elem;
    elems++;
  };
#ifdef _EXC_
  void pop_front() throw (Exc) 
#else
    void pop_front() 
#endif
  {
    _checkCond(elems==0, LIST_EMPTY_EXC);
    bottom = (bottom+1) % size;
    elems --;
  };

#ifdef _EXC_
  T & operator [](int i) throw (Exc) 
#else
    T & operator [](int i) 
#endif
  {
    _checkCond(i >= elems, LIST_INDEX_OUT_OF_RANGE_EXC);
    return _list[(bottom+i)%size];
  };
  
  int find(const T & el) {
    bool found = false;
    int pos = NOT_FOUND;
    for (int i = 0; (i < elems) & ! (found); i++) {
      if (_list[(bottom+i)%size]==el) {
	pos = i; 
	found = true;
      };
    };
    return pos;
  };
#ifdef _EXC_
  void remove(int i) throw (Exc) 
#else
    void remove(int i) 
#endif
  {
    _checkCond(i > elems, LIST_NON_EXISTING_ELEM_EXC);
    for (int j = bottom+i; j < bottom+elems-1; j ++) 
      swap(_list[j],_list[j+1]);
    elems --;
  };
};
#endif
