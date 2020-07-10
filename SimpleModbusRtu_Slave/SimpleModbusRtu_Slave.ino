/**
 *  SimpleModbusRtu_example
 *  http://www.comfile.co.kr
 */

#include <SimpleModbusRtu_uno.h>

uint16_t au16data[16];
uint8_t au8data[10];
char n;

void setup() {
  char i;
  for( i=2; i<10; i++){ pinMode(i, OUTPUT); }
  for(i=10; i<14; i++){ pinMode(i, INPUT); }
  for(i=16; i<20; i++){ pinMode(i, INPUT); }

  SetModbus(0, 115200, 1, au16data, au8data );
}

void loop() {
  StartModbus();

  for(n=0; n<8; n++){  digitalWrite(n+2, bitRead(au8data[0], n) ) ;}
  
  for(n=0; n<4; n++){ bitWrite( au8data[1],n, digitalRead(n+10) ) ;}
  for(n=0; n<4; n++){ bitWrite( au8data[1],n+4, digitalRead(n+16));}
  
  au16data[0] = analogRead(A0);
  au16data[1] = analogRead(A1);
}
