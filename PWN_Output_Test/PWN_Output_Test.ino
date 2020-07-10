void setup() {
  
}

void loop() {
  int i=0;

  // 10번 LED 점점 밝게~!
  // 11번 LED 점점 어둡게~!
  for(;i<255;i++)
  {
    analogWrite(10,i);
    analogWrite(11,255-i);
    delay(5);
  }

  // 10번 LED 점점 어둡게~!
  // 11번 LED 점점 밝게~!
  for(;i>0;i--)
  {
    analogWrite(10,i);
    analogWrite(11,255-i);
    delay(5);
  }
}//
//[출처] [FA-DUINO 18TA] 아두이노 PWM 출력 테스트|작성자 컴파일제품스터디
