#include <TimerOne.h>

int OffTime;
int acPin = 11;

void setup()
{
  pinMode(acPin, OUTPUT);
  attachInterrupt(0, Motor_Update, RISING);  
  Serial.begin(57600);
}

void Motor_Update()
{
  Timer1.initialize (65 * OffTime);
  Timer1.attachInterrupt (acPhaseISR);
}

void acPhaseISR()
{
  digitalWrite(acPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(acPin, LOW);
  Timer1.initialize (1000000);
}

void loop()  {
  for(int i=60;i<110;i++){
    OffTime = i;
    delay(100);
  }  
  for(int i=110;i>=60;i--){
    OffTime = i;
    delay(100);
  }  
}

  
