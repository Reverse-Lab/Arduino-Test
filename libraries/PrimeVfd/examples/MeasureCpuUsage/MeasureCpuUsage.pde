/*
  PrimeVfd Library - Measure CPU Usage
 
 Demonstrates the use a Futaba NA202MD13AA VFD (Prime VFD).
 
 This sketch computes the CPU usage of up to 4 simultaneous
 VFDs.
 
  The circuit:
  
 * All VFD pin 2/TP2 to +5v @ 1a. Do not connect to Arduino +5v, it can not supply the needed current.
 * All VFD pin 4/TP4 to PD2 (on regular Arduino, digital pin 2)  
 * All VFD pin 6/TP6 to PD3 (on regular Arduino, digital pin 3)  
 * All VFD pin 10/TP10 to 0v/GND. It must be connected to the power supply 0v/GND as well as the Arduino 0v/GND
 * VFD 1 pin 8/TP8 to PD4 (on regular Arduino, digital pin 4)
 * VFD 2 pin 8/TP8 to PD5 (on regular Arduino, digital pin 4)
 * VFD 3 pin 8/TP8 to PD6 (on regular Arduino, digital pin 4)
 * VFD 4 pin 8/TP8 to PD7 (on regular Arduino, digital pin 4)
 
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

#define NUM_DISPLAYS 1

#if (NUM_DISPLAYS < 1) || (NUM_DISPLAYS > 4)
#error NUM_DISPLAYS must be 1, 2, 3 or 4.
#endif

// initialize the library with the numbers of the interface pins
PrimeVfd vfd1(PrimeVfd::PIN_CONFIG_PORT_D_OPTION_1);
#if NUM_DISPLAYS >= 2
  PrimeVfd vfd2(PrimeVfd::PIN_CONFIG_PORT_D_OPTION_2);
#endif
#if NUM_DISPLAYS >= 3
  PrimeVfd vfd3(PrimeVfd::PIN_CONFIG_PORT_D_OPTION_3);
#endif
#if NUM_DISPLAYS >= 4
  PrimeVfd vfd4(PrimeVfd::PIN_CONFIG_PORT_D_OPTION_4);
#endif

static unsigned long baseLineWorkCount;

unsigned long doWork()
{
  // Count up until 1second timer runs out.
  unsigned long workCount = 0;
  
  unsigned long startTime = micros();
  while ( (micros()-startTime) < 1000000 ) // Loop for 1 second.
  {
    workCount++;
  }

  return workCount;
}

void setup() {
  
  // Compute how much work we can do without
  // the displays running.
  baseLineWorkCount = doWork();
  
  // start the library/displaying to the vfd(s)
  // and display which vfd we are.
  vfd1.begin();
  vfd1.print("Display 1");
  
#if NUM_DISPLAYS >= 2
  vfd2.begin();
  vfd2.print("Display 2");
#endif

#if NUM_DISPLAYS >= 3
  vfd3.begin();
  vfd3.print("Display 3");
#endif

#if NUM_DISPLAYS >= 4
  vfd4.begin();
  vfd4.print("Display 4");
#endif
}

void loop() {
  
  unsigned long currentWorkCount = doWork();
  unsigned long percentage = (baseLineWorkCount - currentWorkCount) * 100 / baseLineWorkCount;

  vfd1.setCursor(0, 1);
  vfd1.print("CPU usage: ");
  vfd1.print(percentage);
  vfd1.print("%");

#if NUM_DISPLAYS >= 2
  vfd2.setCursor(0, 1);
  vfd2.print("CPU usage: ");
  vfd2.print(percentage);
  vfd2.print("%");
#endif  
  
#if NUM_DISPLAYS >= 3
  vfd3.setCursor(0, 1);
  vfd3.print("CPU usage: ");
  vfd3.print(percentage);
  vfd3.print("%");
#endif  
  
#if NUM_DISPLAYS >= 4
  vfd4.setCursor(0, 1);
  vfd4.print("CPU usage: ");
  vfd4.print(percentage);
  vfd4.print("%");
#endif  
}

