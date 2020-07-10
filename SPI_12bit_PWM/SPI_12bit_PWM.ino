
#include <SPI.h>

int LEDs[] = {4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4095};
SPISettings A(7500000, MSBFIRST, SPI_MODE0); 
//TLC5947 can accept 30MHz if individual device or 15MHz if in series with other devices.
#define SCLK 13
#define MOSI 11
int latch = 8;

void setup() {
Serial.begin(38400,SERIAL_8O1);
Serial.println("SPI TEST");

pinMode(latch, OUTPUT);

SPI.begin();
SPI.beginTransaction(A);

for(int i = 0; i < 24; i ++){             //For each LED channel
SPI.transfer(LEDs[i]);                    //Transfer first 8 bits of 12-bit data
SPI.transfer(LEDs[i] >> 8);};             //Transfer last 4 bits of 12-bit data
digitalWrite(latch, HIGH);
digitalWrite(latch, LOW);
};

void loop() { 
}
