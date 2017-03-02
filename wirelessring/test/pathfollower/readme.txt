; Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
;
; http://erika.sssup.it
;
; File: Readme.txt
;
; Copyright (C) 2004 Antonio Mancina mancina@gandalf.sssup.it
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This file is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Directory Contents: 

erika/ it contains the files to upload into the RCX flash

shark/ it contains the application to run with Shark on the PC to
which the IR tower is connected

Demo description: 

This demo is for three stations, two RCXs and a PC. The PC will be
connected to the tower in front of which there will be a wireless
Gateway: the PC will execute the S.Ha.R.K. application in the shark
directory. Using the App Interface you will gain control over the two
RCX: the repeater will forward the commands (slow, fast, stop...) sent
by the PC with the Send Command Button to the RCX Path Follower. The
PC may require to both RCXs the reading of the rotating speed of the
motors or their difference (the Repeater will return always the same
value).  The periods of the answers may be set through the interface.

Please note that the protocol is affected by some bug that prevents it
to work in particular stressed conditions of traffic for more than two
or three minutes. Debugging work is in progress.

In order to use the demo, you must follow the next steps.


SHARK

1) Install shark 1.22 (you can download it from the web page
   shark.sssup.it).  Suppose you install it on shark_base.

2) Copy the shark demo directory on shark_base/demos directory (create a
   directory for this).

3) Copy the content of the shark_patch_1.22 on shark_base/drivers/oldchar
   directory.

4) Enter the directory ~/shark/drivers/oldchar and compile the
   libraries, using make.

5) Enter the directory ~/shark/demos/wireless_monitor and compile the
   demo, using make.

6) Let the demo run on a pc with the infrared tower on a serial
   port. The wireless gateway must be in front of the tower. Note that
   the wireless gateway should be powered on BEFORE the demo starts,
   because in the initial phase the demo sends a configuration message
   to the wireless gateway.


ERIKA

1) Compile the demos and load them on two RCX. One of them, the path
   follower, must be on the path, while the other one can be every
   where you want.

2) Configure the wireless gateway pressing PRGM button. Through this,
   a configuration message is sent to the wireless gateway.

3) Initialize the sensors of the two cars, pressing the RUN button 4 times.
   1) Set BLACK (Front Side sensor on black part of the path)
   2) Set REFERENCE (Front Side sensor on reference part of the path)
   3) Set WHITE (Front Side sensor on white part of the path)
   4) Set Offset between sensors (Both the sensors on the same color)
  

The demo is now ready to start.


BUGS AND PROBLEMS

The demo is not so robust. Two main causes of problems are
communication and motor settings. If the communication seems to be
absent, you can try setting the IR to ShortRange, or you can try to
avoid them to interfere, positioning them as far as possible (some
meters should be enough). If the communication seems to work but the
pathfollower can't follow the path, the problem is probably in the
parameters of the motor. Maybe the problem is in the sensor
settings. In this case you can try to restart all and reset the
sensors. If the problem is not solved, you must work on the parameters
of the control algorithm. In this case...well...good luck.

Sometimes the shark monitor suddenly stops to work. I don't know why,
if you can find something please let me know.

When the car are sending periodic messages, expecially with short
periods, it becomes difficult to send new messagges from the
monitor. You can only retry to send them.



FINAL NOTES

The demo is only a preliminary version so it has bugs which must be
solved.

More over it as three main problems:

1) The repeater should follow the path together with the path
   follower. The initial idea was to have two path followers, but we
   had a lot of problems in setting up parameters of the second
   car. So for the moment the second car is only a repeater.

2) The upper library (the Interaction Layer) is not so good, it needs
   to be rewritten in a lot parts. We will do it as soon as possible,
   for the moment this version is working, so you can use this.

3) The demo was written for the old version (1.22) of Shark. At the
   end of our work, a newer version (1.4) of Shark was released, and
   the demo cannot work with it. We will work on this as soon as
   possible.


You can find material on the demos on
erika.sssup.it/wireless/wireless.html, or reading Antonio Mancina's
and Michele Cirinei's Master Thesis (in the download section of the
ERIKA site).
