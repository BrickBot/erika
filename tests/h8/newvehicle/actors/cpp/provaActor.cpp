#include "namedobject.hpp"
#include "baseexc.hpp"
#include "actor.hpp"
#include "port.hpp"
#include "fiforeceiver.hpp"

#include <fstream>
#include <iostream>
#include "actorsource.hpp"
#include "actoraccumulator.hpp"
#include "actoroutput.hpp"

void testactor(fstream & dbg)
{
  
  try {
    Channel<int> c0;
    Channel<int> c1;
    
    OutputPort<int,3> p1;
    ActorConstSource<int,3> a0(dbg, 5);
    a0.setPort(&p1);
    p1.addChannel(0,&c0);
    
    InputPort<int,1> p2;
    OutputPort<int,3> p3;
    ActorAccumulator<int,3> a1(dbg);
    FifoReceiver<int,1> f0;
    p2.addReceiver(0, &f0);
    a1.setInputPort(&p2);
    a1.setOutputPort(&p3);
    c0.connectReceiver(&f0);
    p3.addChannel(0, &c1);

    InputPort<int,1> p4;
    FifoReceiver<int,1> f1;
    p4.addReceiver(0, &f1);
    c1.connectReceiver(&f1);
    ActorOutput<int,1> a2(dbg,cout);
    a2.setInputPort(0, &p4);
    
    InputPort<int,1> p5;
    InputPort<int,1> p6;
    FifoReceiver<int,1> f2;
    FifoReceiver<int,1> f3;
    p5.addReceiver(0, &f2);
    p6.addReceiver(0, &f3);
    c1.connectReceiver(&f3);
    c0.connectReceiver(&f2);
    ActorOutput<int,2> a3(dbg,cout);
    a3.setInputPort(0, &p5);
    a3.setInputPort(1, &p6);


    a0.init();
    a1.init();
    a2.init();
    a3.init();

    for (int i = 0; i<100;i++) 
      {
	a0.fire();
	a1.fire();
	a2.fire();
	a3.fire();
      }


  } catch (Exc & e) {
    dbg <<"Exception "<< e.what()<<" caught."<<endl;
    dbg <<"File "<< e.file()<<endl;
    dbg <<"Line "<< e.line()<<endl;
  };
}

int main() {
  fstream dbg("output_test.txt", ios::out);
  testactor(dbg);
  return 0;
};
