// 최종 수정 03-07 (END 센서 : 작업종료 하강 감지 Flag 추가)
// DRV8825 : motor_delay max : 800
// DRV8834 : motor_delay max : 200 (1/4 step)
#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
modbusDevice regBank;
modbusSlave slave;

const int dirPin = A1;      // 모터 회전 방향 출력 (5V)
const int stepperPin = A2;  // 모터 회전 펄스 출력 (5V)
const int enPin = A0;       // 모터 기동 준비 출력 (5V)
const int CCW = 0; 
const int CW = 1;

const int motor_delay = 30; // 회전속도 (작을수록 빨리회전 : Minimum 15)
//int M_motor_delay = 5;

int Accel = 400; // 가속 스텝
int Decel = 400; // 감속 스텝

const int markPin = 4;  // 마크 검출 입력 단자 (24V)
int markState = 0;        
const int startPin = 2;  // 작업 시작 위치 근접 센서 
int startState = 0;
int startFlag = 0;

const int teachPin = 6;  // 마크 티칭 입력 단자 (24V)
int teachState = 0;
int t_Count = 0;

// 0301 추가
const int teachoutPin = 12;
int teachoutState = 0;

const int teachLED = A4;
int ledState = 0;
//int potentioValue = 0;
//int potValue = 0;

