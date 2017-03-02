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
%

function ErikaEducational_make_rtw_hook(varargin)
% ErikaEducational target-specific hook file for the build process (make_rtw).

persistent MODEL_SETTINGS

Action    = varargin{1};
modelName = varargin{2};
buildArgs = varargin{6};

switch Action
case 'entry'
  % nothing...
  
case 'exit'
  disp(['### Successful completion of build ',...
	'procedure for model: ', modelName]);
  
end


