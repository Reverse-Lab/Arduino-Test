 
// 자료를 재정의합니다.
#define u8 volatile unsigned char
#define u16 volatile unsigned short
#define bounceTimer 50
 
// 상수를 정의합니다.
typedef enum{DIGIT_4=0XA4,DIGIT_5=0XA5} TYPE_DIGIT;
typedef enum{FL_ALL=0XF0,FL_STOP=0XFF,FL_N1=0XF1,FL_N2=0XF2,FL_N3=0XF3,FL_N4=0XF4,FL_N5=0XF5} TYPE_FLASH;
typedef enum{RADIX_16=0XFA,RADIX_10=0XFB} TYPE_RADIX;
typedef enum{DOT_FLOFF=0XDF,DOT_OFF=0XD0,DOT_ON1=0XD1,DOT_ON2=0XD2,DOT_ON3=0XD3,DOT_ON4=0XD4,DOT_ON5=0XD5,DOT_FL1=0XD6,DOT_FL2=0XD7,DOT_FL3=0XD8,DOT_FL4=0XD9,DOT_FL5=0XDA} TYPE_DOT;
 
// SGN모모듈을 다루는 용도로 사용되는 사용자 정의 함수의 선언부입니다.
void writeRS232(const u8* dS,const u8 len);
void writeData(u8 channel,u8 index, u8 data);
void writeValue(u8 chnnel, u16 data, TYPE_RADIX radix); 
void setDigit(u8 channel,TYPE_DIGIT digit);
void setFlash(u8 channel, TYPE_FLASH flash);
void setDot(u8 channel, TYPE_DOT dot);

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
int btnState = 0;
 
// SGN에 띄울 값을 저장하는 변수입니다.
u16 value=0;
 
void setup() {  
  // put your setup code here, to run once:
  Serial.begin(9600);  // RS232 채널 1 포트를 개방합니다. ( 통신속도 : 9600 Baudrate , data : 8bit, Parity : None, Stop : 1bit)
  while(!Serial);      // RS232 채널 1 포트가 정상적으로 동작할 수 있을때까지 대기합니다.
  delay(1000);           // 안정성을위해 1초정도 대기합니다.
  
 
//   SGN 모듈의 출력설정부분입니다.
  writeValue(2,12345,RADIX_10);     
  setFlash(2,FL_ALL);               
  setDot(2,DOT_ON1);
  setDot(2,DOT_ON2);
  setDot(2,DOT_ON3);
  setDot(2,DOT_ON4);
  setDot(2,DOT_ON5); 

  pinMode(buttonPin, INPUT);

  writeValue(0,0,RADIX_10); // ID가 0인 SGN 모듈에 value의 값을 10진수 "0"으로 초기화
  
}
 
void loop() {
  // put your main code here, to run repeatedly:
  // writeValue(0,value++,RADIX_16); // ID가 0인 SGN 모듈에 변수 value의 값을 16진수 형식으로 출력합니다.

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) 
  {
    if (btnState == 0)
    {
    delay(10);
    btnState = 1;
    }
  }

  if (buttonState == LOW)
  {
    if (btnState == 1)
    {
    value += 1;
    writeValue(0,value,RADIX_10); // ID가 1인 SGN 모듈에 변수 value의 값을 10진수 형식으로 출력합니다.
    delay(10);
    btnState = 0;
    }
  }
}
 
 
// 사용자 정의 함수 정의부분입니다.
 
void writeData(u8 channel,u8 index, u8 data)
{
  u8 tmpD[3];
  tmpD[0] = 0xe0 + channel;
  tmpD[1] = index;
  tmpD[2] = data;
  writeRS232(tmpD,3);
}
void writeValue(u8 channel,u16 data,TYPE_RADIX radix)
{
  u8 tmpD[4];
  tmpD[0] = 0xe0 + channel;
  tmpD[1] = radix;
  tmpD[2] = data >> 8;
  tmpD[3] = data;
 
  writeRS232(tmpD,4);
}
void setDigit(u8 channel,TYPE_DIGIT digit)
{
  u8 tmpD[2];
  tmpD[0] = 0xe0 + channel;
  tmpD[1] = digit;
  writeRS232(tmpD,2);
}
void setFlash(u8 channel, TYPE_FLASH flash)
{
  u8 tmpD[2];
  tmpD[0] = 0xe0 + channel;
  tmpD[1] = flash;
  writeRS232(tmpD,2);
}
 
void setDot(u8 channel, TYPE_DOT dot)
{
  u8 tmpD[2];
  tmpD[0] = 0xe0 + channel;
  tmpD[1] = dot;
  writeRS232(tmpD,2);
}
void writeRS232(const u8* dS,u8 len)
{
  u8 i;
  for(i=0;i<len;i++)
  {
    Serial.write(*dS++);
  }
}
//[출처] [FA-DUINO]FA-DUINO에는 FND를 사용하시려면??? (SGN시리즈 제품 사용하기)|작성자 컴파일제품스터디
