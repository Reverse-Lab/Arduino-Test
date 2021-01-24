//Sine_analogwrite

float s[101];
float t=0.01;
int pin = 13;
void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}
void loop() {
  for (int i=0; i<101; i++)  {
   s[i]=127.5*(1+sin(2*PI*i*t));
   Serial.println(s[i], 2);
   analogWrite(pin, s[i]);
   delayMicroseconds(100);
  }

}
