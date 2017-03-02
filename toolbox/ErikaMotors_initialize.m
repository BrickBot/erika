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
% CVS: $Id: ErikaMotors_initialize.m,v 1.1 2004/06/01 16:42:37 vestrini Exp $
%

function ret = ErikaMotors_initialize

cr = sprintf('\n');


d = [ 'image(imread(''ErikaMotors.tif''));', cr ];
      
if (~strcmp(get_param(gcb,'ports_used'),'C'))
  d = [ d, 'port_label(''input'',1,''power A'');' , cr, ...
	'port_label(''output'',1,''speed A'');' , cr ];
  if (strcmp(get_param(gcb,'fast_implementation'),'no'))
    d = [ d, 'port_label(''input'',2,''direction A'');' , cr];
    if (strcmp(get_param(gcb,'ports_used'),'A and C'))
      d = [ d, 'port_label(''input'',3,''power C'');' , cr, ...
	    'port_label(''input'',4,''direction C'');' , cr, ...
	    'port_label(''output'',2,''speed C'');' , cr ];
    end
  elseif (strcmp(get_param(gcb,'ports_used'),'A and C'))
    d = [ d, 'port_label(''input'',2,''power C'');' , cr, ...
	  'port_label(''output'',2,''speed C'');' , cr ];
  end
else
  d = [ d, 'port_label(''input'',1,''power C'');' , cr, ...
	'port_label(''output'',1,''speed C'');' , cr ];
  if (strcmp(get_param(gcb,'fast_implementation'),'no'))
    d = [ d, 'port_label(''input'',2,''direction C'');' , cr];
  end
end

set_param(gcb,'MaskDisplay', d );
ret = d;
