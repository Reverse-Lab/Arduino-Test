#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

const int grayPin[8]={4,5,6,7,8,9,10,11};

void grayPinSet(){
  for(int8_t i=0;i<8;i++)
  {
    pinMode(grayPin[i], INPUT);
  }
}

void encoder_Update()
{
  int i;
  int enc[8];
  for(i=0;i<8;i++){
    enc[i] = 1 - digitalRead(grayPin[i]);
    Serial.print(enc[i]);
  }Serial.print(", ");
  
  uint8_t pos=0;
  pos= (enc[7]<<7);
  for(i=6;i>=0;i--)  
  {
    if( (enc[i]+bitRead(pos,i+1))==0 || (enc[i]+bitRead(pos,i+1))==2 )
    {
      pos|=(0<<i);
    }else if( (enc[i]+bitRead(pos,i+1))==1)
    {
      pos|=(1<<i);
    }
  }
  
  Serial.print(pos,BIN);Serial.print(", ");
  Serial.print(pos,DEC);Serial.print(", ");Serial.print(pos*1.40625);
  Serial.println();
  delay(10);
}

void setup() {
  Serial.begin(115200);
  grayPinSet();
  attachPCINT(digitalPinToPCINT(grayPin[1]), encoder_Update, CHANGE);
}

void loop() {
  
}
