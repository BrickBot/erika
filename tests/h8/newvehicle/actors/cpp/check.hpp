#ifndef  _CHECKS_HPP_
#define _CHECKS_HPP_
#ifdef _CHECKS_
#ifdef _EXC_
#define _checkCond(expr,eid) \
{\
  if(expr)\
    throw Exc(eid,__FILE__,__LINE__);\
}; 

#define _checkCondObj(expr,eid) \
{\
  if(expr)\
    throw Exc(eid,getID(),__FILE__,__LINE__);\
}; 

#else
inline void _checkCond(bool expr, excID eid) {
  if((expr))
    sysAbort(eid);
  return;
}; 

#define _checkCondObj(expr,eid) _checkCond(expr,eid)

#endif
#else
#define _checkCond(expr,eid) 
#define _checkCondObj(expr,eid)
#endif
#endif 
