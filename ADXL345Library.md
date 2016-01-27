# Introduction #

The ADXL345 is a small, thin, low power, three-axis accelerometer with high resolution (13-bit) measurement up to ±16g. Digital output data is formatted as 16-bit twos complement and is accessible through either a SPI (3- or 4-wire) or I2C digital interface.

The ADXL345 is well suited for mobile device applications. It measures the static acceleration of gravity in tilt-sensing applications, as well as dynamic acceleration resulting from motion or shock. Its high resolution (4mg/LSB) enables resolution of inclination changes of as little as 0.25°.

Several special sensing functions are provided. Activity and inactivity sensing detect the presence or lack of motion and if the acceleration on any axis exceeds a user-set level. Tap sensing detects single and double taps. Free-Fall sensing detects if the device is falling. These functions can be mapped to interrupt output pins. An integrated 32 level FIFO can be used to store data to minimize host processor intervention.

Low power modes enable intelligent motion-based power management with threshold sensing and active acceleration measurement at extremely low power dissipation.

This library is used for communicating and using the ADXL345 chip.


# Details #
The library is designed to provide methods for get and set methods for all registers of the device.  The spec can be found within the [datasheet](http://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf).

Additionally, there are some top-level functions for easily using the chip.  If you find that there could be any other top-level functions, please submit a bug to the [issue tracker](http://code.google.com/p/uav-helicoptor-ece4007/issues/list)

The chip boots up into standby mode, which is an ultra low power mode.  While in standby, the chip will not make any measurements.

# Common Practices #
The following is a list of common practices for using the library:

  * Verify the chip by reading the device-id register. Should return 345 in octal.
  * Place the chip into standby mode when doing configurations
  * Specify your resolution and sensitivity ranges
  * Place the chip into measurement mode when ready to get data

# Tutorial #
The following is a brief section of code to show a simple program the reads and prints the ADXL345 data to stdout, in C++.

```
#include <iostream>
#include <stdlib.h>
#include "ADXL345.h"
#include "../time/ticker.h"

using namespace std;

ADXL345 accel;


void print_data() {
    short int x=0, y=0, z=0;
    accel.get_data_x_raw(x);
    accel.get_data_y_raw(y); 
    accel.get_data_z_raw(z);
    cout << "X: " << x << "\t"
         << "Y: " << y << "\t"
         << "Z: " << z << "\t"
         << endl;
}

int main() {

    //Verify connection to chip and bus by checking device id
    unsigned char id = 0;
    if(accel.get_dev_id(id) != 0) {
        cout << "Device not verified" << endl;
        exit(-1);
    } else {
        cout << "Device ID: " << oct << (int)id << endl;
    }

    //Set the device to measure in full resolution
    accel.set_full_res();

    //Set the device in measurement mode
    accel.set_measurement_mode();

        register_timeout(print_data,0.05,true);

    while(1) {;}

    return 0;

}

```