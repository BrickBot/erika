#ifndef __BASEEXC_HPP__
#define __BASEEXC_HPP__

const int maxFileName=100;
typedef enum {
  LIST_FULL_EXC,
  LIST_EMPTY_EXC,
  LIST_INDEX_OUT_OF_RANGE_EXC,
  LIST_NON_EXISTING_ELEM_EXC,
  NAMEDOBJ_TOO_MANY_ID_EXC,
  NAMEDOBJ_WRONG_ID_EXC,
  NAMEDOBJ_ID_ALREADY_PRESENT_EXC,
  ACTOR_SLOT_BUSY_EXC,
  ACTOR_SLOT_EMPTY_EXC,
  ACTOR_SLOT_OUT_OF_RANGE_EXC,
  ACTOR_WRONG_OBJ_FOR_PORT_EXC,
  PORT_SLOT_BUSY_EXC,
  PORT_SLOT_EMPTY_EXC,
  PORT_SLOT_OUT_OF_RANGE_EXC,
  PORT_ABSENT_RECEIVER_EXC,
  PORT_WRONG_OBJ_FOR_RECEIVER_EXC,
  PORT_WRONG_OBJ_FOR_CHANNEL_EXC,
  PORT_CHANNEL_NOT_FOUND_EXC,
  CHANNEL_ABSENT_RECEIVER_EXC,
  CHANNEL_WRONG_OBJ_FOR_CHANNEL_EXC,
  RECEIVER_FULL_BUFFER_EXC,
  RECEIVER_EMPTY_BUFFER_EXC,
  ASSERTION_FAILED_EXC
} excID;

inline void my_stringcopy(const char * from, char * to, int max) {
    int i;
    for (i = 0; 
	 (i < max-1) && (from[i]!='\0');
	 i++)
      to[i]=from[i];
    to[i]='\0'; 
};


#ifdef _EXC_
#include "cfg.hpp" 
class Exc {
protected:
  /// Contains a brief description of the exception.
  excID _what;
  char _fileName[maxFileName];
  int _fileLine;
  namedObjID _objName;

public:
  Exc (excID what,
       namedObjID oname = nullObjID,
       const char * fname = "",
       int fline=0) :_what(what), _fileLine(fline), _objName(oname) 
  {
    my_stringcopy(fname, _fileName, maxFileName);
  };
  Exc (excID what,
       const char * fname = "",
       int fline=0) : _what(what), _fileLine(fline), _objName(nullObjID)
  {
    my_stringcopy(fname, _fileName, maxFileName);
  };
  
  /** Returns the error string. 
   *  This is the standard diagnostic behavior, since it is 
   *  virtual it can be overidden 
   */
  virtual excID what(void) {
    return _what; 
  };
  char * file() {
    return _fileName;
  };
  int line() const {
    return _fileLine;
  };
  
  namedObjID object() const {
    return _objName;
  };
  
  virtual ~Exc() throw() {};
};
#else

#include <stdlib.h>
#include <iostream>
inline void sysAbort(excID e) {
  cerr<<"Exception n. "<<e<<" raised."<<endl;
  cerr<<"Aborting system ...."<<endl;
  abort();
};
#endif

#endif
