#include <AFMotor.h>    // L293D 모터 드라이브 라이브러리
#include <Servo.h>       // 서보모터 라이브러리

Servo servo1;    // 서보모터 1
Servo servo2;   // 서보모터 2

void setup() {
  Serial.begin(9600);  // 시리얼 모니터 시작. 속도는 9600
  
  servo1.attach(9); // 서보모터 1 on
  servo2.attach(10); // 서보모터 2 on
}

void loop() {
  servo1.write(0);    // 서보모터 1의 각도를 0으로
  delay(1000);        // 딜레이 1000ms
  Serial.println(int(servo1.read()));
  servo1.write(180);  // 서보모터 2의 각도를 180으로
  delay(1000);        // 딜레이 1000ms

  Serial.println(int(servo1.read()));
     
  servo2.write(0);    // 서보모터 2의 각도를 0으로 
  delay(1000);        // 딜레이 1000ms
  Serial.println(int(servo1.read()));
  servo2.write(180);  // 서보모터 2의 각도를 180으로
  delay(1000);        // 딜레이 1000ms

  Serial.println(int(servo2.read()));
}
