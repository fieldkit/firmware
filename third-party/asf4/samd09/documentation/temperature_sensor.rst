Temperature sensor
==================

Temperature sensor is middleware which provides API to measure temperature.

There can be any amount of instances of same or different temperature sensor in a system with the limitation by available
memory.

Architecture and provided functionality
---------------------------------------

All temperature sensors are composed of two parts:

* Common interface

Common interface part contains interface that is shared between all temperature sensors. Common interface is a driver
for a generic temperature sensor. Each instance of particular temperature sensor contains one instance of generic temperature sensor.
This allows applications to use common interface of temperature sensor despite which sensor is used in a system and simplifies
switching to another sensor.

* Sensor specific

Implementation for specific temperature sensors.

AT30TSE75X Temperature Sensor
=============================

User guide for the IO1 Xplained Pro can be found here: http://www.atmel.com/images/atmel-42078-io1-xplained-pro_user-guide.pdf

The mode I2C driver being used by middleware should be set standard/fast mode, the baud rate should be set to 100000.
Set I2C driver pins to correspond to the standard header I2C pins which are pins 11 and 12 on
the standard EXT header of the Xplained pro.
