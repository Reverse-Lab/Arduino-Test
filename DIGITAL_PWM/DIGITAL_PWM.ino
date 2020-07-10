int ledPin = 6; // 아두이노 pwm핀(~ 핀)

int pwm = 0; // pwm 값

int fade = 1; // 증가값, 초기 '+', 255 도달시 '-' 변경, 0 도달시 '+'변경



void setup() {

  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

}



void loop() {

  analogWrite(ledPin, pwm); 

  delay(10); // 10밀리초

  pwm += fade;  // pwm 값에 fade값을 더해서 pwm에 저장

  if(pwm <= 0 || pwm >=255) { // pwm이 0이거나 255이면

    fade = -fade;  // '+', '-' 부호변경 

  }

  Serial.println(pwm);

}



//출처: https://postpop.tistory.com/11 [postpop]
