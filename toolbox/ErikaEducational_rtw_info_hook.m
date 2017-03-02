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

function varargout = ErikaEducational_rtw_info_hook(varargin)
% Target specific hook file for
% providing RTW the necessary information regarding this
% target.

% Copyright 1994-2002 The MathWorks, Inc.
% $Revision: 1.1 $ $Date: 2004/06/01 16:42:35 $

Action    = varargin{1};
modelName = varargin{2};

switch Action
 case 'wordlengths'
  
%  varargout{1} = rtwhostwordlengths(modelName);
varargout{1} = struct('CharNumBits',8,'ShortNumBits',16,'IntNumBits',16,'LongNumBits',32)
  
 case 'cImplementation'
  
varargout{1} = struct('ShiftRightIntArith',1,'Float2IntSaturates',0,'IntPlusIntSaturates',0,'IntTimesIntSaturates',0)
  
 otherwise
  % Properly accommodate future releases of Real-Time Workshop
  varargout = [];
  
end


