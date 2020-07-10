#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

#include <TimerOne.h>

int OffTime = 98;
int acPin = 12;
int rpmSensor = 3;
unsigned int rpm;
volatile byte pulses;
unsigned long timeold;
unsigned int pulsesperturn = 1;
int pulseLed = 13;

int num_blade = 2;

void counter()
{
  pulses++;
  digitalWrite (pulseLed, HIGH); 
    if (millis() - timeold >= 100){
        detachInterrupt(digitalPinToInterrupt(rpmSensor));
//        detachInterrupt(digitalPinToInterrupt(set_blade_pin));
        rpm = (60 * 1000 / pulsesperturn / num_blade)/ (millis() - timeold)* pulses;
        timeold = millis();
        pulses = 0;
        Serial.print("RPM = "); 
        Serial.println(rpm, DEC);
        digitalWrite (pulseLed, LOW);
      attachInterrupt(1, counter, FALLING);
    }
}

const int grayPin[8]={4,5,6,7,8,9,10,11};

void grayPinSet(){
  for(int8_t i=0;i<8;i++)
  {
    pinMode(grayPin[i], INPUT);
  }
}

void setup()
{
  pinMode(acPin, OUTPUT);
  pinMode(rpmSensor, INPUT);
  pinMode(pulseLed, OUTPUT);
  grayPinSet();
  attachInterrupt(0, Motor_Update, RISING);  
  attachInterrupt(1,counter,FALLING);
  attachPCINT(digitalPinToPCINT(4), Encoder_Update, CHANGE);  
  Serial.begin(115200);
}

void Motor_Update()
{
  Timer1.initialize (65 * OffTime);
  Timer1.attachInterrupt (acPhaseISR);
}

void Encoder_Update()
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
}

void acPhaseISR()
{
  digitalWrite(acPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(acPin, LOW);
  Timer1.initialize (1000000);
}

void loop()  {
  /*
  for(int i=35;i<120;i++){
    OffTime = i;
    delay(100);
  }  
  for(int i=120;i>=35;i--){
    OffTime = i;
    delay(100);
  }  */
}

  
