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
 * CVS: $Id: ErikaSuspend.c,v 1.1 2004/06/01 16:42:41 vestrini Exp $
 */

#define S_FUNCTION_NAME  ErikaSuspend
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include <string.h>

static void mdlInitializeSizes(SimStruct *S)
{
  ssSetNumSampleTimes(S, 1);
  ssSetNumContStates(S, 0);
  ssSetNumNonsampledZCs(S, 0);
  ssSetNumDiscStates(S,1); 

  /* no output ports */
  if (!ssSetNumOutputPorts(S, 0)) return;

  /* two input ports */
  if (!ssSetNumInputPorts(S, 2)) return;

  /* inport number: 0 */
  if(!ssSetInputPortVectorDimension(S, 0, 1)) return;
  ssSetInputPortDataType(S, 0, SS_BOOLEAN);
  ssSetInputPortOverWritable(S, 0, 0);
  ssSetInputPortOptimOpts(S, 0, SS_NOT_REUSABLE_AND_GLOBAL);

  if(!ssSetInputPortVectorDimension(S, 1, 1)) return;
  ssSetInputPortDataType(S, 1, SS_BOOLEAN);
  ssSetInputPortOverWritable(S, 1, 0);
  ssSetInputPortOptimOpts(S, 1, SS_NOT_REUSABLE_AND_GLOBAL);

  ssSetNumSFcnParams(S, 0);

#if defined(MATLAB_MEX_FILE)
  if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
    if( ssGetErrorStatus(S) != NULL ) return;
  } else return;  
#endif       

  /* Options */
  ssSetOptions(S, (SS_OPTION_RUNTIME_EXCEPTION_FREE_CODE ));
}

/* Function to initialize sample times */
static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
  UNUSED_PARAMETER(tid);
}

#define MDL_START
static void mdlStart(SimStruct *S)
{
  int i;

  for (i =0; i< ssGetNumDiscStates(S); i++)
    ssGetDiscStates(S)[i]=0;
}

#define MDL_UPDATE
#define SUSPEND (*(((const boolean_T**)ssGetInputPortSignalPtrs(S, 0))[0]))
#define RESUME (*(((const boolean_T**)ssGetInputPortSignalPtrs(S, 1))[0]))
static void mdlUpdate(SimStruct *S, int_T tid)
{
  UNUSED_PARAMETER(tid);

  if (SUSPEND && !ssGetDiscStates(S)[0]) {
    /* this should stop simulation when input = 1, 
     but I don't know how to do it*/
    ssGetDiscStates(S)[0]=1;
    return;
  }
  if (RESUME && ssGetDiscStates(S)[0]) {
    /* this should resume simulation */
    ssGetDiscStates(S)[0]=0;
    return;
  }
}


static void mdlTerminate(SimStruct *S)
{
}

#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif

