# Introduction #
This section straight from [WikiPedia](http://en.wikipedia.org/wiki/PID_controller):

A proportional–integral–derivative controller (PID controller) is a generic control loop feedback mechanism (controller) widely used in industrial control systems – a PID is the most commonly used feedback controller. A PID controller calculates an "error" value as the difference between a measured process variable and a desired setpoint. The controller attempts to minimize the error by adjusting the process control inputs.

![http://upload.wikimedia.org/wikipedia/commons/thumb/4/43/PID_en.svg/300px-PID_en.svg.png](http://upload.wikimedia.org/wikipedia/commons/thumb/4/43/PID_en.svg/300px-PID_en.svg.png)

The PID controller calculation (algorithm) involves three separate constant parameters, and is accordingly sometimes called three-term control: the proportional, the integral and derivative values, denoted P, I, and D. Heuristically, these values can be interpreted in terms of time: P depends on the present error, I on the accumulation of past errors, and D is a prediction of future errors, based on current rate of change. The weighted sum of these three actions is used to adjust the process via a control element such as the position of a control valve, or the power supplied to a heating element.

Some applications may require using only one or two actions to provide the appropriate system control. This is achieved by setting the other parameters to zero. A PID controller will be called a PI, PD, P or I controller in the absence of the respective control actions. PI controllers are fairly common, since derivative action is sensitive to measurement noise, whereas the absence of an integral term may prevent the system from reaching its target value due to the control action.


# Algorithm #

The PID Control Algorithm is defined as the sum of 3 three correcting terms, which forms the output value.  Each term works on the error which is given by the difference between the nominal and the actual values.  The first term is the proportional term and is given by the P constant multiplied by the error.  The second term is the integral term and is given by the I constant multiplied by the integral of the error.  Finally, the derivative term is given by the D constant multiplied by the derivative of the error.

![http://upload.wikimedia.org/wikipedia/en/math/6/5/a/65a3f316105fae298911606534d8182b.png](http://upload.wikimedia.org/wikipedia/en/math/6/5/a/65a3f316105fae298911606534d8182b.png)

where:
  * Kp is the proportional gain
  * Ki is the integral gain
  * Kd is the derivative gain
  * e is the error (nominal - actual)
  * t is time


## Proportional Term ##

The proportional term makes a change to the output that is proportional to the error.  It is given by:
![http://upload.wikimedia.org/wikipedia/en/math/3/f/8/3f80d26e2e621e0f123bb26e80e609e5.png](http://upload.wikimedia.org/wikipedia/en/math/3/f/8/3f80d26e2e621e0f123bb26e80e609e5.png)

A high proportional gain results in high change to the output.  If the gain is too high, then the system can become unstable.  If the gain is too low then the control action may be too small when responding to system disturbances.

![http://upload.wikimedia.org/wikipedia/commons/thumb/2/2b/Change_with_Kp.png/320px-Change_with_Kp.png](http://upload.wikimedia.org/wikipedia/commons/thumb/2/2b/Change_with_Kp.png/320px-Change_with_Kp.png)

### Droop ###

A pure proportional controller will not always settle at its target value.  In fact, it will often undershoot the nominal value and sit below the setpoint.  This is called droop.

Droop is proportional to the process gain and inversely proportional to the proportional gain.  Specifically the steady-state gain is given by:

![http://rogercortesi.com/eqn/tempimagedir/eqn1107.png](http://rogercortesi.com/eqn/tempimagedir/eqn1107.png)

Droop is an inherent defect in using purely proportional control systems and can be overcome by adjusting the setpoint to be above the nominal value, or more reliably, correcting the error by using the integral term.

## Integral Term ##

The integral term contribution is proportional to the magnitude of the error and also the duration of the error.  The integral is expressed as the sum of the instantaneous error over time and gives the accumulated offset that should have been previously corrected.  The integral term is given by the integral constant multiplied by the integral of error over time:

![http://upload.wikimedia.org/wikipedia/en/math/d/5/9/d593c27abdc1aecffb56d06d2a9ba8e3.png](http://upload.wikimedia.org/wikipedia/en/math/d/5/9/d593c27abdc1aecffb56d06d2a9ba8e3.png)

The Integral term accelerates the movement of the process towards the nominal value and eliminates the steady-state error that occurs from the Proportional term.  However, since the integral term responds to accumulated error, there is a possibility of overshooting the setpoint.  If overshoot occurs, then the system will try to correct itself.  If the system is not properly tuned, this can lead the the system to "seek" back and forth across the setpoint.

![http://upload.wikimedia.org/wikipedia/commons/thumb/c/c0/Change_with_Ki.png/320px-Change_with_Ki.png](http://upload.wikimedia.org/wikipedia/commons/thumb/c/c0/Change_with_Ki.png/320px-Change_with_Ki.png)

## Derivative Term ##

The derivative of the process error is calculated by determining the slope of the error over time and multiplying that by the derivative gain constant.  It is given by:

![http://upload.wikimedia.org/wikipedia/en/math/0/6/9/0691207bf621049b3e8bb98627a50f27.png](http://upload.wikimedia.org/wikipedia/en/math/0/6/9/0691207bf621049b3e8bb98627a50f27.png)

The derivative term slows the rate of change of the output.  Generally, the derivative term is used to reduce the magnitude of the overshoot produced by the integral controller and to increase the combined controller stability.  The derivative term amplifies the error due to noise, thus making the output highly sensitive to noise in the error term.

![http://upload.wikimedia.org/wikipedia/commons/thumb/c/c7/Change_with_Kd.png/320px-Change_with_Kd.png](http://upload.wikimedia.org/wikipedia/commons/thumb/c/c7/Change_with_Kd.png/320px-Change_with_Kd.png)


# Loop Tuning #
Tuning a PID controller is the process of adjusting the gain parameters to achieve a desired, stable system response.  Stability is a required attribute of all systems, but many systems differ in their need to reach their setpoint.  Some need to reach the setpoint as fast as possible, others need to gradually work their way to the setpoint.  Tuning the control loop allows you to achieve both the stability and desired system response.

Even though there are only three parameters to use, PID tuning can be a difficult process.  There are several traditional methods to tuning a loop discussed below.

## Stability ##
If the controller parameters are not chosen correctly, the system will be not stable (i.e. its output diverges, with or without oscillation, and is limited only by saturation or mechanical breakage).  Instability can occur by excessive gain, particularly in presence of lag.

Stability of response is defined by the process not oscillating for any combination of conditions and setpoints, though sometimes marginal stability (bounded oscillation) is acceptable.


## Behavior ##

Since desired behavior changes from application to application, the best behavior is describe by regulation and control tracking.  Regulation is the ability of the system to stay at a given setpoint.  Control tracking is the ability to change a setpoint and have the system respond to it.  Rise time and settling time describe the criteria for control tracking.

## Overview of Methods ##
There are several methods in tuning the control loop.  The most effective methods generally require a definition of process models, then choosing the loop parameters based on dynamic model parameters.

The choice of method depends mostly on if the control loop can be taken offline or not and the response time of the system.

| **Methods** | **Advantages** | **Disadvantages** |
|:------------|:---------------|:------------------|
| Manual Tuning | No Math Required. Online Method. | Requires Experience |
| Ziegler-Nichols | Proven Method. Online Method. | Process Upset, some trial and error, very aggressive |
| Software Tools | Consistent tuning. Online or offline method. May include valve and sensor analysis. Allow simulation before downloading. Can support Non-Steady State (NSS) Tuning. | Some cost and training involved |
| Cohen-Coon  | Good Process Model | Some math. Offline method. Only good for first-order processes. |


### Manual Tuning ###
If the system needs to stay online, then manual tuning is an option.  To do this, you generally will set the I and D terms to 0, and increase the P value until the system begins to oscillate.  The P value should then be decreased to half of that value.  I should then be increased until any offset is corrected in reasonable time.  Too much I will make the system unstable, however.  At this point, D can be added if needed to stabilize the system.

**Effects of Increasing Parameters Independently**
| **Parameter** | **Rise Time** | **Overshoot** | **Settling Time** | **Steady-State Error** | **Stability** |
|:--------------|:--------------|:--------------|:------------------|:-----------------------|:--------------|
| Kp            | Decrease      | Increase      | Small Change      | Decrease               | Degrade       |
| Ki            | Decrease      | Increase      | Increase          | Decrease Significantly | Degrade       |
| Kd            | Minor Decrease | Minor Decrease | Minor Decrease    | No Effect in Theory    | Improve if Kd is small |


### Ziegelr-Nichols Method ###
In this method the I and D values are again set to 0.  The P value is increased until it reaches the ultimate gain, Ku.  The loop will the begin to oscillate.  Ku and the oscillation period Pu, are used to calculate the gain values as shown below.

| **Control-Type** | **Kp** | **Ki** | **Kd** |
|:-----------------|:-------|:-------|:-------|
| **P**            | 0.50Ku | -      | -      |
| **PI**           | 0.45Ku | 1.2Kp/Pu | -      |
| **PID**          | 0.60Ku | 2Kp/Pu | KpPu/8 |


# Library Tutorial #
The PID Library allows for the use of PID Control Algorithm.  The library is relatively simple requiring only the use of two methods; the init method and the regulate method.

The init method is used to initialize the PID Loop with the appropriate loop parameters.  Its input arguments are the P constant, I constant, D constant, the sample time, and the maximum output that should be allowed.  The output is an error code.

| **Error Code** | **Problem** |
|:---------------|:------------|
| 0              | No Error    |
| -1             | Overflow Error |
| -2             | Probably an Unstable Configuration |

The other method is the regulate function, which should be called on a regular time interval.  This function accepts the input parameters of the nominal value and the actual value, with an in-out variable which will have the output variable after execution.  The function returns an error code

| **Error Code** | **Problem** |
|:---------------|:------------|
| 0              | No Error    |
| -1             | Overflow Error |

## Sample Program ##
This program is designed to emulate a real control system.

```
#include "pid.h"
#include "../signals/ticker.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

PID pid;

void control_loop() {

        static unsigned short int value = 0;
        unsigned short int out = 0;

        if(!inDeadBand(1234,value,5)) {
                if(pid.regulate(1234,value,out) != 0) {
                        cout << "ERROR" << endl;
                }

                if(value > 1234) value -= out;
                else value += out;

                cout << "out: " << out << "\t";
                cout << "newVal: " << value << endl;
        } else {
                cout << "In Deadband at: " << value << endl;
        }

}


int main(int argc, char **argv) {

        if(pid.init(40,50,0,50,100) < 0) {
                cout << "Warning Unstable PID Parameters" << endl;
                return -1;
        }

        register_timeout(control_loop,0.05,true);
        
        while(1) {;}
        return 0;
}
```

# References #
[PID Controller on WiKiPedia](http://en.wikipedia.org/wiki/PID_controller#PID_controller_theory)

# Extra #
