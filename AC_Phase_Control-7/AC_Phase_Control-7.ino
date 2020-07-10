#include <TimerOne.h>


class Flasher
{
  int AC_LOAD;
  long OffTime;
  long Trigger;

  int acState;
  unsigned long previousMicros;

  public:
  Flasher(int pin, long off, long trigger)
  {
    AC_LOAD = pin;
    pinMode(AC_LOAD, OUTPUT);

    OffTime =  65 * off;
    Trigger = trigger;

    acState = LOW;
    previousMicros = 0;    
  }

  void Update()
  {
    unsigned long currentMicros = micros();
    if(currentMicros - previousMicros >= OffTime)
    {
      acState = HIGH;
      previousMicros = currentMicros;
      digitalWrite(AC_LOAD, acState);
      delayMicroseconds(8.33);
      acState = LOW;
      digitalWrite(AC_LOAD, acState);
      Serial.println(OffTime);
    }
    /*
    else if((acState == LOW) && (currentMicros - previousMicros >= Trigger))
    {
      acState = HIGH;
      previousMicros = currentMicros;
      digitalWrite(AC_LOAD, acState);
      Serial.println(Trigger);
    }*/
  }

  void SetOnTime(long NewOnTime)
  {
    OffTime = NewOnTime;
  }
};

Flasher Motor(11, 50, 9);

void Motor_Update()
{
  Motor.Update();
}

void setup()
{
  attachInterrupt(0, Motor_Update, RISING);  
  Serial.begin(57600);
}


void loop()  {
  for(int i=5;i<120;i++){
    Motor.SetOnTime(i);
  }  
  for(int i=120;i>=5;i--){
    Motor.SetOnTime(i);
  }  
}

  
