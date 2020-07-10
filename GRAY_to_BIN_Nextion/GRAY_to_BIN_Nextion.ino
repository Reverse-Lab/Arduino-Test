#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary 

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Serial you are going to use

int led = 13;

const int grayPin[8]={3,4,5,6,7,8,9,10};

void grayPinSet(){
  for(int8_t i=0;i<8;i++)
  {
    pinMode(grayPin[i], INPUT);
  }
}

unsigned int rpm;
volatile byte pulses;
unsigned long timeold;
unsigned int pulsesperturn = 256;

void counter()
{
  pulses++;
  if(millis() - timeold > 1)
  {
    detachInterrupt(1);
    rpm = (60*1000 / pulsesperturn/pulses) / (millis() - timeold)* pulses;
    timeold = millis();
    pulses = 0;
    attachInterrupt(1, encoder_Update, CHANGE);
  }
}

void encoder_Update()
{
  int i;
  int rad;
  int enc[8];
  for(i=0;i<8;i++){
    enc[i] = 1 - digitalRead(grayPin[i]);
  }
  
  uint8_t pos=0;
  pos = (enc[7]<<7);
  for(i=6;i>=0;i--)  
  {
    if((enc[i]+bitRead(pos,i+1))==0 || (enc[i]+bitRead(pos,i+1))==2 )
    {
      pos|=(0<<i);
    }else if((enc[i]+bitRead(pos,i+1))==1)
    {
      pos|=(1<<i);
    }
  }

  counter();

  rad = pos*1.40625;
  if ((rad >= 0) && (rad <180)) digitalWrite(led, HIGH); 
  if ((rad >= 180) && (rad < 360)) digitalWrite(led, LOW);

   myNex.writeNum("n0.val", rad);
   myNex.writeNum("n1.val", rpm);
}

void setup() {

  pinMode (led, OUTPUT);
  
  myNex.begin(9600);
  grayPinSet();
  attachInterrupt(1, encoder_Update, CHANGE);
}

void loop() {
  
}
