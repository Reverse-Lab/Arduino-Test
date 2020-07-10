/*
  PrimeVfd Library - Blink
 
 Demonstrates the use a Futaba NA202MD13AA VFD (Prime VFD).
 
 This sketch prints "Hello World!" to the VFD and makes the 
 cursor block blink.
 
  The circuit:
  
 * VFD pin 2/TP2 to +5v @ 1a. Do not connect to Arduino +5v, it can not supply the needed current.
 * VFD pin 4/TP4 to PD2 (on regular Arduino, digital pin 2)  
 * VFD pin 6/TP6 to PD3 (on regular Arduino, digital pin 3)  
 * VFD pin 8/TP8 to PD4 (on regular Arduino, digital pin 4)
 * VFD pin 10/TP10 to 0v/GND. It must be connected to the power supply 0v/GND as well as the Arduino 0v/GND
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 8 Feb 2010
 by Tom Igoe
 modified 27 Feb 2011 - ported to VFD library
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/PrimeVfd
*/

// include the library code:
#include <PrimeVfd.h>

// initialize the library with the numbers of the interface pins
PrimeVfd vfd(PrimeVfd::PIN_CONFIG_PORT_D_OPTION_1);

void setup() {
  // start the library/displaying to the vfd 
  vfd.begin();
  // Print a message to the VFD.
  vfd.print("hello, world!");
}

void loop() {
  // Turn off the blinking cursor:
  vfd.noBlink();
  delay(3000);
   // Turn on the blinking cursor:
  vfd.blink();
  delay(3000);
}


