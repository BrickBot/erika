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
 * CVS: $Id: ErikaLightSensors.c,v 1.1 2004/06/01 16:42:36 vestrini Exp $
 */

#define S_FUNCTION_NAME  ErikaLightSensors
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

/* parameters */
boolean_T port_used(SimStruct *S, int_T n); /* port used by a light sensor */
int8_T ports(SimStruct *S); /* coded port parameter (3 bit in a byte) */
int8_T nports(SimStruct *S); /* number of ports used by sensors */  
boolean_T fast_mode(SimStruct *S); /* conversion mode: 0=normal 1=fast */
int8_T sensitivity(SimStruct *S); /* sensitivity: from 0 (min) to 9 (max) */
/* states */
#define sensor_value(S,n) (( (uint16_T*) ssGetDWork(S,0) )[n]) 
/* signals */
uint16_T input(SimStruct *S, int_T n); /* sensor input values signals */
#define output(S,n) ( ((uint16_T*) ssGetOutputPortSignal(S, n))[0] ) /* sensor output values signals */

#ifndef MATLAB_MEX_FILE
#  error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif


static void mdlCheckParameters(SimStruct *S)
{
  if ( mxIsComplex(ssGetSFcnParam(S,0)) ||
       !mxIsDouble(ssGetSFcnParam(S,0)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,0)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,0)) != 1) ||
       ( ports(S)<1 && ports(S)>7 )
       ) {
    ssSetErrorStatus(S, "First parameter (ports used) has to be a non "
		     "complex double type, with 2 dimensions and one element "
		     "with values 1 (port 1), 2 (port 2), 4 (port 3), "
		     "3 (port 1 and 2), 5 (port 1 and 3), "
		     "6 (port 2 and 3) or 7 (port 1,2 and 3)");
    return;
  }
  if ( mxIsComplex(ssGetSFcnParam(S,1)) ||
       !mxIsDouble(ssGetSFcnParam(S,1)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,1)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,1)) != 1) ) {
    ssSetErrorStatus(S, "Second parameter (conversion mode) has to be a non "
		     "complex double type, with 2 dimensions and one element "
		     "with value 2 (fast mode) or other (normal mode)");
    return;
  }
  if ( mxIsComplex(ssGetSFcnParam(S,2)) ||
       !mxIsDouble(ssGetSFcnParam(S,2)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,2)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,2)) != 1) ||
       (sensitivity(S) < 1 && sensitivity(S)>10) 
       ) {
    ssSetErrorStatus(S, "Third parameter (sensitivity) has to be a non "
		     "complex double type, with 2 dimensions and one element"
		     "with values 1, 2, 3, 4, 5, 6, 7, 8, 9 or 10");
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
		     "ports, conversion mode and sensitivity");
    return; 
  }

  /* Sample time */
  ssSetNumSampleTimes(S, 1);
 
  /* state */
  ssSetNumDWork(S,1); 
  ssSetDWorkDataType(S,0,SS_UINT16);
  ssSetDWorkWidth(S,0,nports(S));
  ssSetDWorkName(S,0,"sensor");

  /* Output and input ports */
  if (!ssSetNumOutputPorts(S, nports(S))) return;
  if (!ssSetNumInputPorts(S, nports(S)) ) return;
  for (i=0; i<nports(S); i++) {
    /* output port */
    if(!ssSetOutputPortVectorDimension(S, i, 1)) return;
    ssSetOutputPortOptimOpts(S, i, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortDataType(S, i, SS_UINT16);
    /* input port */
    if(!ssSetInputPortVectorDimension(S, i, 1)) return;
    ssSetInputPortOverWritable(S, i, 0);
    ssSetInputPortOptimOpts(S, i, SS_NOT_REUSABLE_AND_GLOBAL);
    /* ssSetInputPortDirectFeedThrough(S, i, 1); */
  }

  /* Options */
  ssSetOptions(S, (SS_OPTION_RUNTIME_EXCEPTION_FREE_CODE ));
}


static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
}


static void mdlInitializeConditions(SimStruct *S)
{
  int i;
  for (i =0; i< nports(S); i++)
    sensor_value(S,i) = 0;
}


static void mdlOutputs(SimStruct *S, int_T tid)
{
  int i;
  UNUSED_PARAMETER(tid);

  for (i=0; i<nports(S); i++)
    output(S,i) = sensor_value(S,i); 
}


#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
  int i;
  UNUSED_PARAMETER(tid);

  for (i=0; i<nports(S); i++)
    sensor_value(S,i) = input(S,i); 
}


static void mdlTerminate(SimStruct *S)
{
}


#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
  boolean_T p[3], fs;
  int8_T nport, s;

  p[0] = port_used(S,1);
  p[1] = port_used(S,2);
  p[2] = port_used(S,3);
  ssWriteRTWVectParam(S, "ErikaInputPorts", p, SS_BOOLEAN, 3);

  if (!ssWriteRTWStrParam(S,  "ErikaDefine", "__AD_SUPPORT__"))
    return;

  ssWriteRTWStrParam(S,  "ErikaInterrupt", "LightSensors");

  ssWriteRTWStrParam(S, "InterruptHandler", "lightsensors_handler"); 
  ssWriteRTWStrParam(S, "IRQ", "ADI_IRQ");

  fs = fast_mode(S);
  ssWriteRTWScalarParam(S, "FastMode", &fs, SS_BOOLEAN);

  nport=nports(S);
  ssWriteRTWScalarParam(S, "NumberOfUsedPorts", &nport, SS_INT8);

  s=sensitivity(S);
  ssWriteRTWScalarParam(S, "Sensitivity", &s, SS_INT8);
} 


boolean_T port_used(SimStruct *S, int_T n) 
{
  switch (ports(S)) {
  case 1: if (n==1) return 1; else return 0;
  case 2: if (n==2) return 1; else return 0;
  case 3: if (n==3) return 1; else return 0;
  case 4: if (n!=3) return 1; else return 0;
  case 5: if (n!=2) return 1; else return 0;
  case 6: if (n!=1) return 1; else return 0;
  case 7: return 1;
  }
  return 0;
}

int8_T nports(SimStruct *S)
{
  switch (ports(S)) {
  case 1: case 2: case 3: return 1;
  case 4: case 5: case 6: return 2;
  case 7: return 3;
  }
  return 1;
}

boolean_T fast_mode(SimStruct *S) 
{
  return *((real_T*) mxGetPr(ssGetSFcnParam(S,1))) == 2;
}

int8_T sensitivity(SimStruct *S) 
{
  return *((real_T*) mxGetPr(ssGetSFcnParam(S,2))) - 1;
}

uint16_T input(SimStruct *S, int_T n) 
{
  real_T i;
  i = (*(((const real_T**)ssGetInputPortSignalPtrs(S, n))[0]));
  if (i<0) return 0; 
  if (i>1024) return 1024;
  return i;
}

int8_T ports(SimStruct *S)
{
  return *((real_T*) mxGetPr(ssGetSFcnParam(S,0)));
}


#include "simulink.c" 

