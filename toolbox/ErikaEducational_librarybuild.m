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
% CVS: $Id: ErikaEducational_librarybuild.m,v 1.1 2004/06/01 16:42:35 vestrini Exp $
%

function ret = ErikaEducational_librarybuild

a = ErikaOnOffButton_build;
a = a + ErikaRunButton_build;
a = a + ErikaViewButton_build;
a = a + ErikaPrgmButton_build;
a = a + ErikaSuspend_build;
a = a + ErikaTerminate_build;
a = a + ErikaLcdDisplay_build;
a = a + ErikaMotors_build;
a = a + ErikaLightSensors_build;
ret = a;
