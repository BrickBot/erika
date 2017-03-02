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
 * CVS: $Id: ErikaViewButton.c,v 1.1 2004/06/01 16:42:42 vestrini Exp $
 */

#define S_FUNCTION_NAME  ErikaViewButton
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"


/* returns parameters */
boolean_T output_mode_trigger(SimStruct *S); /* single peak when input passes from 0 to 1 */
boolean_T output_mode_edge(SimStruct *S); /* changes (0 or 1) when input passes from 0 to 1 */
boolean_T output_mode_pass(SimStruct *S); /* output follows input */
/* returns block input */
boolean_T input(SimStruct *S);
/* block output */
#define output(S) ( ( (boolean_T*) ssGetOutputPortSignal(S, 0) )[0] )
/* states */
#define current_output(S) ( ( (boolean_T*) ssGetDWork(S,0) )[0] )
#define previous_input(S) ( ( (boolean_T*) ssGetDWork(S,1) )[0] )

#ifndef MATLAB_MEX_FILE
#  error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif

static void mdlCheckParameters(SimStruct *S)
{
  if ( mxIsComplex(ssGetSFcnParam(S,0)) ||
       !mxIsDouble(ssGetSFcnParam(S,0)) ||
       (mxGetNumberOfDimensions(ssGetSFcnParam(S,0)) != 2) ||
       (mxGetNumberOfElements(ssGetSFcnParam(S,0)) != 1) ||
       ( !output_mode_trigger(S) &&
	 !output_mode_pass(S) &&
	 !output_mode_edge(S) ) 
       ) {
    ssSetErrorStatus(S, "First parameter (output mode) has to be a non "
		     "complex double type, with 2 dimensions and one element"
		     "with value 0 (trigger), 1 (edge) or 2 (pass)");
    return;
  }
}


static void mdlInitializeSizes(SimStruct *S)
{
  /* parameters */
  ssSetNumSFcnParams(S, 1);
  ssSetSFcnParamTunable(S, 0, 0);
  if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
    mdlCheckParameters(S);
    if( ssGetErrorStatus(S) != NULL ) return;
  } 
  else {
    ssSetErrorStatus(S, "1 Parameters expected: "
		     "output mode");
    return; 
  }

  /* sample times */
  ssSetNumSampleTimes(S, 1);  

  /* states */
  if (output_mode_pass(S)) 
    ssSetNumDWork(S,1); /* no need to store previous input */
  else {
    ssSetNumDWork(S,2);
    ssSetDWorkDataType(S,1,SS_BOOLEAN);
    ssSetDWorkWidth(S,1,1);
    ssSetDWorkName(S,1,"previous_input");
  }
  ssSetDWorkDataType(S,0,SS_BOOLEAN);
  ssSetDWorkWidth(S,0,1);
  ssSetDWorkName(S,0,"current_output");

  /* output ports */
  if (!ssSetNumOutputPorts(S, 1)) return;
  if(!ssSetOutputPortVectorDimension(S, 0, 1)) return;
  ssSetOutputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
  ssSetOutputPortDataType(S, 0, SS_BOOLEAN);

  /* input ports */
  if (!ssSetNumInputPorts(S, 1)) return;
  if(!ssSetInputPortVectorDimension(S, 0, 1)) return;
  ssSetInputPortDataType(S, 0, SS_BOOLEAN);
  ssSetInputPortDirectFeedThrough(S, 0, 0);
  ssSetInputPortOverWritable(S, 0, 0);
  ssSetInputPortOptimOpts(S, 0, SS_NOT_REUSABLE_AND_GLOBAL);
 
  /* options */
  ssSetOptions(S, (SS_OPTION_RUNTIME_EXCEPTION_FREE_CODE ));
}


static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
  UNUSED_PARAMETER(tid);
  output(S) = current_output(S);
}

#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
  UNUSED_PARAMETER(tid);
  /* trigger mode */
  if (output_mode_trigger(S)) {
    if (!current_output(S) && input(S) && !previous_input(S) )
      current_output(S) = 1;
    else
      current_output(S) = 0;
    previous_input(S) = input(S);
    return;
  }
  /* edge mode */
  if (output_mode_edge(S)) {
    if (input(S) && !previous_input(S) )
      current_output(S) = !current_output(S);
    previous_input(S) = input(S);
    return;
  }
  /* pass mode */
  if (output_mode_pass(S)) {
    current_output(S) = input(S);
    return;
  }
}


static void mdlTerminate(SimStruct *S)
{
} 

#define MDL_START
static void mdlStart(SimStruct *S)
{
  current_output(S) = 0;
  if (!output_mode_pass(S))
    previous_input(S) = 0;
} 


#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
  boolean_T op, ot, oe;
  op = output_mode_pass(S);
  ot = output_mode_trigger(S);
  oe = output_mode_edge(S);

  ssWriteRTWScalarParam(S, "OutputModePass", &op, SS_BOOLEAN);
  ssWriteRTWScalarParam(S, "OutputModeTrigger", &ot, SS_BOOLEAN);
  ssWriteRTWScalarParam(S, "OutputModeEdge", &oe, SS_BOOLEAN);
} 



boolean_T output_mode_trigger(SimStruct *S) 
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) == 0);
}

boolean_T output_mode_edge(SimStruct *S) 
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) == 1);
}

boolean_T output_mode_pass(SimStruct *S) 
{
  return ( *((real_T*) mxGetPr(ssGetSFcnParam(S,0))) == 2);
}

boolean_T input(SimStruct *S)
{
  return *(((const boolean_T**)ssGetInputPortSignalPtrs(S, 0))[0]);
}



#include "simulink.c"   
