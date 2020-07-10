#include <SoftwareSerial.h>
//HC-05 AT MODE
SoftwareSerial BTSerial(16, 17);   //bluetooth module Tx:Digital 2 Rx:Digital 3

void setup() {
 
  Serial.begin(9600);
  BTSerial.begin(38400);
 
}

void loop() {
  if (BTSerial.available())
    Serial.write(BTSerial.read());
  if (Serial.available())
    BTSerial.write(Serial.read());
 
}
