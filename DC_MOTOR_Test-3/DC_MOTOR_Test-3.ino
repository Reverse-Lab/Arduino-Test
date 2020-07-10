
#define in1   5   //PE5
#define in2   15  //PB7
#define en    16  //PG3
#define val   45  //PF0

void setup() {

  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (en, OUTPUT);
  pinMode (val, INPUT);

  digitalWrite(en, HIGH);
  digitalWrite(in1, LOW);
}
void loop() 
{
    int vol = analogRead(val);
    int pwm = map(vol, 0, 1023, 0, 255);
    analogWrite(in2, pwm);
}
