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
% CVS: $Id: ErikaOnOffButton_initialize.m,v 1.1 2004/06/01 16:42:38 vestrini Exp $
%

function ret = ErikaOnOffButton_initialize

cr = sprintf('\n');

d = [ 'image(imread(''ErikaOnOffButton.tif''));', cr , ...
      'port_label(''output'',1,''', ...
      get_param(gcb,'output_mode'), ...
      ''');', cr ];
if (strcmp(get_param(gcb,'use_interrupt'),'yes'))
  d = [ d, 'port_label(''input'',1,''int'')' , cr];
end

set_param(gcb,'MaskDisplay', d );
ret = d;
