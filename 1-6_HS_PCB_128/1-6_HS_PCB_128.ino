// 한솔 PCB v1.3 PRG
//  2020-05-20


#include <Key.h>
#include <Keypad.h>

#include "Wire.h"

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_7segment matrix1 = Adafruit_7segment();


//----------------------------------------------------------------------//

#define footSW        37  //DI2  풋 스위치
#define updownSensor  38  //DI3 하강 실린더 센서 입력
#define rightSensor   39  //DI4 제판 좌측 센서 ON/OFF
#define leftSensor    40  //DI5 제판 우측 센서 ON/OFF

#define sqieezSol     20  //DO13  // 스퀴즈 ON/OFF
#define rightSol      21  //DO14  // 제판 우측 이동
#define leftSol       22  //DO15  // 제판 좌측 이동

#define manual_led    45  //LED15
#define harf_led      46  //LED16
#define auto_led      47  //LED17
#define jig_led       49  //LED19
#define pos_led       50  //LED20
#define shape_led     48  //LED18

#define Buz 25

int footState = 0;                   // 센서 상태 저장 변수
int mode;                        // 0:수동, 1:반자동, 2: 자동
int jig;                         // 0 자동지그 OFF, 1:자동지그 ON
int start_pos;                   // 0:전인쇄, 1:후인쇄 모드 저장 변수
int shape;                       // 0:곡면, 1:평면
int startAddr;                   // 0:정지, 1:작업 개시
int FND;
int CT;

//--------------DS12C887------------------

const int dscs=43;
const int dsas=52;
const int dsrw=42;
const int dsds=44;
const int dsirq=51;
//define bidirectional address-data bus(양방향 주소 버스 정의)

const int ad[8]={28,29,30,31,32,33,34,35};
//등록 A B C D 주소

#define regA 0x0A
#define regB 0x0B
#define regC 0x0C
#define regD 0x0D

//함수 선언
void bus2out();
void bus2in();
void write_ds(byte add,byte date);
byte read_ds(byte add);
int i; // DS12C887 Bit Address 지정용

const byte numRows = 7;
const byte numCols = 2;

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


void mode_update()
{
  switch (mode){
    case 0:
    digitalWrite (manual_led, HIGH);
    digitalWrite (harf_led, LOW);
    digitalWrite (auto_led, LOW);
    write_ds(0xE4, byte(mode));
    break;

    case 1:
    digitalWrite (manual_led, LOW);
    digitalWrite (harf_led, HIGH);
    digitalWrite (auto_led, LOW);
    write_ds(0xE4, byte(mode));
    break;

    case 2:
    digitalWrite (manual_led, LOW);
    digitalWrite (harf_led, LOW);
    digitalWrite (auto_led, HIGH);
    write_ds(0xE4, byte(mode));
    break;

    default:
    break;
  }
}

void jig_update()
{
  switch (jig){
    case 0:
    digitalWrite (jig_led, LOW);
    write_ds(0xE5, byte(jig));
    break;

    case 1:
    digitalWrite (jig_led, HIGH);
    write_ds(0xE5, byte(jig));
    break;

    default:
    break;
  }
}

void pos_update()
{
  switch (start_pos){
    case 0:
    digitalWrite (pos_led, LOW);
    write_ds(0xE6, byte(start_pos));
    break;

    case 1:
    digitalWrite (pos_led, HIGH);
    write_ds(0xE6, byte(start_pos));
    break;

    default:
    break;
  }
}

void shape_update()
{
  switch (shape){
    case 0:
    digitalWrite (shape_led, LOW);
    write_ds(0xE7, byte(shape));
    break;

    case 1:
    digitalWrite (shape_led, HIGH);
    write_ds(0xE7, byte(shape));
    break;

    default:
    break;
  }
}

