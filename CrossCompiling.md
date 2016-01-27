# Introduction #

Cross compiling is a way to compile code for the BeagleBone from a host computer.  This allows you to target the ARM architecture with your standard x86 machine.


# How To #

First you need to download and install your toolchain and target system source code.

After this, you must setup your compilation environment.  This is done by setting several environment variables.

At the least, you should setup the CC or CXX variables for the compiler, and then export the library directories.

Ex.
```
$ export PATH=$PATH:/path/to/toolchain/bin
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/toochain/lib
```

Now compile examples.
```
$ arm-angstrom-linux-gnueabi-gcc hello_world.c
$ arm-angstrom-linux-gnueabi-g++ hello_world.cc
```

You can export the CC or CXX variables to make it easier if you like.


## Using the Source File ##

In the scripts directory of the source code, there is a file called _beagle-source-me.txt_.  This file is an example of how to easily setup your cross compiling environment.

After verifying that all the paths are correct, simply run the command:
```
$ source beagle-source-me.txt
```

Now everything is good to go...to compile do something like:
```
$ CC hello_world.c
$ CXX hello_world.cc
```