#ifndef _MODE_H_
#define _MODE_H_

typedef unsigned short ModeID;

class Mode {
public:
  virtual void enterMode(ModeID oldmode)=0;
  virtual void exitMode(ModeID newmode)=0;
};

#endif
