// DRV8825 : motor_delay max : 800
// DRV8834 : motor_delay max : 200 (1/4 step)

const int dirPin = 8;
const int stepperPin = 9;
const int enPin = 12;
const int CCW = 0;
const int CW = 1;
const int motor_delay = 200;
int Accel = 400;
int Decel = 200;

void setup()
{
    pinMode(dirPin, OUTPUT);
    pinMode(stepperPin, OUTPUT);
    pinMode(enPin, OUTPUT);
}

void step(boolean dir, int steps)
{
    digitalWrite(dirPin, dir);
    delayMicroseconds(20);
    for (int i = Accel; i > 0; i--)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay + i);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay + i);
    }
    for (int i = 0; i < steps; i++)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay);
    }
    for (int i = 0; i < Decel; i++)
    {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(motor_delay + i);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(motor_delay + i);
    }
}

void loop()
{
  digitalWrite(enPin, LOW);
  step(CW, 6400);
  digitalWrite(enPin, HIGH);
  delay(2000);
}
