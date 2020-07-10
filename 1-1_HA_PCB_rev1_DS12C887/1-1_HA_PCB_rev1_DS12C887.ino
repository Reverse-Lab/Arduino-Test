#include <Key.h>
#include <Keypad.h>

#include "Wire.h"

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_7segment matrix1 = Adafruit_7segment();
#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif


//----------------------------------------------------------------------//
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

#define LED1 45
#define LED2 46
#define LED3 47
#define LED4 48
#define LED5 49
#define LED6 50

#define Buz 25

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

int FND;
int CT;

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
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time
 
    // These maintain the current state
    int ledState;                     // ledState used to set the LED
    unsigned long previousMillis;      // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);     
      
    OnTime = on;
    OffTime = off;
    
    ledState = LOW; 
    previousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to change the state of the LED
    // 출력 시간 계산을 위한 함수 선언
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // 현재 시간을 기억 (Remember the time)
      digitalWrite(ledPin, ledState);  // 출력 OFF (Update the actual LED)
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // 현재 시간을 기억 (Remember the time)
      digitalWrite(ledPin, ledState);      // 출력 ON (Update the actual LED)
    }
  }
};

//------------------------------DS12C887 
void bus2out(){
  for(i=0;i<8;i++)
  {
    pinMode(ad[i], OUTPUT);
  }
}
void bus2in(){
  for(i=0;i<8;i++)
  {
    pinMode(ad[i], INPUT);
  }
}

