#include "namedobject.hpp"
#include "baseexc.hpp"

#include <fstream>

class MyNamedObj : public NamedObj {
  fstream & f;
public:
  MyNamedObj(fstream & ifs, namedObjID n = nullObjID) : NamedObj(n), f(ifs) {
    f<<"MyNamedObject " << (int) getID() <<" regularly created."<<endl; 
  };
};

class MyNamedObj1 : public NamedObj {
  fstream & f;
public:
  MyNamedObj1( fstream & ifs,namedObjID n = nullObjID) : NamedObj(n), f(ifs) {
    f<<"MyNamedObject1 " << (int) getID() <<" regularly created."<<endl; 
  };
};


void testnamedobj(fstream & dbg)
{
  try {
    MyNamedObj pippo(dbg, 0);
    MyNamedObj pippo1(dbg);
    MyNamedObj1 pluto(dbg,2);
    MyNamedObj1 pluto1(dbg);
    MyNamedObj1 pluto2(dbg,2);
  } catch (Exc & e) {
    dbg <<"Exception "<< e.what()<<" caught."<<endl;
    dbg <<"File "<< e.file()<<endl;
    dbg <<"Line "<< e.line()<<endl;
  };
}

int main() {
  fstream dbg("output_test.txt", ios::out);
  testnamedobj(dbg);
  return 0;
};
