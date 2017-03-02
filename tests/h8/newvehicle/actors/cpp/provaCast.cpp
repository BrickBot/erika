#include "namedobject.hpp"
#include "baseexc.hpp"

#include <fstream>

class MyChannel : public NamedObj {
  fstream & f;
public:
  MyChannel(fstream & ifs, namedObjID n = nullObjID) : NamedObj(n), f(ifs) {
    f<<"MyChannel " << (int) getID() <<" regularly created."<<endl; 
    f<<"Typeid: "<<getTypeID()<<endl;
  };
  static TypeID getTypeID() {
    return CHANNEL_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==CHANNEL_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
};

class MyInputPort : public NamedObj {
  fstream & f;
public:
  MyInputPort( fstream & ifs,namedObjID n = nullObjID) : NamedObj(n), f(ifs) {
    f<<"MyInputPort " << (int) getID() <<" regularly created."<<endl;  
    f<<"Typeid: "<<getTypeID()<<endl;
  };
  static TypeID getTypeID() {
    return INPUT_PORT_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==INPUT_PORT_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
};


void testcast(fstream & dbg)
{
  MyChannel pippo(dbg, 0);
  MyInputPort pippo1(dbg);
  
  dbg<<"Initialising pointer t to NamedObj to an instance of MyChannel"<<endl;
  NamedObj * t = &pippo;
  
  dbg<<"Casting to p, pointer to MyChannel"<<endl;
  MyChannel * p = my_dynamic_cast<MyChannel> (t);
  if (p!=0)
    dbg <<"OK!"<<endl;
  else
   dbg <<"FAILED!"<<endl;
  dbg<<"Casting to p1, pointer to MyInputPort"<<endl;
  MyInputPort * p1 = my_dynamic_cast<MyInputPort> (t);
  if (p1==0)
    dbg <<"OK!"<<endl;
  else
    dbg <<"FAILED!"<<endl;
  dbg<<"Initialising p1 to a pointer to MyInputPort"<<endl;
  p1 = &pippo1;
  dbg<<"Casting to a pointer to NamedObject"<<endl;
  t = my_dynamic_cast<NamedObj>(p1);
  if (t!=0)
    dbg <<"OK!"<<endl;
  else
    dbg <<"FAILED!"<<endl;

}


int main() {
  fstream dbg("output_test.txt", ios::out);
  testcast(dbg);
  return 0;
};
