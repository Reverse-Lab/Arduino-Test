// 최종 수정 03-01
// DRV8825 : motor_delay max : 800
// DRV8834 : motor_delay max : 200 (1/4 step)

const int dirPin = A1;      // 모터 회전 방향 출력 (5V)
const int stepperPin = A2;  // 모터 회전 펄스 출력 (5V)
const int enPin = A0;       // 모터 기동 준비 출력 (5V)
const int CCW = 1; 
const int CW = 0;

const int motor_delay = 5; // 회전속도 (작을수록 빨리회전)
//int M_motor_delay = 5;

int Accel = 400; // 가속 스텝
int Decel = 200; // 감속 스텝

const int markPin = 4;  // 마크 검출 입력 단자 (24V)
int markState = 0;        
const int startPin = 2;  // 작업 시작 입력 단자 (24V)
int startState = 0;
const int teachPin = 6;  // 마크 티칭 입력 단자 (24V)
int teachState = 0;
int t_Count = 0;

// 0301 추가
const int teachoutPin = 12;
int teachoutState = 0;

const int teachLED = 13;
int ledState = 0;
//int potentioValue = 0;
//int potValue = 0;

const int solpushPin = 10;  // 푸쉬 솔 출력 단자 (24V)
int solpushState = 0;        
const int backlightPin = 11;  // 백 라이트 입력 단자 (24V)
int backlightState = 0;

void setup()
{
    pinMode(dirPin, OUTPUT);        // 5V
    pinMode(stepperPin, OUTPUT);
    pinMode(enPin, OUTPUT);

    pinMode(markPin, INPUT);    //24V
    pinMode(startPin, INPUT);
    pinMode(teachPin, INPUT);
    pinMode(teachoutPin, OUTPUT);
    pinMode(teachLED, OUTPUT);

    pinMode(solpushPin, OUTPUT);        // 24V
    pinMode(backlightPin, OUTPUT);

    Serial.begin(9600);
}

void step(boolean dir, int steps)
{
    digitalWrite(dirPin, dir);
    delayMicroseconds(20);
    /*for (int i = Accel; i > 0; i--)
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
    }*/
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
  startState = digitalRead (startPin); // 작업 시작 유무 판단
  teachState = digitalRead (teachPin); // 리모트 티칭 티칭 유무 판단


  if (teachState == HIGH ) // 센서 리모트 티칭 버튼 HIGH 경우
   {if (t_Count == 0) {delay(10); t_Count = 1;}}
  if (teachState == LOW)
  { if (t_Count == 1) 
    {
      switch(teachoutState){ // 티칭 버튼 누른 횟수 판단
        case 0 :             // 처음 누른 경우
              digitalWrite (teachoutPin, HIGH);    // 센서로 HIGH 출력
              digitalWrite (backlightPin, HIGH);   // 백 라이트 ON
              digitalWrite (teachLED, HIGH);       // 티칭 버튼 LED ON
              digitalWrite (solpushPin, HIGH);     // 푸쉬 솔 전진
              delay(150);                          // 티칭 준비 티칭시간
              teachoutState = 1;                   // 버튼 누른 횟수 1로 설정
              //Serial.println(teachoutState);
              break;
        case 1 :                                   // 버튼 누른 횟수 1인 경우
              digitalWrite (teachoutPin, LOW);     // 센서로 LOW 출력
              delay(150);                          // 150ms 대기 (센서 메뉴얼 참고 - 리모트 티칭)
              digitalWrite (teachoutPin,HIGH);     // 센서로 HIGH 출력 (마크 티칭 대기모드 진입)
              teachoutState = 2;                   // 버튼 누른 횟수 2로 설정
              //Serial.println(teachoutState);
              break;
        case 2 :                                   // 버튼 누른 횟수 2인 경우
              digitalWrite (teachoutPin, LOW);     // 센서로 출력 LOW (마크 티칭 완료)
              delay(1000);                         // 1초 대기
              digitalWrite (solpushPin, LOW);      // 푸쉬 솔 후진
              digitalWrite (backlightPin, LOW);    // 백 라이트 OFF
              digitalWrite (teachLED, LOW);        // 티칭 버튼 LED OFF
              teachoutState = 0;                   // 리모트 티칭 완료, 버튼 누른 횟수 초기화
              //Serial.println(teachoutState);
        default : break;
              //teachoutState = 0;
      }  t_Count =0;
    } 
  };
  
  if (startState == HIGH ) // 근접센서 HIGH는 대기 모드로 인식
  {
    digitalWrite(enPin, HIGH); // 모터 기동준비 해재 (EN 신호가 HIGH면 모터드라이버는 기동준비 안된걸로 판단!!)
    markState = digitalRead (markPin);
    ledState = digitalRead (teachLED);
    if (ledState == 0 ) 
    {
     digitalWrite(solpushPin, LOW); // 푸쉬 솔 작동 (LOW : 후진)
    }  
  } 
  else // 근접센서 LOW인 경우 (작업 개시로 판단)
  {
      digitalWrite(solpushPin, HIGH); // 푸쉬 솔 작동 (HIGH : 전진) 
      delay (100);                    // 푸쉬 솔 전진 완료 대기 (0.1초)
      
      while (markState == LOW)        // 마크 검출 시작
        {
          digitalWrite (backlightPin, HIGH); //검출 전까지 백라이트 ON
          digitalWrite(dirPin, CW);          // 모터 회전 방향 결정 (CW : 정회전)
          digitalWrite(enPin, LOW);
          /* 모터 속도 가변 실패 (03-01 : 너무 느림)
          potentioValue = map(analogRead(potentioPin),0,1023,0,255);
          int potValue_Raw = potentioValue / 5;
          potValue = potValue_Raw * 5;
          M_motor_delay = potValue + 5;
          //Serial.println (M_motor_delay);
          */
          digitalWrite(stepperPin, HIGH);
          delayMicroseconds(motor_delay);
          digitalWrite(stepperPin, LOW);
          delayMicroseconds(motor_delay);    // 모터 회전 속도
          markState = digitalRead (markPin); // 스탭마다 마크 판정
        }

        digitalWrite(enPin, LOW);
        step(CW, 1);                      // 감속 스텝 적용
        delayMicroseconds(100);
        digitalWrite(enPin, HIGH);
        digitalWrite (backlightPin, LOW); // 검출 후 백라이트 OFF
        delay(3000);  // 작업시간 
        digitalWrite(solpushPin, LOW);   // 푸쉬 솔 작동 (LOW : 후진)
        
      };
    
     markState = 0;
     teachState = 0;
     solpushState = 0;
   
  // digitalWrite(enPin, LOW);
  // step(CW, 6400);
  // digitalWrite(enPin, HIGH);
  // delay(2000);
}
