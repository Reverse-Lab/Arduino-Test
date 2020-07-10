#include <Key.h>
#include <Keypad.h>


#define footSW        37  //DI2  풋 스위치
#define updownSensor  38  //DI3 하강 실린더 센서 입력
#define rightSensor   39  //DI4 제판 좌측 센서 ON/OFF
#define leftSensor    40  //DI5 제판 우측 센서 ON/OFF
#define DI6           41

#define sqzSol        20  //DO13   // 스퀴즈 ON/OFF
#define rightSol      21  //DO14 // 제판 우측 이동
#define leftSol       22  //DO15  // 제판 좌측 이동
#define DO16          23  
#define DO17          24

#define Buz           25

#define LED1          45
#define LED2          46
#define LED3          47
#define LED4          48
#define LED5          49
#define LED6          50

const byte numRows = 7;
const byte numCols = 2;
int FND;

char keymap[numRows][numCols]={
  {'1', 'A'},
  {'2', 'B'},
  {'3', 'C'},
  {'4', 'D'},
  {'5', 'E'},
  {'6', 'F'},
  {'7', 'G'}
};

byte rowPins[numRows] = {2,3,4,5,6,7,8};
byte colPins[numCols] = {12,13};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


class Flasher
{
    // 클래스 멤버변수 선언
    int outPin;      // 출력 핀 이름
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time
 
    // These maintain the current state
    int solState;                   // 출력 상태 체크
    unsigned long previousMillis;      // will store last time LED was updated
 
  // 구조체 - creates a Flasher 
  // 멤버변수 초기화
  public:
  Flasher(int sol, long on, long off)
  {
    outPin = sol;
    pinMode(outPin, OUTPUT);    

    OnTime = on;  // 센서 감지 후 대기 시간
    OffTime = off;  // 센서 꺼진 후 복귀 시산

    solState = LOW;
    previousMillis = 0;
  }
 
  void ON()
  {
    unsigned long currentMillis = millis();

      if(solState == LOW)
      {
        solState = HIGH;  // 솔밸브 ON
        digitalWrite(outPin, solState);  // 출력 OFF (Update the actual LED)
      }
  }
  
  void OFF()
  {
    unsigned long currentMillis = millis();

      if(solState == HIGH)
      {
        solState = LOW;  // 솔밸브 ON
        digitalWrite(outPin, solState);  // 출력 OFF (Update the actual LED)
      }
  }
};

Flasher sqz(sqzSol, 200, 200);  // 스퀴즈 ON/OFF
Flasher right(rightSol, 500, 500);  // 제판 우측 이동 ON/OFF
Flasher left(leftSol, 500, 500);  // 제판 좌측 이동 ON/OFF
 
void setup() {

  pinMode(footSW, INPUT);
  pinMode(updownSensor, INPUT); // 업다운 실린더
  pinMode(rightSensor, INPUT); // 좌측 실린더
  pinMode(leftSensor, INPUT); // 우측 실린더

  pinMode(sqzSol, OUTPUT);
  pinMode(rightSol, OUTPUT); 
  pinMode(leftSol, OUTPUT);

  pinMode(Buz, OUTPUT);

}

void loop() {

      
  char btn = myKeypad.getKey();
  if(btn != NO_KEY) {
    
    digitalWrite(Buz, HIGH);
    delay(40);
    digitalWrite(Buz, LOW);
    delay(10);

  }

  if(btn == '1')
  {
    sqz.ON(); // 스퀴즈 ON/OFF
    delay(100);
  }
  if(btn == 'A')
  {
    sqz.OFF();  // 스퀴즈 ON/OFF
    delay(100);
  }
 // sqz.ON();  // 스퀴즈 업다운  
//  sqz.OFF();  // 스퀴즈 업다운
/*  
  right.ON();  // 제판 우측 
  right.OFF();  // 제판 우측 
#define DI2 37 // 풋 스위치
#define DI3 38 // 하강 실린더 센서 입력
#define DI4 39 // 제판 좌측 센서 ON/OFF
#define DI5 40 // 제판 우측 센서 ON/OFF
#define DI6 41

#define DO13 20  // 스퀴즈 ON/OFF
#define DO14 21  // 제판 우측 이동
#define DO15 22  // 제판 좌측 이동
#define DO16 23  
#define DO17 24
  left.ON();  //제판 좌측
  left.OFF();  //제판 좌측
*/
}
 
