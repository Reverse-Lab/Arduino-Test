#include <Key.h>
#include <Keypad.h>

#define footSW        23  //DI2  풋 스위치
#define updownSensor  22  //DI3 하강 실린더 센서 입력
#define rightSensor   24  //DI4 제판 좌측 센서 ON/OFF
#define leftSensor    25  //DI5 제판 우측 센서 ON/OFF

#define sqieezSol     53  //DO13  // 스퀴즈 ON/OFF
#define rightSol      52  //DO14  // 제판 우측 이동
#define leftSol       50  //DO15  // 제판 좌측 이동

#define Buz 12

int footState;                    // 센서 상태 저장 변수
int autoState;                    // 자동 수동 모드 저장 변수
int fowardState;                  // 전인쇄, 후인쇄 모드 저장 변수

const byte numRows = 2;
const byte numCols = 7;
int FND;

char keymap[numRows][numCols]={
  {'1', 'B', 'C', 'D', 'E', 'F', 'G'},
  {'A', '2', '3', '4', '5', '6', '7'}
};
byte rowPins[numRows] = {28, 29};
byte colPins[numCols] = {63, 64, 65, 66, 67, 68, 69};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


class Flasher
{
    int solPin;         // 출력 핀 이름
    int sensorPin;      // 센서 핀
    int checkTime;      // 인터벌 대기시간
 
    int solState;       // 출력 상태 변수
    int sensorState;    // 센서 상태 변수

  public:
  Flasher(int sol, int sens, int check)
  {
    solPin = sol;
    pinMode(solPin, OUTPUT);    

    sensorPin = sens;
    pinMode(sensorPin, INPUT);

    checkTime = check;  // 센서 감지 후 대기 시간
  }
 
  void ON()
  {
    sensorState = digitalRead(sensorPin);
    delay(checkTime);
      if(sensorState == LOW)
      {
        solState = HIGH;                // 솔밸브 ON
        digitalWrite(solPin, solState);  
        delay(checkTime);
      }
  }
  
  void OFF()
  {
    sensorState = digitalRead(sensorPin);
    delay(checkTime);
      if(sensorState == HIGH)
      {
        solState = LOW;                 // 솔밸브 OFF
        digitalWrite(solPin, solState);  
        delay(checkTime);
      }
  }
};

Flasher sqieez(sqieezSol, updownSensor, 10);    // (출력핀, 센서핀, 타입아웃)
Flasher right(rightSol, rightSensor, 10);       // 제판 우측 이동 ON/OFF
Flasher left(leftSol, leftSensor, 10);          // 제판 좌측 이동 ON/OFF

void setup() {
  
  pinMode(28, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP); // 키패드 풀업
  
  pinMode(footSW, INPUT);

  pinMode(Buz, OUTPUT);

}

void loop() {
  
  char btn = myKeypad.getKey();           // 키패드 상태 저장 변수  
  if(btn != NO_KEY) {
    
    digitalWrite(Buz, HIGH);
    delay(40);
    digitalWrite(Buz, LOW);
    delay(10);

  }

  switch(btn) {
    case '1':
      right.ON(); // 스퀴즈 ON/OFF
    break;

    case 'A':
      right.OFF(); // 스퀴즈 ON/OFF
    break;

    default:
    break;  
  }
}
 
