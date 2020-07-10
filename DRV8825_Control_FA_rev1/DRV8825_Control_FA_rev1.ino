// DRV8825 : motor_delay max : 800
// DRV8834 : motor_delay max : 200 (1/4 step)

const int dirPin = A1;      // 모터 회전 방향 출력 (5V)
const int stepperPin = A2;  // 모터 회전 펄스 출력 (5V)
const int enPin = A0;       // 모터 기동 준비 출력 (5V)
const int CCW = 0; 
const int CW = 1;

const int motor_delay = 50; // 회전속도 (작을수록 빨리회전)

int Accel = 400; // 가속 속도
int Decel = 200; // 감속 속도

const int markPin = 4;  // 마크 검출 입력 단자 (24V)
int markState = 0;        
const int startPin = 2;  // 작업 시작 입력 단자 (24V)
int startState = 0;
const int teachPin = 6;  // 마크 티칭 입력 단자 (24V)
int teachState = 0;

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

    pinMode(solpushPin, OUTPUT);        // 24V
    pinMode(backlightPin, OUTPUT);
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
  teachState = digitalRead (teachPin); // 마크센서 티칭 유무 판단

  if (teachState == HIGH )  //마크센서 티칭 중이면 백라이트 작동
  {
    digitalWrite (backlightPin, HIGH);
    delay(15000);
    digitalWrite (backlightPin, LOW);
  };

  if (startState == HIGH )
  {
    digitalWrite(enPin, HIGH); // 모터 기동준비 해재 (EN 신호가 HIGH면 모터드라이버는 기동준비 안된걸로 판단!!)
    markState = digitalRead (markPin);
    teachState = digitalRead (teachPin);
    digitalWrite(solpushPin, LOW); // 푸쉬 솔 작동 (LOW : 후진)   
  } else {
      digitalWrite(solpushPin, HIGH); // 푸쉬 솔 작동 (HIGH : 전진)  **
      delay (500);                   // 푸쉬 솔 동작 대기
      
      while (markState == LOW) // 마크 검출 시작
        {
        digitalWrite(dirPin, CW); // 모터 회전 방향 결정 (CW : 역회전)
        digitalWrite(enPin, LOW);
        digitalWrite(stepperPin, HIGH);
        delayMicroseconds(motor_delay);
        digitalWrite(stepperPin, LOW);
        delayMicroseconds(motor_delay); // 모터 회전 속도
        markState = digitalRead (markPin); // 스탭마다 마크 검출유무 판정
        }

        digitalWrite(enPin, LOW);
        step(CW, 1);
        delayMicroseconds(100);
        digitalWrite(enPin, HIGH);
        delay(3000);  // 작업시간 
      };
     // digitalWrite(solpushPin, LOW); // 푸쉬 솔 작동 (LOW : 후진)
     markState = 0;
     teachState = 0;
     solpushState = 0;
   
  // digitalWrite(enPin, LOW);
  // step(CW, 6400);
  // digitalWrite(enPin, HIGH);
  // delay(2000);
}
