#include "fiforeceiver.hpp"
#include "control_new.cpp"
#include "actoroutput.hpp"
#include "actorinput.hpp"

void testcontrol1(fstream & dbg)
{
  
  try {
    fstream f("x.mat",ios::in);
    
    Channel<double> c0, c1, c2, c3, c4;

    
    OutputPort<double> source_o1;
    source_o1.addChannel(0, &c0);
    ActorInput<double,1> source(dbg,cin);    
    source.setOutputPort(0, &source_o1);
        
    ActorObserver<double> obs(dbg);
    FifoReceiver<double> obs_rec1;
    FifoReceiver<double> obs_rec2;
    InputPort<double,1> obs_i1;
    InputPort<double,1> obs_i2;
    OutputPort<double> obs_o1;
    obs_i1.addReceiver(0, &obs_rec1);
    obs_i2.addReceiver(0, &obs_rec2);
    c0.connectReceiver(&obs_rec1);
    c4.connectReceiver(&obs_rec2);
    int t;
    obs_rec2.put(0,t);
    obs.setPorts(&obs_i1,&obs_i2,&obs_o1);
    obs.setL(1);
    obs.setT(0.004);
    obs.setB(0.105);
    obs.setVref(0.2);
    obs_o1.addChannel(0, &c1);

    ActorStateFeedback<double> sf(dbg);
    FifoReceiver<double> sf_rec1;
    InputPort<double,1> sf_i1;
    sf_i1.addReceiver(0,&sf_rec1);
    FifoReceiver<double> sf_rec2;
    InputPort<double,1> sf_i2;
    sf_i2.addReceiver(0,&sf_rec2);
    c0.connectReceiver(&sf_rec1);
    c1.connectReceiver(&sf_rec2);
    OutputPort<double> sf_o1;
    OutputPort<double> sf_o2;
    OutputPort<double> sf_o3;
    sf.setPorts(&sf_i1, &sf_i2, &sf_o1, &sf_o2, &sf_o3);
    sf.setAlpha(-30);
    sf.setBeta(-5);
    sf.setVref(+0.2);

    sf_o1.addChannel(0, &c2);
    sf_o2.addChannel(0, &c3);
    sf_o3.addChannel(0, &c4);
    //sf_o3.addChannel(0, &c5);

    InputPort<double,1> ou_i1;
    InputPort<double,1> ou_i2;
    InputPort<double,1> ou_i3;
    FifoReceiver<double> ou_rec1;
    FifoReceiver<double> ou_rec2;
    FifoReceiver<double> ou_rec3;
    ou_i1.addReceiver(0, &ou_rec1);
    ou_i2.addReceiver(0, &ou_rec2);
    ou_i3.addReceiver(0, &ou_rec3);
    c2.connectReceiver(&ou_rec1);
    c3.connectReceiver(&ou_rec2);
    c4.connectReceiver(&ou_rec3);
    ActorOutput<double,3> ou(dbg,cout);
    ou.setInputPort(0, &ou_i1);
    ou.setInputPort(1, &ou_i2);
    ou.setInputPort(2, &ou_i3);

    source.init();
    obs.init();
    sf.init();
    ou.init();
    for(int i = 0; i<1000;i++) {
      source.fire();
      obs.fire();
      sf.fire();
      ou.fire();
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