const int solpushPin = 13;  // 푸쉬 솔 출력 단자 (24V)
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

  // 03-06 추가   HMI 연동
  //Assign the Modbus Slave ID.
  regBank.setId(1);

  //Add Digital Output registers
  regBank.add(10001); // 업/다운
  regBank.add(10002); // 솔 부쉬/풀
  regBank.add(10003); // 모터 /온/오프
  regBank.add(10004); // 센서 온/오프
  regBank.add(10005); // 티칭 온/오프

    
  //Add Digital Output registers
  regBank.add(1);  // HMI 티칭 버튼


  slave._device = &regBank; 
  // Initialize the baudrate serial port 
  // Set in SCADA: Baud rate=9600,Data bits=8,Stop bits=1,Parity=None,Flow control=None
  slave.setBaud(57600);  

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
      } */
      for (int i = 0; i < steps; i++)
      {
        digitalWrite(stepperPin, HIGH);
        delayMicroseconds(motor_delay);
        digitalWrite(stepperPin, LOW);
        delayMicroseconds(motor_delay);
        markState = digitalRead (markPin);
        if (markState == HIGH) {break;}
      }
      
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

  //03-06 추가 HMI 연동
  //HMI Digital Input
  byte DI1 = digitalRead(startPin);  
  if (DI1 >= 1)regBank.set(10001,1);
  if (DI1 <= 0)regBank.set(10001,0);
  
  byte DI2 = digitalRead(solpushPin);
  if (DI2 >= 1)regBank.set(10002,1);
  if (DI2 <= 0)regBank.set(10002,0);
  
  byte DI3 = digitalRead(enPin);
  if (DI3 >= 1)regBank.set(10003,0);
  if (DI3 <= 0)regBank.set(10003,1);
  
  byte DI4 = digitalRead(markPin);
  if (DI4 >= 1)regBank.set(10004,1);
  if (DI4 <= 0)regBank.set(10004,0);
  
  //byte DI5 = digitalRead(teachPin);
  byte DI5 = teachState;
  if (DI5 >= 1)regBank.set(10005,1);
  if (DI5 <= 0)regBank.set(10005,0);

  //Digital output (세트비트 출력)
  int DO3 = regBank.get(1); // HMI 티칭 버튼 추가 (03-08)
  slave.run(); delay(5);
  if (DO3 <= 0) teachState = LOW; 
  if (DO3 >= 1) teachState = HIGH; 

  // teachState = digitalRead (teachPin); // 리모트 티칭 티칭 유무 판단 (버튼제거로 주석처리 : 추후 보완)

  if (teachState == HIGH ) // 센서 리모트 티칭 버튼 HIGH 경우
   { startState = digitalRead (startPin); // 작업시작 상승위치
    if (startState == LOW)
    { if (t_Count == 0) {delay(10); t_Count = 1;} } // 상승 위치인 경우 티칭 불가
   }
  if (teachState == LOW)
  { 
    if (t_Count == 1) 
    { 
      slave.run(); delay(30);
      switch(teachoutState){ // 티칭 버튼 누른 횟수 판단
        case 0 :             // 처음 누른 경우
              digitalWrite (teachoutPin, HIGH);              // 센서로 HIGH 출력
              digitalWrite (backlightPin, HIGH);             // 백 라이트 ON
              digitalWrite (teachLED, HIGH);                  // 티칭 버튼 LED ON
              digitalWrite (solpushPin, HIGH); slave.run(); delay(30);// 푸쉬 솔 전진
              delay(50);                                   // 티칭 준비 티칭시간
              teachoutState = 1;                            // 버튼 누른 횟수 1로 설정
              //Serial.println(teachoutState);
              break;
        case 1 :                                            // 버튼 누른 횟수 1인 경우
              digitalWrite (teachoutPin, LOW);              // 센서로 LOW 출력
              delay(150);                                   // 150ms 대기 (센서 메뉴얼 참고 - 리모트 티칭)
              digitalWrite (teachoutPin,HIGH);              // 센서로 HIGH 출력 (마크 티칭 대기모드 진입)
              teachoutState = 2;                            // 버튼 누른 횟수 2로 설정
              //Serial.println(teachoutState);
              break;
        case 2 :                                            // 버튼 누른 횟수 2인 경우
              digitalWrite (teachoutPin, LOW);              // 센서로 출력 LOW (마크 티칭 완료)
              delay(30);                                  // 1초 대기
              digitalWrite (solpushPin, LOW); slave.run(); delay(30);// 푸쉬 솔 후진
              digitalWrite (backlightPin, LOW);             // 백 라이트 OFF
              digitalWrite (teachLED, LOW);                 // 티칭 버튼 LED OFF
              teachoutState = 0;                            // 리모트 티칭 완료, 버튼 누른 횟수 초기화
              //Serial.println(teachoutState);
        default : break;
              //teachoutState = 0;
      }  t_Count =0; slave.run(); delay(30);
    } 
  };
  
  startState = digitalRead (startPin); // 작업시작 상승위치
  if (startState == LOW ) // 근접센서 LOW는 대기위치로 인식
  {
    if (startFlag == 1){ delay(10); startFlag = 0; }; // 근접센서 신호가 꺼지는 순간인지 확인
    
    digitalWrite(enPin, HIGH);  slave.run(); delay(30);// 모터 기동준비 해재 (EN 신호가 HIGH면 모터드라이버는 기동준비 안된걸로 판단!!)
    markState = digitalRead (markPin);
    ledState = digitalRead (teachLED); 
      if (ledState == 0 ) // 
      {
        digitalWrite(solpushPin, LOW); slave.run(); delay(30);// 푸쉬 솔 작동 (LOW : 후진)
      }
  }; 
  
  startState = digitalRead (startPin); // 작업시작 상승위치
  if (startState == HIGH ) // 근접센서 LOW 인 경우 (작업 개시로 판단)
  {
    if (startFlag == 0)     // 근접센서 신호가 켜지는 순간인지 확인
    {
      digitalWrite(solpushPin, HIGH); slave.run();    //delay(100);// 푸쉬 솔 작동 (HIGH : 전진) 
      delay (30);                                     // 푸쉬 솔 전진 완료 대기 (0.1초)
      digitalWrite (backlightPin, HIGH);              //검출 전까지 백라이트 ON
      regBank.set(10003,1); slave.run(); delay(10);   //모터 ON 강제 할당
      digitalWrite(enPin, LOW);
      step(CW, 12400);
      digitalWrite(enPin, HIGH); regBank.set(10003,0); slave.run(); delay(30); // 모터 OFF 강제 할당
      digitalWrite (backlightPin, LOW);           // 검출 후 백라이트 OFF

    } startFlag = 1;
  }
  markState = 0;
  teachState = 0;
  solpushState = 0;
  startState =0;
  
  // 03-08 추가 HMI 연동
   slave.run();
   delay(100);
}
