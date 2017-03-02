#include "namedobject.hpp"
#include "actor.hpp"
#include "baseexc.hpp"
#include "check.hpp"
#include "port.hpp"
#ifdef _DEBUG_
#include <fstream>
#endif


template<typename INT, typename FLOAT>
class ActorInputDecoder :  public Actor, public NamedObj {
  //PARAMETERS
  FLOAT _yscale, _yd, _q;
  
  FLOAT _y1, _y2;

  //PORTS
  InputPort<INT,1> * _in1;
  InputPort<INT,1> * _in2;
  OutputPort<FLOAT> * _out1;
  OutputPort<FLOAT> * _out2;

  //results
  FLOAT _y, _th;

public:
  ActorInputDecoder(namedObjID n = nullObjID):
    NamedObj(n),_yscale(0),_yd(0),_q(0), _in1(0), _in2(0), _out1(0), _out2(0), _y1(0), _y2(0)
  {    
  };
  
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  

  void setPorts(InputPort<INT,1> *i1,  
		InputPort<INT,1> *i2,  
		OutputPort<FLOAT> * o1,
		OutputPort<FLOAT> * o2)
  {
    _in1 = i1;
    _in2 = i2;
    _out1 = o1;
    _out2 = o2;
  };

  void setYd(FLOAT yref, FLOAT ywhite) {
    _yd = yref-ywhite;
  };

  void setYscale(FLOAT yw) {
    _yscale=yw;
  };
  
  void setQ(FLOAT q) {
    _q = q;
      };
#ifdef _EXC_
  virtual void init() throw (Exc)
#else
    virtual void init()
#endif
  {
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ACTOR_SLOT_EMPTY_EXC);
  };  
  #ifdef _EXC_
  virtual void fire() throw (Exc)
#else
    virtual void fire()
#endif
  {
    //Init should be called first!!!
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ASSERTION_FAILED_EXC);
    int t;      
    
    _y1 = FLOAT(_in1->receive(0,t));
    _y2 = FLOAT(_in2->receive(0,t));
	

    _y = (_y1 - _yd)*_yscale;
    _th = ((_y1-_y2)/_q)*_yscale;
    
    _out1->broadcast(_y, t);
    _out2->broadcast(_th, t);
  };
};


template<typename FLOAT, typename INT>
class ActorOutputEncoder :  public Actor, public NamedObj {
  FLOAT _G0r, _G1r, _G0l, _G1l; 

  InputPort<FLOAT,1> * _in1;
  InputPort<FLOAT,1> * _in2;
  OutputPort<INT> * _out1;
  OutputPort<INT> * _out2;
  
  FLOAT _vr1, _vl1; 
  
  INT _vr, _vl;
public:
  ActorOutputEncoder(namedObjID n = nullObjID):
    NamedObj(n), _out1(0), _out2(0), _vr1(0), _vl1(0),_vr(0), _vl(0) 
  {    
  };
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };  

  void setPorts(InputPort<FLOAT,1> *i1,  
		InputPort<FLOAT,1> *i2,  
		OutputPort<INT> * o1,
		OutputPort<INT> * o2)
  {
    _in1 = i1;
    _in2 = i2;
    _out1 = o1;
    _out2 = o2;
  };

  void setG0l(FLOAT G0l) {
    _G0l = G0l;
  };

  void setG1l(FLOAT G1l) {
    _G1l = G1l;
  };

  void setG0r(FLOAT G0r) {
    _G0r = G0r;
  };
  
  void setG1r(FLOAT G1r) {
    _G1r = G1r;
  };
  #ifdef _EXC_
  virtual void init() throw (Exc)
#else
    virtual void init()
#endif
  {
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ACTOR_SLOT_EMPTY_EXC);
  };  
  #ifdef _EXC_
  virtual void fire() throw (Exc)
#else
    virtual void fire()
#endif
  {
    //Init should be called first!!!
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ASSERTION_FAILED_EXC);
    int t;      
    
    _vr1 = _in1->receive(0,t);
    _vl1 = _in2->receive(0,t);

    _vr1 = _G0r + _G1r*_vr1;
    _vl1 = _G0l + _G1l*_vl1;
    
    _vr = INT(_vr1);
    _vl = INT(_vl1);
    _out1->broadcast(_vr, t);
    _out2->broadcast(_vl, t);
  };

};

template<typename FLOAT>
class ActorStateFeedback:  public Actor, public NamedObj {
  //Gains
  FLOAT _alpha, _beta, _vref;

  //Ports
  InputPort<FLOAT,1> * _in1;
  InputPort<FLOAT,1> * _in2;
  OutputPort<FLOAT> * _out1;
  OutputPort<FLOAT> * _out2;
  OutputPort<FLOAT> * _out3;
    
  //Sensor variables
  //Here to avoid local variables
  FLOAT _y, _th;

  //Data for actuators
  FLOAT _vr, _vl, _u;
  
#ifdef _DEBUG_
  fstream & _f;
#endif
public:
#ifdef _DEBUG_
  ActorStateFeedback(fstream &f, namedObjID n = nullObjID):
    NamedObj(n), _f(f),_in1(0), _in2(0), _out1(0), _out2(0), _y(0), _th(0), _vr(0), _vl(0), _u(0)
  {    
  };
#else
  ActorStateFeedback(namedObjID n = nullObjID):
    NamedObj(n), _in1(0), _in2(0), _out1(0), _out2(0), _y(0), _th(0), _vr(0), _vl(0),_u(0)
  {    
  };
#endif
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  

