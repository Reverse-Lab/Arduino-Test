/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix1 = Adafruit_7segment();
Adafruit_7segment matrix2 = Adafruit_7segment();

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix1.begin(0x70);
  matrix2.begin(0x71);
}

void loop() {
  // try to print a number thats too long
  matrix1.print(10000, DEC);
  matrix1.writeDisplay();
  delay(500);

  // print a hex number
  matrix1.print(0xBEEF, HEX);
  matrix1.writeDisplay();
  delay(500);

  // print a floating point 
  matrix1.print(123.4);
  matrix1.writeDisplay();
  delay(500);

  matrix2.print(10000, DEC);
  matrix2.writeDisplay();
  delay(1500);

  // print a hex number
  matrix2.print(0xBEEF, HEX);
  matrix2.writeDisplay();
  delay(1500);

  // print a floating point 
  matrix2.print(123.4);
  matrix2.writeDisplay();
  delay(1500);
  
  // print with print/println
  for (uint16_t counter = 0; counter < 1111; counter++) {
    matrix1.println(counter);
    matrix1.writeDisplay();
    delay(10);
  }

  // method #2 - draw each digit
  uint16_t blinkcounter = 0;
  boolean drawDots = false;
  for (uint16_t counter = 0; counter < 1111; counter++) {
    matrix1.writeDigitNum(0, (counter / 1000), drawDots);
    matrix1.writeDigitNum(1, (counter / 100) % 100, drawDots);
    //matrix1.drawColon(drawDots);
    matrix1.writeDigitNum(2, (counter / 10) % 10, drawDots);
    matrix1.writeDigitNum(3, counter % 10, drawDots);
   
    blinkcounter+=50;
    if (blinkcounter < 500) {
      drawDots = false;
    } else if (blinkcounter < 1000) {
      drawDots = true;
    } else {
      blinkcounter = 0;
    }
    matrix1.writeDisplay();
    delay(10);
  }
}