void write_ds(byte add,byte date)    //칩 쓰기 명령
{
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  
  //set address on bus(주소 쓰기)
  for(i=0;i<8;i++)
    {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsrw,0);
  
  //set byte to write(데이터 쓰기)
  for(i=0;i<8;i++)
    {digitalWrite(ad[i],bitRead(date,i));}
  digitalWrite(dsrw,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
}

byte read_ds(byte add)         //칩 읽기 명령
{
  byte readb=0;
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  
  //set address on bus(주소 쓰기)
  for(i=0;i<8;i++)
    {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsds,0);
  
  bus2in(); // Input Mode 전환
  for(i=0;i<8;i++){digitalWrite(ad[i],1);}
  readb = digitalRead(ad[0])|(digitalRead(ad[1])<<1)|(digitalRead(ad[2])<<2)|(digitalRead(ad[3])<<3)|(digitalRead(ad[4])<<4)|(digitalRead(ad[5])<<5)|(digitalRead(ad[6])<<6)|(digitalRead(ad[7])<<7);
  digitalWrite(dsds,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
  return readb;
}

void set_count()  // DS12C887에 카운터 쓰기
{
  write_ds(0xE0, highByte(FND));
  write_ds(0xE1, lowByte(FND));
}

void set_time()
{
  write_ds(0xE2, highByte(CT));
  write_ds(0xE3, lowByte(CT));
}

Flasher sqz(20, 50, 50);  // 스퀴즈 ON/OFF
Flasher right(21, 50, 50);  // 제판 우측 이동
Flasher left(22, 50, 50);  // 제판 좌측 이동
Flasher led4(23, 50, 50);
Flasher led5(24, 50, 50);
Flasher AC_O1(14, 50, 50);
Flasher AC_O2(15, 50, 50);
 
void setup() {
  
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP); // 키패드 풀업
  
  pinMode(DI2, INPUT);
  pinMode(DI3, INPUT); // 업다운 실린더
  pinMode(DI4, INPUT); // 좌측 실린더
  pinMode(DI5, INPUT); // 우측 실린더
  pinMode(DI6, INPUT);

  pinMode(DO13, OUTPUT);
  pinMode(DO14, OUTPUT); 
  pinMode(DO15, OUTPUT);
  pinMode(DO17, OUTPUT);
  pinMode(DO16, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  pinMode(Buz, OUTPUT);

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

  digitalWrite(Buz, HIGH);
  delay(40);
  digitalWrite(Buz, LOW);
  delay(10);
    
    
  //Serial.begin(115200);
  Wire.begin();
  matrix.begin(0x70);
  matrix1.begin(0x71);
  
  uint8_t counter=0;  
  for (int j=0; j<8; j++) {
  for (uint8_t i=0; i<8; i++) {
    matrix.displaybuffer[i] = _BV((counter+i) % 16) | _BV((counter+i+8) % 16)  ;
    matrix1.displaybuffer[i] = _BV((counter+i) % 16) | _BV((counter+i+8) % 16)  ;
    }
  
    matrix.writeDisplay();
    matrix1.writeDisplay();
    delay(50);
    counter++;
    if (counter >= 16) counter = 0;
    }
    
    matrix.print(FND, DEC); 
    matrix.writeDisplay();
    boolean drawDots = true;
    matrix1.print(0, DEC); 
    matrix1.writeDisplay();
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();
}

void loop() {
  /*
  sqz.Update();  // 스퀴즈 업다운
  right.Update();  // 제판 우측 
  left.Update();  //제판 좌측
  led4.Update();
  led5.Update();
  AC_O1.Update();
  AC_O2.Update();
  */
    matrix.print(FND, DEC); 
    matrix.writeDisplay();

    

  char btn = myKeypad.getKey();
  if(btn != NO_KEY) {
    
    digitalWrite(Buz, HIGH);
    delay(40);
    digitalWrite(Buz, LOW);
    delay(10);
    
   // Serial.println("Reading FND from DS12C887...");
    byte hiByte = read_ds(0xE0); // read(주소)
    byte loByte = read_ds(0xE1); // read(주소)
    FND = word(hiByte, loByte);
    
   // Serial.print("FND Number : "); Serial.println(FND);
    FND++;
    delay(10);
  }
  set_count();
 
 if(btn == '1')
  {
    digitalWrite(LED1, HIGH);
    sqz.Update(); // 스퀴즈 ON/OFF
    delay(100);
  }
  if(btn == 'A')
  {
    digitalWrite(LED1, LOW);
    sqz.Update();  // 스퀴즈 ON/OFF
    delay(100);
  }

 if(btn == '2')
  {
    digitalWrite(LED2, HIGH);
    right.Update(); // 제판 우측 이동
    delay(100);
  }
  if(btn == 'B')
  {
    digitalWrite(LED2, LOW);
    right.Update();  // 제판 우측 이동
    delay(100);
  }

 if(btn == '3')
  {
    digitalWrite(LED3, HIGH);
    left.Update();  // 제판 좌측 이동
    delay(100);
  }
  if(btn == 'C')
  {
    digitalWrite(LED3, LOW);
    left.Update();  // 제판 좌측 이동
    delay(100);
  }

 if(btn == '4')
  {
    digitalWrite(LED4, HIGH);
    led4.Update();
    delay(100);
  }
  if(btn == 'D')
  {
    digitalWrite(LED4, LOW);
    led4.Update();
    delay(100);
  }

 if(btn == '5')
  {
    digitalWrite(LED5, HIGH);
    led5.Update();
    delay(100);
  }
  if(btn == 'E')
  {
    digitalWrite(LED5, LOW);
    led5.Update();
    delay(100);
  }

  if(btn == '6')              // 시간 증가
  {
    CT += 100;
    set_time();
    
    byte ct_hiByte = read_ds(0xE2); // 싸이클타임 read(주소)
    byte ct_loByte = read_ds(0xE3); // 싸이클타임 read(주소)
    CT = word(ct_hiByte, ct_loByte);
    boolean drawDots = true;
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();
    delay(10);
  }
  if(btn == 'F')
  {
    digitalWrite(LED6, LOW);
    delay(100);
  }
  if(btn == '7')            // 시간 감소
  {
    CT -= 100;
    set_time();
    
    byte ct_hiByte = read_ds(0xE2); // 싸이클타임 read(주소)
    byte ct_loByte = read_ds(0xE3); // 싸이클타임 read(주소)
    CT = word(ct_hiByte, ct_loByte);
    boolean drawDots = true;
    matrix1.writeDigitNum(1, (CT / 1000) % 10, drawDots);
    matrix1.writeDigitNum(2, (CT / 100) % 10);
    matrix1.writeDisplay();
    delay(10);
  }
  if(btn == 'G')
  {
    FND = 0;
    delay(10);
  }
}
