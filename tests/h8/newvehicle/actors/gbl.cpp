/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: gbl.cpp,v 1.2 2003/11/11 13:53:54 pj Exp $
 */

#include "erika.h"
#include "include/FPdouble.hpp"
#include "include/rewritereceiver.hpp"
#include "control_new.hpp"



//Dichiarazioni canali
Channel<ERIKA_INT16> c0, c1,c6,c7;
Channel<FPdouble> c2, c3, c4,c5;

    
//Dichiarazione primo attore (emissione dati)
//che arrivano su c0 e c1
//ActorInput<ERIKA_INT16,2> source(dbg,cin);    
OutputPort<ERIKA_INT16> source_o1;
OutputPort<ERIKA_INT16> source_o2;

//Dichiarazione secondo attore (decoder)
ActorInputDecoder<ERIKA_INT16,FPdouble> id;
InputPort<ERIKA_INT16,1> id_i1;
InputPort<ERIKA_INT16,1> id_i2;
OutputPort<FPdouble> id_o1;
OutputPort<FPdouble> id_o2;
RewriteReceiver<ERIKA_INT16> id_rec1;
RewriteReceiver<ERIKA_INT16> id_rec2;

ActorStateFeedback<FPdouble> sf;
RewriteReceiver<FPdouble> sf_rec1;
InputPort<FPdouble,1> sf_i1;

RewriteReceiver<FPdouble> sf_rec2;
InputPort<FPdouble,1> sf_i2;

OutputPort<FPdouble> sf_o1;
OutputPort<FPdouble> sf_o2;
OutputPort<FPdouble> sf_o3;



ActorObserver<FPdouble> obs;
RewriteReceiver<FPdouble> obs_rec1;
RewriteReceiver<FPdouble> obs_rec2;
InputPort<FPdouble,1> obs_i1;
InputPort<FPdouble,1> obs_i2;
OutputPort<FPdouble> obs_o1;
Channel<FPdouble> c_obsout;
Channel<FPdouble> c_sf;

ActorOutputEncoder<FPdouble,ERIKA_INT16> oe;
InputPort<FPdouble,1> oe_i1;
InputPort<FPdouble,1> oe_i2;
OutputPort<ERIKA_INT16> oe_o1;
OutputPort<ERIKA_INT16> oe_o2;
RewriteReceiver<FPdouble> oe_rec1;
RewriteReceiver<FPdouble> oe_rec2;

InputPort<ERIKA_INT16,1> ou_i1;
InputPort<ERIKA_INT16,1> ou_i2;
RewriteReceiver<ERIKA_INT16> ou_rec1;
RewriteReceiver<ERIKA_INT16> ou_rec2;

ActorMotorOutput<ERIKA_INT16> ou;

#ifdef ACTORDEBUG
ActorDebugTheta<FPdouble> ddd;
InputPort<FPdouble,1> ddd_p_obs;
InputPort<FPdouble,1> ddd_p_st;
RewriteReceiver<FPdouble> ddd_r_obs;
RewriteReceiver<FPdouble> ddd_r_st;
#endif

void GBL_init(void)
{
  int temp;

  //Dichiarazione primo attore (emissione dati)
  //che arrivano su c0 e c1
  
  source_o1.addChannel(0, &c0);
  source_o2.addChannel(0, &c1);
  //source.setOutputPort(0, &source_o1);
  //source.setOutputPort(1, &source_o2);
  
  //Dichiarazione secondo attore (decoder)
  id_i1.addReceiver(0, &id_rec1);
  id_i2.addReceiver(0, &id_rec2);
  c0.connectReceiver(&id_rec1);
  c1.connectReceiver(&id_rec2);
  id.setPorts(&id_i1, &id_i2, &id_o1, &id_o2);
  id.setQ(FPdouble(7)/FPdouble(100));
  //id.setYd(800,600); set reading data
  id.setYscale(FPdouble(5)/FPdouble(10000));  // PJ: da cambiare?
  id_o1.addChannel(0,&c2);
  id_o2.addChannel(0,&c3);
  
  sf_i1.addReceiver(0,&sf_rec1);
  
  sf_i2.addReceiver(0,&sf_rec2);
  c2.connectReceiver(&sf_rec1);
  c3.connectReceiver(&sf_rec2);
  
  sf.setPorts(&sf_i1, &sf_i2, &sf_o1, &sf_o2, &sf_o3);
  sf.setAlpha(FPdouble(-20));
  sf.setBeta(FPdouble(-5));
  sf.setVref(FPdouble(6)/FPdouble(10));
  
  sf_o1.addChannel(0, &c4);
  sf_o2.addChannel(0, &c5);
  sf_o3.addChannel(0, &c_sf);

  obs_i1.addReceiver(0, &obs_rec1);
  obs_i2.addReceiver(0, &obs_rec2);
  c2.connectReceiver(&obs_rec1);
  c_sf.connectReceiver(&obs_rec2);
  c_obsout.connectReceiver(&sf_rec2);

  obs_rec2.put(0,temp);
  obs.setPorts(&obs_i1,&obs_i2,&obs_o1);
  obs_o1.addChannel(0, &c_obsout);
  obs.setL(-10);
  obs.setT(FPdouble(6)/FPdouble(1000));
  obs.setB(FPdouble(105)/FPdouble(1000));
  
  oe_i1.addReceiver(0, &oe_rec1);
  oe_i2.addReceiver(0, &oe_rec2);
  c4.connectReceiver(&oe_rec1);
  c5.connectReceiver(&oe_rec2);
  oe.setPorts(&oe_i1, &oe_i2, &oe_o1, &oe_o2);
  oe_o1.addChannel(0, &c6);
  oe_o2.addChannel(0, &c7);
  oe.setG0r(FPdouble(-258)/FPdouble(10));
  oe.setG1r(FPdouble(2103)/FPdouble(10));
  oe.setG0l(FPdouble(2667)/FPdouble(100));
  oe.setG1l(FPdouble(2085)/FPdouble(10));
  
  ou_i1.addReceiver(0, &ou_rec1);
  ou_i2.addReceiver(0, &ou_rec2);
  c6.connectReceiver(&ou_rec1);
  c7.connectReceiver(&ou_rec2);
  
  ou.setInputPort(0, &ou_i1);
  ou.setInputPort(1, &ou_i2);

#ifdef ACTORDEBUG
  ddd.setInputPort(0, &ddd_p_st);
  ddd.setInputPort(1, &ddd_p_obs);
  c3.connectReceiver(&ddd_r_st);
  c_obsout.connectReceiver(&ddd_r_obs);
  ddd_p_obs.addReceiver(0,&ddd_r_obs);
  ddd_p_st.addReceiver(0,&ddd_r_st);
#endif

  //source.init();
  id.init();
  obs.init();
  sf.init();
  oe.init();
  ou.init();

#ifdef ACTORDEBUG
  ddd.init();
#endif
}
