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
% CVS: $Id: ErikaLightSensors_initialize.m,v 1.1 2004/06/01 16:42:36 vestrini Exp $
%

function ret = ErikaLightSensors_initialize

cr = sprintf('\n');


d = [ 'image(imread(''ErikaLightSensors.tif''));', cr ];
      
switch get_param(gcb,'ports_used')
 case '1'
  d = [ d, 'port_label(''input'',1,''value 1'');' , cr, ...
	'port_label(''output'',1,''value 1'');' , cr ];
 case '2'
  d = [ d, 'port_label(''input'',1,''value 2'');' , cr, ...
	'port_label(''output'',1,''value 2'');' , cr ];
 case '3'
  d = [ d, 'port_label(''input'',1,''value 3'');' , cr, ...
	'port_label(''output'',1,''value 3'');' , cr ];
 case '1 and 2'
  d = [ d, 'port_label(''input'',1,''value 1'');' , cr, ...
	'port_label(''output'',1,''value 1'');' , cr , ...
	'port_label(''input'',2,''value 2'');' , cr , ...
	'port_label(''output'',2,''value 2'');' , cr ];
 case '1 and 3'
  d = [ d, 'port_label(''input'',1,''value 1'');' , cr, ...
	'port_label(''output'',1,''value 1'');' , cr , ...
	'port_label(''input'',2,''value 3'');' , cr , ...
	'port_label(''output'',2,''value 3'');' , cr ];
 case '2 and 3'
  d = [ d, 'port_label(''input'',1,''value 2'');' , cr, ...
	'port_label(''output'',1,''value 2'');' , cr , ...
	'port_label(''input'',2,''value 3'');' , cr , ...
	'port_label(''output'',2,''value 3'');' , cr ];
 case '1 2 and 3'
  d = [ d, 'port_label(''input'',1,''value 1'');' , cr, ...
	'port_label(''output'',1,''value 1'');' , cr , ...
	'port_label(''input'',2,''value 2'');' , cr , ...
	'port_label(''output'',2,''value 2'');' , cr , ...
	'port_label(''input'',3,''value 3'');' , cr , ...
	'port_label(''output'',3,''value 3'');' , cr ];
end
  
set_param(gcb,'MaskDisplay', d );
ret = d;
