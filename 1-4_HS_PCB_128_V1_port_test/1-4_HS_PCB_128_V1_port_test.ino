#include <Key.h>
#include <Keypad.h>

#include "Wire.h"

#define HT16K33_ADDRESS         0x70
#define HT16K33_ON              0x21  // Commands
#define HT16K33_STANDBY         0x20
#define HT16K33_DISPLAYON       0x81
#define HT16K33_DISPLAYOFF      0x80
#define HT16K33_BLINKON         0x85 // Blink is off (00), 2 Hz (01), 1 Hz (10), or 0.5 Hz (11) for bits (21) 
#define HT16K33_BLINKOFF        0x81
#define HT16K33_DIM             0xE0 | 0x08 // Set dim from 0x00 (1/16th duty ccycle) to 0x0F (16/16 duty cycle)

//#define COM0 0x00
//#define COM1 0x02
//#define COM2 0x04
//#define COM3 0x06
//#define COM4 0x08
//#define COM5 0x0A
//#define COM6 0x0C
//#define COM7 0x0E
//[출처] ht16k33|작성자 Revyture

static const byte numberTable[] =
{
  0x3F, // 0 = 0011 1111
  0x06, // 1 = 0000 0110
  0x5B, // 2 = 0101 1011
  0x4F, // 3 = 0100 1111
  0x66, // 4 = 0110 0110
  0x6D, // 5 = 0110 1101
  0x7D, // 6 = 0111 1101
  0x07, // 7 = 0000 0111
  0x7F, // 8 = 0111 1111
  0x6F, // 9 = 0110 1111
  0x80, // decimal point
  0x00, // blank  1000 0000
  0x40, // minus sign 0100 0000
  0x77,  // A = 13
  0x7F,  // B = 14
  0x2D,  // C = 15
  0x6F,  // D = 16
  0x3D,  // E = 17
  0x35,  // F = 18
  0x5F,  // G = 19
  0x76,   // H = 20
  0x42,   // I = 21
  0x6A,   // J = 22
  0x00,   // No K!
  0x2C,   // L = 24
  0x00,   // No M!
  0x67,   // N = 26
  0x6F,   // O = 27
  0x37,   // P = 28
  0x57,   // Q = 29
  0x77,   // R = 30
  0x5D    // S = 31
};

#define display1 1
#define display2 2
#define display3 3
#define display4 4
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

void writeInteger(uint8_t dsply, int data)
{
  char string[10] = "";                             // 숫자를 담을 문자 배열을 정의
  itoa(data, string);                               // 표시 할 정수의 ASCII 문자열 표현을 가져옵니다.
  uint8_t length = strlen(string);                  // 문자열의 길이를 얻는다; 정수 자릿수
  uint8_t blanks = 4 - length;                      // 우리는 얼마나 많은 공백이 있습니까?
  
  if (length > 4) return;                           // 길이가 4 자리보다 길면 4 자리 디스플레이에 표시 할 수 없습니다!
  for (uint8_t digit = 0; digit < blanks; digit++)  // 각 숫자를 스크롤하여 디스플레이에 쓸 내용을 결정합니다.
  {
      writeDigit(dsply, digit + 1, 11, 0);          // 공백이있는 자리를 지우십시오
  }

  for (uint8_t digit = 0; digit < 4; digit++)       // 각 숫자를 스크롤하여 디스플레이에 쓸 내용을 결정합니다.
  {
     char ch = string[digit];                      // 다음 문자열 세그먼트의 ASCII 문자를 얻습니다.
      if (ch == '-') {
      writeDigit(dsply, digit + 1 + blanks, 12, 0); // 음수 부호가 필요한지 확인
      } 
      else {                                        //문자는 숫자 여야합니다
      ch -= '0';                                    // 정수로 변환
      writeDigit(dsply, digit + 1 + blanks, ch, 0); // 그것을 디스플레이에 쓰십시오; 정수를 정당화하십시오
      } 
  }
}

void writeFloat(uint8_t dsply, float data, uint8_t dp)
{
  char string[10] = "";  // 숫자를 담을 문자 배열을 정의
  int datanew = 0;

  switch (dp)
  {
    case 0:
    datanew = (int )(1.*data);
    break;

    case 1:
    datanew = (int )(10.*data);
    break;

    case 2:
    datanew = (int )(100.*data);
    break;

    case 3:
    datanew = (int )(1000.*data);
    break;
   }

  itoa(datanew, string);                                    // get ascii character string representation of the integer to be displayed
  uint8_t length = strlen(string);                          // get the length of the string; number of digits in integer
  uint8_t blanks = 4 - length;                              // how many blanks do we have?

  if (length > 4) return;                                   // if length greater than 4 digits we can't display on a four-digit display!
                                                            // scroll through each digit to determine what to write to the display
  for (uint8_t digit = 0; digit < blanks; digit++)          // first the blanks
  {
          if( (digit + 1) == (4 - dp) ) {                   // 공백이 소수점과 일치하는 특수한 경우 처리
            writeDigit(dsply, digit + 1, 0, 0x80);          // 소수점 이하 자릿수 앞에 0을 추가
          }
          else {
            writeDigit(dsply, digit + 1, 11, 0x00);         // 그렇지 않으면 공백이있는 곳이면 자리를 비 웁니다
          }
  }

  for (uint8_t digit = 0; digit < 4; digit++)               // 이제 디스플레이에 쓸 내용을 결정하는 문자
  {
      char ch = string[digit];                              // 다음 문자열 세그먼트의 ASCII 문자를 얻습니다.
      if (ch == '-') {
        if((digit + 1 + blanks) == (4 - dp) ) {
          writeDigit(dsply, digit + 1 + blanks,  0, 0x80);  // 음수 부호가 필요한지 확인하고 소수점을 추가하십시오.
          writeDigit(dsply, digit + 0 + blanks, 12, 0x00);  // 선행 0을 추가
        }
        else {
          writeDigit(dsply, digit + 1 + blanks, 12, 0x00);  // 소수점이없는 음수 부호가 필요한지 확인
        }
      }
      else  {                                               // character must be a digit
        ch -= '0';                                          // convert it to an integer
        if((digit + 1 + blanks) == (4 - dp) ) {
          writeDigit(dsply, digit + 1 + blanks, ch, 0x80);  // write it to the display with decimal point; right justify the integer
        } 
        else {
          writeDigit(dsply, digit + 1 + blanks, ch, 0x00);  // write it to the display; right justify the integer
        } 
     }
  }
}

