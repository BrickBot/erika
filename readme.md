This package contains E.R.I.K.A. Educational, an Embedded Real tIme Kernel Architecture.

The current directory structure contains:
- HAL Layers:
  - H8 (Lego Mindstorms)
- Kernel Layers
  - Fixed priority with preemption thresholds
- Some little tests
- Matlab toolbox for H8

To compile the Kernels use the makefiles in the test directories. There
are no makefiles in the source directories (!).

To compile, you need the gcc compiler recompiled for H8.

You can use gcc-3 to compile Erika, add \_\_GCC3__ option, this enables newlib
support, if you do not want newlib use \_\_NO_NEWLIB__ option

To use Matlab toolbox you need to change /home/riccardo/erika to your
E.R.I.K.A. directory in erika.m and in every file found in toolbox dir,
then open Matlab, select E.R.I.K.A directory and type "erika"

Here's the content of the Erika root directory:
```
- config	GNU makefile configuration files
- etc		Configuration file for Ertec C167 board
- include	C headers and ASM macro definition
- src		Source code
- tests		Examples
- toolbox	Matlab toolbox for H8
```

If you have any questions, bug reports or comments please write to
erika-help@gandalf.sssup.it

Bye

* Paolo Gai             (pj@sssup.it)
* Davide Cantini        (cantini@sssup.it)

==============

This project was originally available via the following website (now offline):
 - http://erika.sssup.it/
