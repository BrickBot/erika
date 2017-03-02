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
% CVS: $Id: ErikaViewButton_build.m,v 1.1 2004/06/01 16:42:43 vestrini Exp $
%

function result = ErikaViewButton_build

obj = 'ErikaEducational/View button';
cr = sprintf('\n');
description = ['zio',cr,'pipn'];
help = 'file:///zio.html';


set_param(obj,'Mask','on');
set_param(obj,'MaskType','button');
set_param(obj,'MaskPromptString','Output Mode:');
set_param(obj,'MaskVariables','output_mode=@ 1');
set_param(obj,'MaskStyleString','popup(edge|trigger|pass)');
set_param(obj,'MaskDisplay','image(imread(''ErikaViewButton.tif''))');
set_param(obj,'MaskDescription',description);
set_param(obj,'MaskCallBackString',';');
%set_param(obj,'MaskEnables','on');
set_param(obj,'MaskHelp',help);
set_param(obj,'MaskIconFrame','on');
set_param(obj,'MaskIconOpaque','on');
set_param(obj,'MaskIconRotate','on');
set_param(obj,'MaskIconUnits','Autoscale');
set_param(obj,'MaskInitialization','ErikaViewButton_initialize;');
set_param(obj,'MaskSelfModifiable','off');
set_param(obj,'MaskTunableValueString','off');
%set_param(obj,'MaskVisibilities','on');


result = 0;

