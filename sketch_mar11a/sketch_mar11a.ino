
void setup() {
  // 여기에 초기화 부분을 넣어 주세요. 이것은 한번만 실행됩니다:
  Serial.begin(9600);
}

void loop() {
  // 여기에 주 처리 부분을 넣어 주세요. 이것은 반복 실행됩니다:
  unsigned long p_t, c_t;
  
  p_t = micros();           // 처음 us 시간을 읽습니다
  delayMicroseconds(1000);  // 1000us 대기 시간을 갖습니다
  c_t = micros();           // 현재 us 시간을 읽습니다

  // 현재 us 시간을 출력하고, 현재 us 시간 - 처음 us 시간 차를
  // 출력합니다:
  Serial.print("시간(us): "); Serial.print(c_t); 
  Serial.print('\t'); Serial.println(c_t - p_t); 
}
