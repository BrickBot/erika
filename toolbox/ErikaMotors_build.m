%
% Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
%
% http://erika.sssup.it
%
% Copyright (C) 2004 Riccardo Vestrini
%
% This program is distributed under the license contained into the
% license.txt file on the E.R.I.K.A. root directory.
%
% CVS: $Id: ErikaMotors_build.m,v 1.1 2004/06/01 16:42:37 vestrini Exp $
%

function result = ErikaMotors_build

obj = 'ErikaEducational/PWM Motors';
cr = sprintf('\n');
description = ['zio',cr,'pipn'];
help = 'file:///zio.html';


set_param(obj,'Mask','on');
set_param(obj,'MaskType','motors');
set_param(obj,'MaskPromptString','Ports used:|Fast implementation:|Brake when off:');
set_param(obj,'MaskVariables','ports_used=@ 1;fast_implementation=@ 2;brake_when_off=@ 3');
set_param(obj,'MaskStyleString','popup(A|C|A and C),popup(yes|no),popup(yes|no)');
set_param(obj,'MaskDisplay','image(imread(''ErikaMotors.tif''))');
set_param(obj,'MaskDescription',description);
set_param(obj,'MaskCallBackString','||');
%set_param(obj,'MaskEnables','on,on,on');
set_param(obj,'MaskHelp',help);
set_param(obj,'MaskIconFrame','on');
set_param(obj,'MaskIconOpaque','on');
set_param(obj,'MaskIconRotate','on');
set_param(obj,'MaskIconUnits','Autoscale');
set_param(obj,'MaskInitialization','ErikaMotors_initialize;');
set_param(obj,'MaskSelfModifiable','off');
set_param(obj,'MaskTunableValueString','off,off,off');
%set_param(obj,'MaskVisibilities','on,on,on');


result = 0;

