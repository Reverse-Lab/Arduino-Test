// https://m.blog.naver.com/PostView.nhn?blogId=yuyyulee&logNo=220345769854&proxyReferer=https:%2F%2Fwww.google.com%2F

int servoPin= 5;
int NP= 6;
int SON= 7;

int freq = 10000; // 20밀리초 (50Hz)
int minPulse = 600;
int maxPulse = 600;

void setup() {
  pinMode(servoPin, OUTPUT);
  pinMode(NP, OUTPUT);
  pinMode(SON, OUTPUT);
  
  digitalWrite(SON, HIGH);
  delay(1000);
  digitalWrite(NP, LOW);
  delay(1000);
  
  digitalWrite(servoPin, LOW);
}

void loop()
{
  for (int i=0; i<=180; i++){
    setServo(i);
  }
  for (int i=180; i>=0; i--){
    setServo(i);
  }
}

void setServo(int degree)
{
  int hiTime = 0;
  int lowTime = 0;

  if(degree < 0) degree = 0;
  if(degree >180) degree = 180;

  hiTime = (int)(minPulse + ((maxPulse - minPulse) / 180.0 * degree));
  lowTime = freq - hiTime;

  digitalWrite(servoPin, HIGH);
  delayMicroseconds(hiTime);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(lowTime);
  
}
