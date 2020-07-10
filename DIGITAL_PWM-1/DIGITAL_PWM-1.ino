int ledPin = 6; // 아두이노 pwm핀(~ 핀)

int val; // ADC 핀에서 전압을 읽고 변환한 값을 저장하는 변수

int pwm = 0; // pwm 값

int old_pwm = 0; // 비교용 변수 선언

unsigned long read_now = 0;



void setup() {

  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

}



void loop() {

  if (millis() - read_now > 100) {

    read_now = millis();

    val = analogRead(A0);

    Serial.print("val: "); Serial.println(val);

    if (val <= 15) val = 0;

    pwm = val/4;

    if (old_pwm != pwm) {

      analogWrite(ledPin, pwm); 

      old_pwm = pwm;

      Serial.print("pwm: "); Serial.println(pwm);

    }

  }

}



//출처: https://postpop.tistory.com/11 [postpop]
