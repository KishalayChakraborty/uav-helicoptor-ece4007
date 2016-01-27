# Introduction #

The timeout library is used to schedule timer interrupts.  Registering a timeout will result in the execution of a callback function after the specified time has elapsed.


# Details #
To use the library you only need to register a timeout by calling the _register\_timeout()_ function.  To do this, you must first declare your callback function that will be called on the interrupt.

**Example of a callback function**
```
void my_callback() {
    cout << "Callback Triggered" << endl;
}
```

Then you simply make a call to the _register\_timeout()_ function, which takes 2 arguments

**Example of registering a timeout**
```
...
   //Schedule my_callback to be run in 500ms
   register_timeout(my_callback,0.5);
...
```

**Example of registering a timeout to re-register itself...effectively crating an interval timer**
```
void my_callback() {
    cout << "Callback Triggered" << endl;
    //Reschedule my_callback to be run in 500ms
    register_timeout(my_callback,0.5);
}
```

# Full Examples #

**test\_timeout.cc**
```
/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/
#include <iostream>
#include "../events/timeout.h"

using namespace std;

bool go = true;

void funcA() {
	cout << "funcA" << endl;
}

void funcB() {
	cout << "funcB" << endl;
	register_timeout(funcB,0.1);
}

void funcC() {
	cout << "funcC" << endl;
	go = false;
}

int main() {

	register_timeout(funcA,0.5);
	register_timeout(funcB,0.1);
	register_timeout(funcC,1.0);

	while(go) {;}

	return 0;
}
```

The output should be:
```
funcB
funcB
funcB
funcB
funcB
funcA
funcB
funcB
funcB
funcB
funcB
funcC
```
Where funcB is printed every 100ms, funcA is printed after 500ms, and funcC is printed after 1s, and closes the program.  Notice, function funcB is the only one that gets resheduled after execution