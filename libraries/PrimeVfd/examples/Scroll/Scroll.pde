/*
  PrimeVfd Library - scrollDisplayLeft() and scrollDisplayRight()
 
 Demonstrates the use a Futaba NA202MD13AA VFD (Prime VFD).
 
 This sketch prints "Hello World!" to the VFD and uses the
 scrollDisplayLeft() and scrollDisplayRight() methods to scroll
 the text.
 
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
  delay(1000);
}

void loop() {
  // scroll 13 positions (string length) to the left 
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    vfd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }

  // scroll 33 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 33; positionCounter++) {
    // scroll one position right:
    vfd.scrollDisplayRight(); 
    // wait a bit:
    delay(150);
  }
  
    // scroll 20 positions (display length + string length) to the left
    // to move it back to center:
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position left:
    vfd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }
  
  // delay at the end of the full loop:
  delay(1000);

}

