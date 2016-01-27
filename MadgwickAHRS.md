# Introduction #

An Inertial Measurement (IMU) is a sensor array consisting of an accelerometer and a gyroscope.  Sometimes, an IMU can incorporate a magnetometer.  In this case, the IMU can be referred to as a Magnetic, Angular Rate, and Gravity (MARG) sensor.  Each of these sensing devices should be 3-axes.  A MARG system, also called an Attitude and Heading Reference Systems (AHRS) system, are able to provide a complete measurement of orientation relative to the direction of gravity and the Earth's magnetic field.

Gyroscopes measure angular velocity, which, can be integrated over time in order to compute orientation. Most gyroscopes used in consumer and low-cost applications are Mirco Electrical Mechanical Systems (MEMS).  Unfortunately, these devices tend to drift in measurement over time, and therefore, integration with these errors will accumulate error in the orientation computations. Because of this, the addition of the other sensors are needed along with an orientation filter in order to correctly estimate the true orientation.

Many orientation filters use a Kalman filter for their orientation filter.  Kalman filters are accurate and effective, but have several disadvantages.  They can be difficult to implement and require covariance
models.  These models needed to describe rotational kinematics in three-dimensions require large state vectors and an implementation of an extended Kalman filter.  This, in turn, makes the Kalman filter computationally strenuous.

Mahoney _et al_ developed a complimentary filter that is simple to implement, computationally efficient, and effectively solves the orientation filter.  However, the performance is only useful for IMU devices and not the MARG sensor arrays.  Madgwick developed a new orientation filter that is applicable to both the IMU and MARG systems.  The filter includes: a single adjustable parameter defined by observable systems characteristics, analytically and optimized gradient descent algorithm enabling performance at low sampling rates, an on-line magnetic distortion compensation algorithm, and a gyroscope bias drift compensation.


# Background #

This section discusses background material that is necessary in fully understanding the algorithm and how it works.

## Accelerometer ##
![http://upload.wikimedia.org/wikipedia/commons/thumb/4/4d/Accelerometer.png/220px-Accelerometer.png](http://upload.wikimedia.org/wikipedia/commons/thumb/4/4d/Accelerometer.png/220px-Accelerometer.png)

An accelerometer is a device that measures proper acceleration, which is acceleration relative to a free-fall, or inertial, observer who is momentarily at rest relative to the object being measured.  This is different than coordinate acceleration which is completely dependent on the chosen coordinate system and set of observers. For example, an acclerometer will measure a value of g in the upward direction when it is stationary on the ground.  This is due to the fact that the have the weight of m\*g, and that is the force that the device is accelerating upward at from a motionless observer.  If the device is in free-fall, toward the center of the Earth, it will measure zero due to it being at rest by the observer, as it is accelerating downward at the same rate, but opposite in direction, that it is at rest.

Typically, accelerometer measurements are fairly noisey, but their measurements do not drift over time


## Gyroscope ##
![http://upload.wikimedia.org/wikipedia/commons/thumb/e/e2/3D_Gyroscope.png/250px-3D_Gyroscope.png](http://upload.wikimedia.org/wikipedia/commons/thumb/e/e2/3D_Gyroscope.png/250px-3D_Gyroscope.png)

Gyroscopes are devices that measure angular momentum.  This can be integrated over time to give orientation.  Unfortunately, MEMS gyroscope measurements drift over time and can lead to accumulation of errors.

The fundamental equation describing a gyroscope is given as:

![http://upload.wikimedia.org/wikipedia/en/math/2/b/c/2bc4d95bec41022989fffcaa95683dae.png](http://upload.wikimedia.org/wikipedia/en/math/2/b/c/2bc4d95bec41022989fffcaa95683dae.png)

where the pseudovectors τ and L are the torque on the gyroscope and its angular momentum.  The scalar I is its moment of inertia, the vector ω is its angular velocity, and the vector α is its angular acceleration.

## Magnetometer ##
![http://upload.wikimedia.org/wikipedia/commons/thumb/a/ac/Fluxgate_Magnetometers.ogv/mid-Fluxgate_Magnetometers.ogv.jpg](http://upload.wikimedia.org/wikipedia/commons/thumb/a/ac/Fluxgate_Magnetometers.ogv/mid-Fluxgate_Magnetometers.ogv.jpg)


A magnetometer is a device that is used to measure the strength and direction of magnetic fields. From this, it it easy to determine heading like a compass.


## Quaternion ##

A quaternion is a four-dimensional complex number that can be used to represent the orientation of a rigid body or coordinate frame in three-dimensional space.

Rest coming soon...

# Algorithm #

Coming soon...

# Simulation #

The following image shows some sample sensor data from each of the three sensors (accelerometer, gyroscope, magnetometer).  The test was performed by moving the MARG array +90degrees, then to -90 degrees, and back to 0 degrees over each of the three axes.

![http://jyore.com/site_media/img/software/madgwick_sensor_data.png](http://jyore.com/site_media/img/software/madgwick_sensor_data.png)


The following is the Euler angle representation of the output of the Madgwick orientation filter.  You can see each angle moves to +90 degrees, then -90 degrees, and then to rest.  The erratic behavior of phi and psi angles as theta approaches +-90 degrees happens in the conversion to the Euler angles and does not exist in the quaternion representation within the system.  The behavior is caused by a singularity called _Gimble Lock_.

![http://jyore.com/site_media/img/software/madgwick_filter.png](http://jyore.com/site_media/img/software/madgwick_filter.png)


# Video #
<a href='http://www.youtube.com/watch?feature=player_embedded&v=tL4JDgeRweI' target='_blank'><img src='http://img.youtube.com/vi/tL4JDgeRweI/0.jpg' width='425' height=344 /></a>

# References #
  * [Madgwick's Original Publication](http://www.x-io.co.uk/res/doc/madgwick_internal_report.pdf)
  * [Acclerometer Wiki](http://en.wikipedia.org/wiki/Accelerometer)
  * [Gyroscope Wiki](http://en.wikipedia.org/wiki/Gyroscope)
  * [Magnetometer Wiki](http://en.wikipedia.org/wiki/Magnetometer)

# Extra #
