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
 * CVS: $Id: ErikaLcdDisplay.c,v 1.1 2004/06/01 16:42:35 vestrini Exp $
 */

#define S_FUNCTION_NAME  ErikaLcdDisplay
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#ifndef MATLAB_MEX_FILE
#  error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif

static void mdlInitializeSizes(SimStruct *S)
{
  /* parameters */
  ssSetNumSFcnParams(S, 0);
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
    ssSetErrorStatus(S, "No parameters expected");
    return; 
  }
  
  /* states */
  ssSetNumSampleTimes(S, 1); 
  ssSetNumContStates(S, 0);  
  ssSetNumNonsampledZCs(S, 0);

  /* output ports */
  if (!ssSetNumOutputPorts(S, 1)) return;
  if(!ssSetOutputPortVectorDimension(S, 0, 1)) return;
  ssSetOutputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);

  /* input ports */
  if (!ssSetNumInputPorts(S, 1)) return;
  if(!ssSetInputPortVectorDimension(S, 0, 1)) return;
  ssSetInputPortDataType(S, 0, SS_INT16);
  ssSetInputPortOverWritable(S, 0, 0);
  ssSetInputPortOptimOpts(S, 0, SS_NOT_REUSABLE_AND_GLOBAL);
  ssSetInputPortDirectFeedThrough(S, 0, 1);

  /* Options */
  ssSetOptions(S, (SS_OPTION_RUNTIME_EXCEPTION_FREE_CODE ));
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
}

#define output(S) (( (real_T *)ssGetOutputPortSignal(S, 0))[0])
static void mdlOutputs(SimStruct *S, int_T tid)
{
  real_T n;
  UNUSED_PARAMETER(tid);
  
  n = ( (const uint8_T**) ssGetInputPortSignalPtrs(S,0) )[0][0];
  
  if (n<-9999)
    output(S) = -9999;
  else if (n>9999)
    output(S) = 9999;
  else output(S) = n;
}

static void mdlTerminate(SimStruct *S)
{
}

#include "simulink.c"
