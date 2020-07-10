int enablePin = 13;
int in1Pin = 12;
int in2Pin = 11;

int nSwitch = 0;
boolean bReverse = false;

int nSpeed = 70;
String sMeg = "Switch : ";


void setup()
{
  Serial.begin(9600);
  pinMode(enablePin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
}
 
void loop()
{
  
  if(Serial.available())
  {
    nSwitch = Serial.read();
  }
  
  if( nSwitch == '1')
  {
    bReverse = true;
  }
  else
  {
    bReverse = false;
  }

  setMotor(nSpeed, bReverse);
  Serial.println(sMeg + int(nSwitch) + ", Speed : " + int(nSpeed));
  delay(1000);
}
 
void setMotor(int nSpeed, boolean bReverse)
{
  analogWrite(enablePin, nSpeed);
  digitalWrite(in1Pin, ! bReverse);
  digitalWrite(in2Pin, bReverse);
}
