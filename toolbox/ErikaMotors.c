/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2004 Riccardo Vestrini
 *
 * This program is distributed under the license contained into the
 * license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: ErikaMotors.c,v 1.1 2004/06/01 16:42:36 vestrini Exp $
 */

#define S_FUNCTION_NAME  ErikaMotors
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include <string.h>


/* parameters */
 /* motor on port A */
boolean_T is_portA(SimStruct *S);
 /* motor on port C */
boolean_T is_portC(SimStruct *S);
/* number of motors (1 or 2) */
uint8_T nports(SimStruct *S); 
/* use different motor implementation, do not use port B for application 
   and do not control motor direction */
boolean_T is_fast(SimStruct *S);
/* set motor pins to Vcc when motor is off (see Erika manual) */
boolean_T is_brake(SimStruct *S);
/* direction (clockwise or anticlockwise) */
boolean_T directionA(SimStruct *S);
boolean_T directionC(SimStruct *S);
/* motor duty cycle */
uint8_T powerA(SimStruct *S);
uint8_T powerC(SimStruct *S);

#ifndef MATLAB_MEX_FILE
#  error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif


static void mdlCheckParameters(SimStruct *S)
{
  if ( mxIsComplex(ssGetSFcnParam(S,0)) ||
       !mxIsDouble(ssGetSFcnParam(S,0)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,0)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,0)) != 1) ||
       ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) < 1 &&
         *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) > 3 )
       ) {
    ssSetErrorStatus(S, "First parameter (output ports used) has to be a non "
		     "complex double type, with 2 dimensions and one element"
		     "with value 1 (A), 2 (C) or 3 (A and C)");
    return;
  }
  if ( mxIsComplex(ssGetSFcnParam(S,1)) ||
       !mxIsDouble(ssGetSFcnParam(S,1)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,1)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,1)) != 1)  
       ) {
    ssSetErrorStatus(S, "Second parameter (fast implementation) has to be a non "
		     "complex double type, with 2 dimensions and one element"
		     "with value 2 (no) or  other (yes)");
    return;
  }
  if ( mxIsComplex(ssGetSFcnParam(S,2)) ||
       !mxIsDouble(ssGetSFcnParam(S,2)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,2)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,2)) != 1)  
       ) {
    ssSetErrorStatus(S, "Third parameter (brake when off) has to be a non "
		     "complex double type, with 2 dimensions and one element"
		     "with value 2 (no) or  other (yes)");
    return;
  }
}


static void mdlInitializeSizes(SimStruct *S)
{
  int i;

  /* Parameters */
  ssSetNumSFcnParams(S, 3);
  ssSetSFcnParamTunable(S, 0, 0);
  ssSetSFcnParamTunable(S, 1, 0);
  ssSetSFcnParamTunable(S, 2, 0);

  if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
    mdlCheckParameters(S);
    if( ssGetErrorStatus(S) != NULL ) return;
  } 
  else {
    ssSetErrorStatus(S, "3 Parameters expected: "
		     "output ports used, fast implementation " 
                     "and brake when off");
    return; 
  }

  /* sample times */
  ssSetNumSampleTimes(S, 1);


  /* Number of output ports */
  if (!ssSetNumOutputPorts(S, nports(S))) return;

  /* outports number */
  for (i=0; i<nports(S);i++) {
    if(!ssSetOutputPortVectorDimension(S, i, 1)) return;
    ssSetOutputPortOptimOpts(S, i, SS_REUSABLE_AND_LOCAL);
  }

  /* Number of input ports */
  if (!ssSetNumInputPorts(S, nports(S)*(1+!is_fast(S)) )) return;


  /* inport number: 0 */
  if(!ssSetInputPortVectorDimension(S, 0, 1)) return;
  ssSetInputPortDataType(S, 0, SS_UINT8);
  ssSetInputPortDirectFeedThrough(S, 0, 1);
  ssSetInputPortOverWritable(S, 0, 0);
  ssSetInputPortOptimOpts(S, 0, SS_NOT_REUSABLE_AND_GLOBAL);

  /* direction control not needed in fast mode */
  if (!is_fast(S)) {
    /* inport number: 1: direction */
    if(!ssSetInputPortVectorDimension(S, 1, 1)) return;
    ssSetInputPortDataType(S, 1, SS_BOOLEAN);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortOverWritable(S, 1, 0);
    ssSetInputPortOptimOpts(S, 1, SS_NOT_REUSABLE_AND_GLOBAL);
  }

  if (nports(S)==2)  {
    if (is_fast(S)) {
      /* port 1: power */
      if(!ssSetInputPortVectorDimension(S, 1, 1)) return;
      ssSetInputPortDataType(S, 1, SS_UINT8);
      ssSetInputPortDirectFeedThrough(S, 1, 1);
      ssSetInputPortOverWritable(S, 1, 0);
      ssSetInputPortOptimOpts(S, 1, SS_NOT_REUSABLE_AND_GLOBAL);
    } else {
      /* port 2: direction */
      if(!ssSetInputPortVectorDimension(S, 2, 1)) return;
      ssSetInputPortDataType(S, 2, SS_UINT8);
      ssSetInputPortDirectFeedThrough(S, 2, 1);
      ssSetInputPortOverWritable(S, 2, 0);
      ssSetInputPortOptimOpts(S, 2, SS_NOT_REUSABLE_AND_GLOBAL);

      /* inport number 3: direction */
      if(!ssSetInputPortVectorDimension(S, 3, 1)) return;
      ssSetInputPortDataType(S, 3, SS_BOOLEAN);
      ssSetInputPortDirectFeedThrough(S, 3, 1);
      ssSetInputPortOverWritable(S, 3, 0);
      ssSetInputPortOptimOpts(S, 3, SS_NOT_REUSABLE_AND_GLOBAL);
    }
  }


  /* Options */
  ssSetOptions(S, (SS_OPTION_RUNTIME_EXCEPTION_FREE_CODE ));
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
}


