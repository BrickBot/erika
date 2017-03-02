#include "fiforeceiver.hpp"
#include "control_new.cpp"
#include "actoroutput.hpp"
#include "actorinput.hpp"

void testcontrol1(fstream & dbg)
{
  
  try {
    fstream f("x.mat",ios::in);
    
    Channel<double> c0, c1, c2, c3, c4;
    OutputPort<double> o01, o02;
    o01.addChannel(0, &c0);
    o02.addChannel(0, &c1);

    ActorInput<double,2> ai(dbg,cin);    
    ai.setOutputPort(0, &o01);
    ai.setOutputPort(1, &o02);

    ActorStateFeedback<double> sf(dbg);
    FifoReceiver<double> r1;
    InputPort<double,1> i1;
    i1.addReceiver(0,&r1);
    FifoReceiver<double> r2;
    InputPort<double,1> i2;
    i2.addReceiver(0,&r2);
    c0.connectReceiver(&r1);
    c1.connectReceiver(&r2);
    OutputPort<double> o1;
    OutputPort<double> o2;
    OutputPort<double> o3;
    sf.setPorts(&i1, &i2, &o1, &o2, &o3);
    sf.setAlpha(-0.9);
    sf.setBeta(-0.5);
    sf.setVref(+0.5);

    o1.addChannel(0,&c2);
    o2.addChannel(0, &c3);
    o3.addChannel(0, &c4);

    InputPort<double,1> p5;
    InputPort<double,1> p6;
    InputPort<double,1> p7;
    FifoReceiver<double> f2;
    FifoReceiver<double> f3;
    FifoReceiver<double> f4;
    p5.addReceiver(0, &f2);
    p6.addReceiver(0, &f3);
    p7.addReceiver(0, &f4);
    c2.connectReceiver(&f2);
    c3.connectReceiver(&f3);
    c4.connectReceiver(&f4);
    ActorOutput<double,3> a3(dbg,cout);
    a3.setInputPort(0, &p5);
    a3.setInputPort(1, &p6);
    a3.setInputPort(2, &p7);

    ai.init();
    sf.init();
    a3.init();
    for(int i = 0; i<1000;i++) {
      ai.fire();
      sf.fire();
      a3.fire();
    };

  } catch (Exc & e) {
    dbg <<"Exception: "<< e.what()<<" caught."<<endl;
    dbg <<"File: "<< e.file()<<endl;
    dbg <<"Line: "<< e.line()<<endl;
    dbg <<"Name: "<< e.object()<<endl;
  };
}

int main() {
  fstream dbg("output_test.txt", ios::out);
  testcontrol1(dbg);
  return 0;  
};
