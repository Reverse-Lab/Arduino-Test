// DRV8825 : motor_delay max : 800
// DRV8834 : motor_delay max : 200 (1/4 step)

const int dirPin = 6;      // 모터 회전 방향 출력 (5V)
const int stepperPin = 5;  // 모터 회전 펄스 출력 (5V)
const int enPin = 7;       // 모터 기동 준비 출력 (5V)
const int CCW = 0; 
const int CW = 1;

unsigned long i;

const int motor_delay = 2; // 회전속도 (작을수록 빨리회전)

int Accel = 10; // 가속 속도
int Decel = 10; // 감속 속도

void setup()
{
    pinMode(dirPin, OUTPUT);        // 5V
    pinMode(stepperPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    digitalWrite (enPin, HIGH);
}

void step(boolean dir, unsigned int steps)
{
    //digitalWrite(dirPin, dir);
    //delayMicroseconds(20);
    /*
    for (int i = Accel; i > 2; i--)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay + i);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay + i);
    }*/
    for (i = 0; i < steps; i++)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay);
    }/*
    for (int i = 2; i < Decel; i++)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay + i);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay + i);
    }*/
}

void loop()
{
 
   digitalWrite(dirPin, LOW);
   step(CW, 100000);
   delay(2000);
   digitalWrite(dirPin, HIGH);
   step(CCW, 100000);
   delay(2000);
}