void setup() {
  
  pinMode(footSW, INPUT);

  pinMode(Buz, OUTPUT);

  pinMode(manual_led,   OUTPUT);
  pinMode(harf_led,     OUTPUT);
  pinMode(auto_led,     OUTPUT);
  pinMode(jig_led,      OUTPUT);
  pinMode(pos_led,      OUTPUT);
  pinMode(shape_led,    OUTPUT);

  pinMode(dscs, OUTPUT);
  pinMode(dsas, OUTPUT);
  pinMode(dsrw, OUTPUT);
  pinMode(dsds, OUTPUT);
  pinMode(dsirq, OUTPUT);
  
//칩 초기화
//  write_ds(regA,0x20);      // 수정 발진기를 켜고 클럭을 계속 작동 시키려면 A 레지스터를 제어하십시오.
//  write_ds(regB,0x26);        //24 시간 모드, 데이터 바이너리 모드를 설정하고 알람 인터럽트를 활성화하십시오

  byte hiByte = read_ds(0xE0); // 카운터 read(주소)
  byte loByte = read_ds(0xE1); // 카운터 read(주소)
  FND = word(hiByte, loByte);

  byte ct_hiByte = read_ds(0xE2); // 카운터 read(주소)
  byte ct_loByte = read_ds(0xE3); // 카운터 read(주소)
  CT = word(ct_hiByte, ct_loByte);

  mode = read_ds(0xE4);
  jig = read_ds(0xE5);
  start_pos = read_ds(0xE6);
  shape = read_ds(0xE7);

  write_ds(0xEF, 0); // 작업개시 주소 초기화

  mode_update();
  jig_update();
  pos_update();
  shape_update();

  digitalWrite(Buz, HIGH);
  delay(40);
  digitalWrite(Buz, LOW);
  delay(10);
    
  //Serial.begin(115200);
  Wire.begin();
  matrix.begin(0x70);
  matrix1.begin(0x71);

  matrix.print(88888, DEC);
  matrix1.print(88888, DEC);
  matrix.writeDisplay();
  matrix1.writeDisplay();

  matrix.blinkRate(1);
  matrix1.blinkRate(1);
  delay(800);
  matrix.blinkRate(0);
  matrix1.blinkRate(0);
  delay(100);
    
    matrix.print(FND, DEC); 
    matrix.writeDisplay();
    boolean drawDots = true;
    matrix1.print(0, DEC); 
    matrix1.writeDisplay();
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();    
}

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

void homePos()
{
  if (start_pos == 1) {
    sqieez.OFF();
    left.OFF();
    right.ON();
  } else {
    sqieez.OFF();
    right.OFF();
    left.ON();
  }
}

void cycleStart()
{
  switch (mode){
    case 1:
      homePos();
      if (start_pos == 0) {
        sqieez.ON();
        left.OFF();
        right.ON();
        sqieez.OFF();
        right.OFF();
        left.ON();
      } else {
        sqieez.ON();
        right.OFF();
        left.ON();
        sqieez.OFF();
        left.OFF();
        right.ON();
      }
      FND++;
      write_ds(0xEF, 0); // 반자동 모드인 경우 작업개시 초기화
    break;

    case 2:
      homePos();
      if (start_pos == 0) {
        sqieez.ON();
        left.OFF();
        right.ON();
        sqieez.OFF();
        right.OFF();
        left.ON();
      } else {
        sqieez.ON();
        right.OFF();
        left.ON();
        sqieez.OFF();
        left.OFF();
        right.ON();
      }
      FND++;
    break;
    
    default:
    break;  
  }
}

void loop() {

    boolean drawDots = true;    
  
    matrix.print(FND, DEC); 
    matrix.writeDisplay();

    footState = digitalRead(footSW);

    if (footState == HIGH)
    {
      startAddr = read_ds(0xEF);
    }

    if (startAddr == 1)
    {
      mode_update();
      jig_update();
      pos_update();
      shape_update();

      if (mode != 0)
      {
        cycleStart();
      } else {
        homePos();
      }
    }

  char btn = myKeypad.getKey();
  if(btn != NO_KEY) {
    
    digitalWrite(Buz, HIGH);
    delay(40);
    digitalWrite(Buz, LOW);
    delay(10);
    byte hiByte = read_ds(0xE0); // read(주소)
    byte loByte = read_ds(0xE1); // read(주소)
    FND = word(hiByte, loByte);
    FND++;
    delay(10);
  }
  set_count();
  
  switch(btn) {
    case '1':
      left.OFF();
      right.ON(); // 
    break;

    case 'A':
      right.OFF();
      left.ON(); // 스퀴즈 ON/OFF
    break;

    case '2':
      sqieez.ON(); // 스퀴즈 ON/OFF
    break;

    case 'B':
      sqieez.OFF(); // 스퀴즈 ON/OFF
    break;

    case '3':
      mode = 0; // 수동모드
      mode_update();
    break;

    case '4':
      mode = 1; // 반자동모드
      mode_update();
    break;

    case '5':
      mode = 2; // 반자동모드
      mode_update();
    break;

    case 'D':
      jig = 1-jig; // 0:자동지그 OFF, 1:자동지그 ON
      jig_update();
    break;
    
    case 'E':
      start_pos = 1-start_pos; // 0:전인쇄, 1:후인쇄모드
      pos_update();
    break;

    case 'F':
      shape = 1-shape; // 0:곡면, 1:평면
      shape_update();
    break;
    
    case '6':
    CT += 100;
    if (CT>20000) CT=20000;
    set_time();
    
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();
    break;
     
    case '7':
    CT -= 100;
    if (CT<100) CT=99;
    set_time();
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();
    break;
    
    case 'G':
    FND = 0;
    break;
    
    default:
    break;  
  }
}
