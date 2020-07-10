#include <EEPROM.h>



int randomNumber;



void setup(){
  Serial.begin(115200); // 시리얼 통신 초기화
  randomSeed(analogRead(0));
}



void loop(){
   Serial.println("Writing random numbers...");
/*
   for(int i = 0; i < 6; i=i+2) {             // EEPROM에 데이터 쓰기
    randomNumber = random(8888, 9999);
    Serial.print("Random Number : "); Serial.println(randomNumber);
    byte hiByte = highByte(randomNumber);
    byte loByte = lowByte(randomNumber);
    EEPROM.write(i, hiByte); // write(주소, 값)
    Serial.println("EEPROM Address : " + String(i) + "\t Value : " + hiByte);
    EEPROM.write(i+1, loByte); // write(주소, 값)
    Serial.println("EEPROM Address : " + String(i+1) + "\t Value : " + loByte);
    Serial.println();
    delay(100);
  }

*/

  Serial.println();
 
  Serial.println("Reading EEPROM...");
  for(int i = 0; i < 6; i=i+2) {               // EEPROM에 데이터 읽기
    byte hiByte = EEPROM.read(i); // read(주소)
    byte loByte = EEPROM.read(i+1); // read(주소)
    randomNumber = word(hiByte, loByte);
    Serial.println("EEPROM Address : " + String(i) + "\t Value : " + randomNumber);
   
    delay(100);
  }

  while(true); // 1회만 시행 후 대기
}