#define vspeedA(S) (( (real_T *)ssGetOutputPortSignal(S, 0))[0])
#define vspeedC(S) (( (real_T *)ssGetOutputPortSignal(S, is_portA(S)))[0])
static void mdlOutputs(SimStruct *S, int_T tid)
{
  UNUSED_PARAMETER(tid);

  if (!is_fast(S)) {
    if (is_portA(S))
      vspeedA(S) = powerA(S)*(directionA(S)==1 ? 1 : -1);
    if (is_portC(S))
      vspeedC(S) = powerC(S)*(directionC(S)==1 ? 1 : -1);
  } else {
    if (is_portA(S))
      vspeedA(S) = powerA(S);
    if (is_portC(S))
      vspeedC(S) = powerC(S);
  }
}



static void mdlTerminate(SimStruct *S)
{
}


#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
  char define[256]="__ERIKA_PWM_USED__";
  uint8_T n;
  boolean_T p1,p2,f,b;
  n = nports(S);
  p1 = is_portA(S);
  p2 = is_portC(S);
  f = is_fast(S);
  b = is_brake(S);

  ssWriteRTWScalarParam(S, "NumberOfMotors", &n, SS_UINT8);
  ssWriteRTWScalarParam(S, "PortAUsed", &p1, SS_BOOLEAN);
  ssWriteRTWScalarParam(S, "PortCUsed", &p2, SS_BOOLEAN);
    
  if (is_brake(S)==1) 
    strcat(define, " __ERIKA_PWM_TOFF_BRAKE__");
  ssWriteRTWScalarParam(S, "BrakeWhenOff", &b, SS_BOOLEAN);

  if (is_fast(S)==1) 
    strcat(define, " __ERIKA_PWM_FAST__");
  ssWriteRTWScalarParam(S, "FastImplementation", &f, SS_BOOLEAN);

  if (!ssWriteRTWStrParam(S,  "ErikaDefine", define))
    return;

} 

boolean_T is_portA(SimStruct *S)
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) != 2);
}

boolean_T is_portC(SimStruct *S)
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) != 1);
}

uint8_T nports(SimStruct *S)
{
  if ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) == 3)
    return 2;
  else 
    return 1;
}

boolean_T is_brake(SimStruct *S)
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,2))) != 2);
}

boolean_T is_fast(SimStruct *S)
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,1))) != 2);
}

boolean_T directionA(SimStruct *S)
{
  return ( (const boolean_T**)ssGetInputPortSignalPtrs(S,1))[0][0];
}

boolean_T directionC(SimStruct *S)
{
  return ( (const boolean_T**)
	   ssGetInputPortSignalPtrs(S,is_portA(S)*2 + 1)
	   )[0][0];
}

uint8_T powerC(SimStruct *S)
{
  return ( (const uint8_T**) 
	   ssGetInputPortSignalPtrs(S,is_portA(S)+!is_fast(S))
	   )[0][0];
}

uint8_T powerA(SimStruct *S)
{
  return ( (const uint8_T**) ssGetInputPortSignalPtrs(S,0))[0][0];
}


#include "simulink.c"  
