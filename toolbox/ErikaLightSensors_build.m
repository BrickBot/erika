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
% CVS: $Id: ErikaLightSensors_build.m,v 1.1 2004/06/01 16:42:36 vestrini Exp $
%

function result = ErikaLightSensors_build

obj = 'ErikaEducational/Light Sensors';
cr = sprintf('\n');
description = ['zio',cr,'pipn'];
help = 'file:///zio.html';


set_param(obj,'Mask','on');
set_param(obj,'MaskType','button');
set_param(obj,'MaskPromptString','Ports used:|Fast mode:|Sensitivity:');
set_param(obj,'MaskVariables','ports_used=@ 1;conversion_mode=@ 2;sensitivity=@ 3');
set_param(obj,'MaskStyleString','popup(1|2|3|1 and 2|1 and 3|2 and 3|1 2 and 3),popup(normal|fast),popup(min|1|2|3|4|5|6|7|8|max)');
set_param(obj,'MaskDisplay','image(imread(''ErikaLightSensors.tif''))');
set_param(obj,'MaskDescription',description);
set_param(obj,'MaskCallBackString','||');
%set_param(obj,'MaskEnables','on,on,on');
set_param(obj,'MaskHelp',help);
set_param(obj,'MaskIconFrame','on');
set_param(obj,'MaskIconOpaque','on');
set_param(obj,'MaskIconRotate','on');
set_param(obj,'MaskIconUnits','Autoscale');
set_param(obj,'MaskInitialization','ErikaLightSensors_initialize;');
set_param(obj,'MaskSelfModifiable','off');
set_param(obj,'MaskTunableValueString','off,off,off');
%set_param(obj,'MaskVisibilities','on,on,on');


result = 0;

