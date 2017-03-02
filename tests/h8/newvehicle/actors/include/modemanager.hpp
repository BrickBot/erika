#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include "include/namedobject.hpp"
#include "include/actor.hpp"
#include "include/port.hpp"
#include "include/mode.hpp"



typedef unsigned short EvtID;

const ModeID UNEXPECTED_TRANSITION = 999;
const ModeID IGNORE_TRANSITION = 998;

template<ModeID modes = 10, EvtID events=4>
class ModeManager : public NamedObj, public Actor {
protected:
  
  ModeID _transitions[modes][events];
  /* 
     TBD: modes should be created and live within their manager
     pretty much as ports should within their actor
  */
  Mode * _modes[modes];

  ModeID _currMode;
  
  InputPort<EvtID,1> * _currEvt;
  
public:
  

  ModeManager(namedObjID id = nullObjID) {
    for(ModeID i = 0; i < modes; i++)
      for (ModeID j = 0; j < events; j++)
	_transitions[i][j] = UNEXPECTED_TRANSITION;
    for (ModeID i = 0; i < modes; i++)
      _modes[i] = 0;
  };
  
  void setPort(InputPort<EvtID,1> *p)
  {
    _currEvt = p;
  };


#ifdef _EXC_
  void setInitialMode(const ModeID id) throw (Exc)
#else
  void setInitialMode(const ModeID id)
#endif
  {
    _currMode = id;
  };

  /* returns the current mode */
  ModeID getMode(void)
  {
    return _currMode;
  };

  /* assigns a mode object to a mode */
#ifdef _EXC_
  void setMode(const ModeID id, Mode * m) throw (Exc)
#else
  void setMode(const ModeID id, Mode * m)
#endif
  {
    _modes[id] = m;
  };

  /* says that if the event ev arrives the current mode passes from id to new_id */
#ifdef _EXC_
  void setTransition(const ModeID id, const EvtID ev, const ModeID new_id) throw (Exc)
#else
  void setTransition(const ModeID id, const EvtID ev, const ModeID new_id)
#endif
  {
    _transitions[id][ev] = new_id;
  };

  /* says that an event is ignored by default */
#ifdef _EXC_
  void ignoreEvent(const EvtID ev) throw (Exc)
#else
  void ignoreEvent(const EvtID ev)
#endif
  {
    ModeID i;
    for(i=0; i<modes; i++)
      _transitions[i][ev] = IGNORE_TRANSITION;
  };



#ifdef _EXC_
  virtual void init() throw (Exc)
#else
  virtual void init()
#endif
  {};
  
#ifdef _EXC_
  virtual void fire() throw (Exc)
#else
  virtual void fire()
#endif
  {
    ModeID newmode, oldmode;
    int i;
    
    EvtID e = _currEvt->receive(0,i);

    _checkCond(e>=events, ASSERTION_FAILED_EXC);
    _checkCond(_transitions[_currMode][e]==UNEXPECTED_TRANSITION, MODEMANAGER_UNEXPECTED_TRANSITION_EXC);

    if (_transitions[_currMode][e] == IGNORE_TRANSITION) {
#ifdef __LCD_USED__
      ERIKA_lcd_write_num(9000+_currMode*10+e);
#endif
      return;
    }

    oldmode = _currMode;
    newmode = _transitions[_currMode][e];
    _modes[oldmode]->exitMode(newmode);
    _currMode = newmode;
    _modes[newmode]->enterMode(oldmode); 
  };


  static TypeID getTypeID() {
    return MODEMANAGER_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==MODEMANAGER_TYPE_ID)|
	    NamedObj::checkType(ty));
  };


};


#endif
