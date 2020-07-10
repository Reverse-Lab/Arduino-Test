/*******************************************************************************************
 * This example tests illuminating individual segments of the display. Pass in the segment
 * and digit you wish to illuminate to illuminateSegement().
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: January 31, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! LINK GOES HERE
 * 
 * This code is Lemonadeware; if you see me (or any other SparkFun employee) at the 
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Hardware Connections:
 * Attach Red Board to computer using micro-B USB cable.
 * Attach Qwiic Alphanumeric board to Red Board using Qwiic cable.
 * 
 * Distributed as-is; no warranty is given.
 *****************************************************************************************/
#include <SparkFun_Alphanumeric_Display.h>
HT16K33 display;

void setup()
{
  Serial.begin(115200);
  Serial.println("Qwiic Alphanumeric examples");
  Wire.begin(); //Join I2C bus

  //check if display will acknowledge
  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");

  display.illuminateSegment('A', 0);
  display.illuminateSegment('L', 1);
  display.illuminateSegment('I', 2);
  display.illuminateSegment('G', 3);
  display.updateDisplay();
}

void loop()
{
}
