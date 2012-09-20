Toyotomi-HVAC
=============
This is an Toyotomi HVAC remote controller library for the Arduino platform.

It was created as a project within the "Distributed Systems II" lesson of the Computer Engineering and Informatics Department of the University of Patras. It is intended to support almost any remote controller function for a wide range of Toyotomi HVAC units. Although it is designed for Arduino Pro Mini (8 MHz), it can also be used on an Arduino Uno/Duemilanove (or any other model operating in 16 MHz clock frequency) by commenting the "CLK_8MHZ" macro definition in "Toyotomi.h" file. The system needs an Xbee module in order to be controlled via a 802.15.4 base station.


Installation

Move the libraries/Toyotomi-HVAC folder into your Arduino libraries folder. Then, copy the "Toyotomi.ino" file into your sketch folder.

Warning

This project is dependent on: XBee, XbeeRadio and Uberdust libraries.

You're done.
