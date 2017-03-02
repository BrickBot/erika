#include "fiforeceiver.hpp"
#include "control_new.cpp"
#include "actoroutput.hpp"
#include "actorinput.hpp"

void testcontrol1(fstream & dbg)
{
  
  try {
    fstream f("x.mat",ios::in);
    //Dichiarazioni canali
    Channel<int> c0, c1,c6,c7;
    Channel<double> c2, c3, c4,c5;

    
    //Dichiarazione primo attore (emissione dati)
    //che arrivano su c0 e c1
    ActorInput<int,2> source(dbg,cin);    
    OutputPort<int> source_o1;
    OutputPort<int> source_o2;
    source_o1.addChannel(0, &c0);
    source_o2.addChannel(0, &c1);
    source.setOutputPort(0, &source_o1);
    source.setOutputPort(1, &source_o2);
   
	
    //Dichiarazione secondo attore (decoder)
    ActorInputDecoder<int,double> id;
    InputPort<int,1> id_i1;
    InputPort<int,1> id_i2;
    OutputPort<double> id_o1;
    OutputPort<double> id_o2;
    FifoReceiver<int> id_rec1;
    FifoReceiver<int> id_rec2;
    id_i1.addReceiver(0, &id_rec1);
    id_i2.addReceiver(0, &id_rec2);
    c0.connectReceiver(&id_rec1);
    c1.connectReceiver(&id_rec2);
    id.setPorts(&id_i1, &id_i2, &id_o1, &id_o2);
    id.setYscale(5e-4);
    id.setQ(0.07);
    id.setYd(800,600);
    id_o1.addChannel(0,&c2);
    id_o2.addChannel(0,&c3);


    ActorStateFeedback<double> sf(dbg);
    FifoReceiver<double> sf_rec1;
    InputPort<double,1> sf_i1;
    sf_i1.addReceiver(0,&sf_rec1);
    FifoReceiver<double> sf_rec2;
    InputPort<double,1> sf_i2;
    sf_i2.addReceiver(0,&sf_rec2);
    c2.connectReceiver(&sf_rec1);
    c3.connectReceiver(&sf_rec2);
    OutputPort<double> sf_o1;
    OutputPort<double> sf_o2;
    OutputPort<double> sf_o3;
    sf.setPorts(&sf_i1, &sf_i2, &sf_o1, &sf_o2, &sf_o3);
    sf.setAlpha(-0.9);
    sf.setBeta(-0.5);
    sf.setVref(+0.2);

    sf_o1.addChannel(0, &c4);
    sf_o2.addChannel(0, &c5);
    //sf_o3.addChannel(0, &c5);


    ActorOutputEncoder<double,int> oe;
    InputPort<double,1> oe_i1;
    InputPort<double,1> oe_i2;
    OutputPort<int> oe_o1;
    OutputPort<int> oe_o2;
    FifoReceiver<double> oe_rec1;
    FifoReceiver<double> oe_rec2;
    oe_i1.addReceiver(0, &oe_rec1);
    oe_i2.addReceiver(0, &oe_rec2);
    c4.connectReceiver(&oe_rec1);
    c5.connectReceiver(&oe_rec2);
    oe.setPorts(&oe_i1, &oe_i2, &oe_o1, &oe_o2);
    oe_o1.addChannel(0, &c6);
    oe_o2.addChannel(0, &c7);
    oe.setG0r(-25.8);
    oe.setG1r(201.3);
    oe.setG0l(-26.67);
    oe.setG1l(208.5);
    
    InputPort<int,1> ou_i1;
    InputPort<int,1> ou_i2;
    FifoReceiver<int> ou_rec1;
    FifoReceiver<int> ou_rec2;
    ou_i1.addReceiver(0, &ou_rec1);
    ou_i2.addReceiver(0, &ou_rec2);
    c6.connectReceiver(&ou_rec1);
    c7.connectReceiver(&ou_rec2);
    ActorOutput<int,2> ou(dbg,cout);
    ou.setInputPort(0, &ou_i1);
    ou.setInputPort(1, &ou_i2);
   
    source.init();
    id.init();
    sf.init();
    oe.init();
    ou.init();
    for(int i = 0; i<1000;i++) {
      source.fire();
      id.fire();
      sf.fire();
      oe.fire();
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