  void setPorts(InputPort<FLOAT,1> *i1,  
		InputPort<FLOAT,1> *i2,  
		OutputPort<FLOAT> * o1,
		OutputPort<FLOAT> * o2,
		OutputPort<FLOAT> * o3) 
  {
    _in1 = i1;
    _in2 = i2;
    _out1 = o1;
    _out2 = o2;
    _out3 = o3;
  };
  
  void setAlpha(FLOAT a) {
    _alpha=a;
  };

  void setBeta(FLOAT b) {
    _beta=b;
  };
  
  void setVref(FLOAT vr) {
    _vref = vr;
  };
      

#ifdef _EXC_
  virtual void init() throw (Exc)
#else
    virtual void init()
#endif
  {
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_out3==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ACTOR_SLOT_EMPTY_EXC);
  };
  
#ifdef _EXC_
  virtual void fire() throw (Exc)
#else
    virtual void fire()
#endif
  {
    //Init should be called first!!!
    _checkCondObj(((_out1==0)||
		   (_out2==0)||
		   (_out3==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ASSERTION_FAILED_EXC);
    int t;      
#ifdef _DEBUG_
    _f<<"Fire called by actor "<<getID()<<endl;
#endif
    _y = (_in1->receive(0,t));
#ifdef _DEBUG_
    if(t) 
      {
	_f<<"First token received"<<endl;
	_f<<"value: "<<_y<<endl;
      };
#endif
    _th = (_in2->receive(0,t));
#ifdef _DEBUG_
    if(t) 
      {
	_f<<"Second token received"<<endl;
	_f<<"value: "<<_th<<endl;
      };
#endif
    _u = _alpha * _y + _beta * _th;
    _vr = _vref + _u/2;
    _vl = _vref - _u/2;

    
    _out1->broadcast(_vr, t);
    _out2->broadcast(_vl, t);
    _out3->broadcast(_u, t);
  };
};

template<typename FLOAT>
class ActorObserver:  public Actor, public NamedObj {
  //Gains
  FLOAT _L, _T, _vref, _b;
  FLOAT _Kt, _Ku;


  //Ports
  InputPort<FLOAT,1> * _in1;
  InputPort<FLOAT,1> * _in2;
  OutputPort<FLOAT> * _out1;
    
  //Sensor variables
  //Here to avoid local variables
  FLOAT _y, _y1,_u;

  //Data for actuators
  FLOAT _th;
  
#ifdef _DEBUG_
  fstream & _f;
#endif
public:
#ifdef _DEBUG_
  ActorObserver(fstream &f, namedObjID n = nullObjID):
    NamedObj(n), _f(f),_in1(0), _in2(0), _out1(0), _y(0), _y1(0), _th(0), _u(0)
  {    
  };
#else
  ActorObserver(namedObjID n = nullObjID):
    NamedObj(n), _in1(0), _in2(0), _out1(0), _y(0), _y1(0), _th(0), _u(0)
  {    
  };
#endif
  static TypeID getTypeID() {
    return ACTOR_TYPE_ID;
  };
  
  bool checkType(TypeID ty) {
    return ((ty==ACTOR_TYPE_ID)|
	    NamedObj::checkType(ty));
  };
  

  void setPorts(InputPort<FLOAT,1> *i1,  
		InputPort<FLOAT,1> *i2,  
		OutputPort<FLOAT> * o1)
  {
    _in1 = i1;
    _in2 = i2;
    _out1 = o1;
  };
  
  void setL(FLOAT L) {
    _L=L;
  };

  void setT(FLOAT T) {
    _T=T;
  };
  
  void setVref(FLOAT vr) {
    _vref = vr;
  };
      
  void setB(FLOAT b) {
    _b = b;
  };
#ifdef _EXC_
  virtual void init() throw (Exc)
#else
    virtual void init()
#endif
  {
    _checkCondObj(((_out1==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ACTOR_SLOT_EMPTY_EXC);
    FLOAT d = _vref * _T;
    FLOAT tau = _T/(2*_b);
    _Kt = (1-_L*_vref*_T);
    _Ku = (2-_L*d)*tau;
  };
  
#ifdef _EXC_
  virtual void fire() throw (Exc)
#else
    virtual void fire()
#endif
  {
    //Init should be called first!!!
    _checkCondObj(((_out1==0)||
		   (_in1==0)||
		   (_in2==0)), 
		  ASSERTION_FAILED_EXC);
    int t;      
#ifdef _DEBUG_
    _f<<"Fire called by actor "<<getID()<<endl;
#endif
    _y = (_in1->receive(0,t));
#ifdef _DEBUG_
    if(t) 
      {
	_f<<"First token received"<<endl;
	_f<<"value: "<<_y<<endl;
      };
#endif
    _u = (_in2->receive(0,t));
#ifdef _DEBUG_
    if(t) 
      {
	_f<<"Second token received"<<endl;
	_f<<"value: "<<_th<<endl;
      };
#endif
    _th = _Kt * _th + _Ku * _u + _L*(_y-_y1);
    _y1=_y;
    
    _out1->broadcast(_th, t);
  };
};


