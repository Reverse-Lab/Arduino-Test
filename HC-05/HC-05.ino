#include <SoftwareSerial.h>
//HC-05 AT MODE
SoftwareSerial BTSerial(5, 6);   //(bluetooth module Tx)--> Digital 2 
                                 //         "        Rx ---> Digital 3

void setup() {
 
  Serial.begin(115200);
  BTSerial.begin(38400);
 
}

void loop() {
  //if (BTSerial.available())
    //Serial.write(BTSerial.read());
  if (Serial.available())
    Serial.write(Serial.read());
 
}
