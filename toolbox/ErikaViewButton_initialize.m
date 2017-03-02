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
% CVS: $Id: ErikaViewButton_initialize.m,v 1.1 2004/06/01 16:42:43 vestrini Exp $
%

function ret = ErikaViewButton_initialize

cr = sprintf('\n');

d = [ 'image(imread(''ErikaViewButton.tif''));', cr , ...
      'port_label(''output'',1,''', ...
      get_param(gcb,'output_mode'), ...
      ''');', cr ];

set_param(gcb,'MaskDisplay', d );
ret = d;
