#include "include/namedobject.hpp"
#include "include/cfg.hpp"
#include "include/baseexc.hpp"
#include "include/check.hpp"

NamedObj * NamedObj::_index[namedMaxNum];

int NamedObj::_IDcount = 0;
#ifdef _EXC_
NamedObj::NamedObj(namedObjID t = nullObjID) throw(Exc) 
#else
NamedObj::NamedObj(namedObjID t = nullObjID)
#endif 
{
  _checkCond(_IDcount > namedMaxNum,NAMEDOBJ_TOO_MANY_ID_EXC);
  if (t == nullObjID) {
    bool found = false;
    int k;
    for (int j = 0; (j < namedMaxNum) && !(found); j++) {
      k = (_IDcount+j) % namedMaxNum;
      if (_index[k] == 0) {
	_ID = k; 
	found = true;
      };
    };
  } else { 
    _checkCond(t >= namedMaxNum,NAMEDOBJ_WRONG_ID_EXC);
    _checkCond(_index[t] != 0,NAMEDOBJ_ID_ALREADY_PRESENT_EXC);
    _ID = t;
  };
  _IDcount++;
  _index[_ID] = this;   
};

#ifdef NAMEDOBJ_HAS_DESTRUCTOR
NamedObj::~NamedObj()
{
   _index[_ID]=0;
}
#endif

void NamedObj::initialiseID() {
  for (int i = 0; i < namedMaxNum; i++) 
    _index[i] = 0;
};