void writeDigit(uint8_t dsply, uint8_t digit, uint8_t data, uint8_t dp) 
{
  if(dsply == 1) {
    digit = (digit - 1)*2 + 0; 
  } 
  if(dsply == 2) {
    digit = (digit - 1)*2 + 8 ;
  }
  if(dsply == 3) {
    digit = (digit - 1)*2 + 1;
  }
  if(dsply == 4) {
    digit = (digit - 1)*2 + 9;
  }
  writeByte(HT16K33_ADDRESS, digit, numberTable[data] | dp);
}

void clearDsplay(int dsply) 
{
  for(int i = 0; i < 8; i++)  {
  writeDigit(dsply, i, 11, 0);  // 클리어, 위의 숫자 표에서 11은 비어 있음
  }
}

void initHT16K33()
{
  writeCommand(HT16K33_ADDRESS, HT16K33_ON);         // Turn on system oscillator
  writeCommand(HT16K33_ADDRESS, HT16K33_DISPLAYON);  // Display on
  writeCommand(HT16K33_ADDRESS, HT16K33_DIM);        // Set brightness
}

void blinkHT16K33(int time) 
{
  writeCommand(HT16K33_ADDRESS, HT16K33_BLINKON);  // Turn on blink
  delay(500*time);
  writeCommand(HT16K33_ADDRESS, HT16K33_BLINKOFF);  // Turn on blink
}

 /* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
         i = 0;
       do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
       } while ((n /= 10) > 0);     /* delete it */

     if (sign < 0)
         s[i++] = '-';
         s[i] = '\0';
         reverse(s);
} 

void reverse(char s[])
 {
     int i, j;
     char c;
 
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 
// Wire.h read and write protocols
void writeCommand(uint8_t address, uint8_t command)
{
 Wire.beginTransmission(address);  // Initialize the Tx buffer
 Wire.write(command);              // Put command in Tx buffer
 Wire.endTransmission();           // Send the Tx buffer
}

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
 Wire.beginTransmission(address);  // Initialize the Tx buffer
 Wire.write(subAddress);           // Put slave register address in Tx buffer
 Wire.write(data);                 // Put data in Tx buffer
 Wire.endTransmission();           // Send the Tx buffer
}

uint8_t readByte(uint8_t address, uint8_t subAddress)
{
 uint8_t data; // `data` will store the register data  
 Wire.beginTransmission(address);         // Initialize the Tx buffer
 Wire.write(subAddress);                  // Put slave register address in Tx buffer
 Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
 Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address 
 data = Wire.read();                      // Fill Rx buffer with result
 return data;                             // Return data read from slave register
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
 Wire.beginTransmission(address);   // Initialize the Tx buffer
 Wire.write(subAddress);            // Put slave register address in Tx buffer
 Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
 uint8_t i = 0;

 Wire.requestFrom(address, count);  // Read bytes from slave register address 

    while (Wire.available()) {
    dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
} 

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

Flasher sqz(20, 2550, 2000);  // 스퀴즈 ON/OFF
Flasher right(21, 3550, 2000);  // 제판 우측 이동
Flasher left(22, 1550, 2000);  // 제판 좌측 이동
Flasher led4(23, 1800, 2000);
Flasher led5(24, 4000, 2000);
 
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

  byte hiByte = read_ds(0xE0); // read(주소)
  byte loByte = read_ds(0xE1); // read(주소)
  FND = word(hiByte, loByte);
    
  //Serial.begin(115200);
  Wire.begin();

  digitalWrite(Buz, HIGH);
  delay(40);
  digitalWrite(Buz, LOW);
  delay(10);
  
  initHT16K33();  // initialize display
  clearDsplay(display1);  // clear display 1
  //clearDsplay(display2);  // clear display 2

  //writeInteger(display1, 8888); // Test display, turn on all segments of display 1
  writeDigit(display1, 1, 12, 64);   // - 표시 출력
  writeDigit(display1, 2, 12, 64);   // 네번째값은 각 세그면트 16진수 값
  writeDigit(display1, 3, 12, 64);
  writeDigit(display1, 4, 12, 64); 
  blinkHT16K33(2); // Blink for 2 seconds
  writeInteger(display1, FND);

}

void loop() {

  sqz.Update();  // 스퀴즈 업다운
  right.Update();  // 제판 우측 
  left.Update();  //제판 좌측
  led4.Update();
  led5.Update();


  FND++;
      
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
    delay(100);
  }
  set_count();
  writeInteger(display1, FND);   // write integer count to display 1
  
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

  if(btn == '6')
  {
    digitalWrite(LED6, HIGH);
    delay(100);
  }
  if(btn == 'F')
  {
    digitalWrite(LED6, LOW);
    delay(100);
  }
  if(btn == '7')
  {
    FND = 1;
    delay(10);
  }
}
