#ifndef __NAMEDOBJ_HPP___
#define __NAMEDOBJ_HPP___
#include "baseexc.hpp"
#include "cfg.hpp"
#include "typed.hpp"

class NamedObj: public TypedObj {
  static NamedObj * _index[namedMaxNum];
  static int _IDcount;
  int _ID;

public:

  /** 
   * Base constructor.  
   *  
   * @param n A (possibly unique) name for the NamedObj: by default the
   * NamedObj name is set equal to the typeid() + the NamedObj ID.*/
  #ifdef _EXC_
  NamedObj(namedObjID t = nullObjID) throw(Exc);
  #else
  NamedObj(namedObjID t = nullObjID);
  #endif
  
  static TypeID getTypeID() {
    return NAMEDOBJ_TYPE_ID;
  };

  bool checkType(TypeID ty) {
    return (ty==NAMEDOBJ_TYPE_ID);
  };

  /// Destructor
  virtual ~NamedObj();
  
  static void initialiseID();

  ///Get the NamedObj name
  inline const namedObjID &getID() { return _ID; };

  /** 
   *  Returns the pointer to the NamedObj with the specified name or
   *  NULL if such NamedObj does not exists.  */
  static NamedObj * _find(const namedObjID & who) {
    return _index[who];
  };
};

#endif
