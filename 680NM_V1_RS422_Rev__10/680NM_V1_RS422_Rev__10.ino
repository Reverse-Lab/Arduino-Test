
#include "ht16k33.h"
HT16K33 HT;

#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(28, 29, 30, 31, 32, 33, 34, 35, 27, 25, 26);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16

#include <FX1S.h>
//by program-plc.blogspot.com
#define FX1Sbaud 9600
#define FX1Sformat SERIAL_7E2
#define FX1Stimeout 1000

enum
{
  FX1SPACKET1,
  FX1SPACKET2,
  FX1SPACKET3,
  FX1SPACKET4,
  FX1SPACKET5,
  FX1STOTAL_NO_OF_PACKETS
};
FX1SPacket FX1Spackets[FX1STOTAL_NO_OF_PACKETS];

FX1SpacketPointer FX1Spacket1 = &FX1Spackets[FX1SPACKET1];
FX1SpacketPointer FX1Spacket2 = &FX1Spackets[FX1SPACKET2];
FX1SpacketPointer FX1Spacket3 = &FX1Spackets[FX1SPACKET3];
FX1SpacketPointer FX1Spacket4 = &FX1Spackets[FX1SPACKET4];
FX1SpacketPointer FX1Spacket5 = &FX1Spackets[FX1SPACKET5];

unsigned int FX1SreadD_1[3];
unsigned int FX1SreadD_2[3];
unsigned int FX1SreadD_3[3];
unsigned int FX1SreadD_4[2];
unsigned int FX1SreadD_5[1];

const int buzzer_pin=14;
const int lcdPSB_pin = 36;
unsigned long beepOnTime;
boolean beepFlag;

void beep() {
  digitalWrite(buzzer_pin, HIGH);
  beepOnTime = millis(); beepFlag = true;
}

void setup() {  
  Serial.begin(57600);
  //Serial1.begin(9600,SERIAL_7E2);
  FX1S_construct(FX1Spacket1, FX1S_READ_D, 0, 3, FX1SreadD_1);
  FX1S_construct(FX1Spacket2, FX1S_READ_D, 3, 3, FX1SreadD_2);
  FX1S_construct(FX1Spacket3, FX1S_READ_D, 6, 3, FX1SreadD_3);
  FX1S_construct(FX1Spacket4, FX1S_READ_D, 9, 2, FX1SreadD_4);
  FX1S_construct(FX1Spacket5, FX1S_READ_D, 16, 1, FX1SreadD_5);
  FX1S_configure(&Serial1, FX1Sbaud, FX1Sformat, FX1Stimeout, FX1Spackets, FX1STOTAL_NO_OF_PACKETS);
  
  pinMode (buzzer_pin, OUTPUT);
  pinMode (lcdPSB_pin, OUTPUT) ;
  digitalWrite (lcdPSB_pin, HIGH); // GLCD HIGH : Data Mode, LOW : SPI Mode
  HT.begin(0x70); 
  //delay (500);
}

int8_t key;
unsigned long press_Time; int8_t key_Flag = 0;
unsigned long lcd_Time = millis();

void loop() {  
  static uint8_t lastkey=0; 
  FX1S_update();
  unsigned int D0 = FX1SreadD_1[0]  ;unsigned int D1 = FX1SreadD_1[1] ; unsigned int D2 = FX1SreadD_1[2];
  unsigned int D3 = FX1SreadD_2[0]  ;unsigned int D4 = FX1SreadD_2[1] ; unsigned int D5 = FX1SreadD_2[2];
  unsigned int D6 = FX1SreadD_3[0]  ;unsigned int D7 = FX1SreadD_3[1] ; unsigned int D8 = FX1SreadD_3[2];
  unsigned int D9 = FX1SreadD_4[0]  ;unsigned int D10 = FX1SreadD_4[1]; unsigned int D16 = FX1SreadD_5[0];
  
    if (bitRead(D10, 0))  HT.setLedNow(39); else HT.clearLedNow(39); // 판들림
    if (bitRead(D10, 1))  HT.setLedNow(23); else HT.clearLedNow(23); //
    if (bitRead(D10, 2))  HT.setLedNow(22); else HT.clearLedNow(22); //
    if (bitRead(D10, 3))  HT.setLedNow(55); else HT.clearLedNow(55); // 뒤쪽위치 설정
    if (bitRead(D10, 4))  HT.setLedNow(21); else HT.clearLedNow(21); // 스퀴즈
    if (bitRead(D10, 5))  HT.setLedNow(38); else HT.clearLedNow(38); // Option
    if (bitRead(D10, 6))  HT.setLedNow(20); else HT.clearLedNow(20); // 수동
    if (bitRead(D10, 7))  HT.setLedNow(37); else HT.clearLedNow(37); // 잉크덮개
    if (bitRead(D10, 8))  HT.setLedNow(36); else HT.clearLedNow(36); // 진공흡착
    if (bitRead(D10, 9))  HT.setLedNow(52); else HT.clearLedNow(52); // 앞쪽위치 설정
    if (bitRead(D10, 10)) HT.setLedNow(19); else HT.clearLedNow(19);  // 자동
    if (bitRead(D10, 11)) HT.setLedNow(35); else HT.clearLedNow(35);  // 전인쇄
    if (bitRead(D10, 12)) HT.setLedNow(50); else HT.clearLedNow(50);  // 
    //if (bitRead(D10, 13)) HT.setLedNow(51); else HT.clearLedNow(51);  // 작업수량 지움
    if (bitRead(D10, 14)) HT.setLedNow(34); else HT.clearLedNow(34);  // 진공모터
    if (bitRead(D10, 15)) HT.setLedNow(18); else HT.clearLedNow(18);  // 반자동
  
  int d_D1 = D1*0.1; int f_D1 = D1-(d_D1*10); 
  int d_D4 = D4*0.1; int f_D4 = D4-(d_D4*10); 
  int d_D5 = D5*0.1; int f_D5 = D5-(d_D5*10); 
  int d_D6 = D6*0.1; int f_D6 = D6-(d_D6*10); // 진공 ON 시간
  int d_D16 = D16*0.1; int f_D16 = D16-(d_D16*10); // 진공 OFF 시간
  
  unsigned long reflash_Time = 1300;
  
  if (D9 == 1 || D9 == 2 || D9 == 4 ) {                // D9 : 페이지 선택
    if ((millis() - lcd_Time >= reflash_Time)) {    
    u8g.firstPage();  
    do {    
      
      u8g.setFont(u8g_font_fub17n);  //17Pixel
      u8g.setPrintPos( 62, 19);
      u8g.printf("%05d\n", D0);
      u8g.setFont(u8g_font_lucasfont_alternater); // 7Pixel
      u8g.drawStr( 9, 9, "WORK");
      u8g.drawStr( 1, 19, "COUNTER");
      u8g.drawStr( 1, 33, "AUTO CYCLE TIME");
      u8g.setPrintPos( 95, 33);
      u8g.printf("%03d\n",d_D1);u8g.print(".");u8g.printf("%01d\n",f_D1);u8g.print("s");
      u8g.drawStr( 1, 47, "FRONT");
      u8g.setPrintPos( 40, 47);
      u8g.printf("%02d\n", D2);u8g.print("%");
      u8g.drawStr( 72, 47, "REAR");
      u8g.setPrintPos( 106, 47);
      u8g.printf("%02d\n", D3);u8g.print("%");
      u8g.drawStr( 1, 62, "POSITION DATA");
      u8g.setPrintPos( 85, 62);
      u8g.printf("%03d\n",d_D4);u8g.print(".");u8g.printf("%01d\n",f_D4);u8g.print("mm");
      
      switch (D9) {
        case 1: u8g.drawStr( 88, 33, ">"); break;   // AUTO CYCLE TIME
        case 2: u8g.drawStr( 33, 47, ">"); break;   // FRONT
        case 4: u8g.drawStr( 100, 47, ">"); break;  // REAR
        default : break; }
      
      } while( u8g.nextPage() ); lcd_Time = millis();
    } 
  }
  if (D9 == 3080 || D9 == 3088 || D9 == 3104 || D9 == 3136 || D9 == 3200) {
    if((millis() - lcd_Time >= reflash_Time)) {
      u8g.firstPage();  
      do {
      u8g.setFont(u8g_font_lucasfont_alternater); // 7Pixel
      u8g.drawStr( 1, 9, "SQEEZE ON TIME");
      u8g.setPrintPos( 101, 9);
      u8g.printf("%02d\n",d_D5);u8g.print(".");u8g.printf("%01d\n",f_D5);u8g.print("s");
      // u8g.setPrintPos( 1, 16);
      // u8g.print("D9 : "); u8g.printf("%05d\n", D9);
      u8g.drawStr( 1, 25, "MODE SELECT");
      u8g.setPrintPos( 91, 25);
      u8g.printf("%01d\n", D7);u8g.print(" Mode");
      u8g.drawStr( 1, 36, "CYCLE MODE");
      u8g.setPrintPos( 91, 36);
      u8g.print(D8);u8g.print(" Cycle");
      u8g.drawStr( 1, 51, "VACUUM  ON TIME");
      u8g.setPrintPos( 101, 51);
      u8g.printf("%02d\n",d_D6);u8g.print(".");u8g.printf("%01d\n",f_D6);u8g.print("s");
      u8g.drawStr( 1, 62, "VACUUM OFF TIME");
      u8g.setPrintPos( 101, 62);
      u8g.printf("%02d\n",d_D16);u8g.print(".");u8g.printf("%01d\n",f_D16);u8g.print("s");
      
      switch (D9) {
        case 3080: u8g.drawStr( 94, 62, ">"); break;  // VACUUM OFF
        case 3088: u8g.drawStr( 94, 9, ">"); break;  // SQEEZE ON TIME
        case 3104: u8g.drawStr( 94, 51, ">"); break; // VACUUM  ON TIME
        case 3136: u8g.drawStr( 84, 25, ">"); break;  // MODE SELECT
        case 3200: u8g.drawStr( 84, 36, ">"); break; // CYCLE MODE
        default : break; }
        
      } while( u8g.nextPage() ); lcd_Time = millis();
    } 
  }
  
  unsigned long beepDelayTime = 200;
  if ((beepFlag) && (millis()-beepOnTime) >= beepDelayTime) {
    digitalWrite (buzzer_pin, LOW); beepFlag = false;
  }
  
  unsigned long delay_Time = 200;
  unsigned long current_Time = millis();
  
  if ((key_Flag >= 100) && ((current_Time - press_Time) >= delay_Time)) {
    switch (key_Flag) {
      case 100 : while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000017")); Serial1.flush(); key_Flag = 0; break;
      case 102 : while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202000019")); Serial1.flush(); key_Flag = 0; break;
      default : break;
    }
  }
  
  key = HT.readKey();
  if (key > 0) beep();  //============================ BEEP  
  if (key != 0) { // key is pressed
    if (key != lastkey) {
      switch (key) {
        case  6: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101020201001A")); Serial1.flush(); // 상승/하강
                 press_Time = millis(); key_Flag = 102; break;
         
        case  5: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002100018")); Serial1.flush();  // 앞쪽/뒤쪽
                 press_Time = millis(); key_Flag = 100;break;
        
        case  4: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000280001F")); Serial1.flush(); // 스퀴즈
                 press_Time = millis(); key_Flag = 100;break;
        
        case  3: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002002019")); Serial1.flush(); // 수동
                press_Time = millis(); key_Flag = 100;break;
        
        case  2: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000200401B")); Serial1.flush();  // 자동
                press_Time = millis(); key_Flag = 100; break;
        
        case  1: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000219")); Serial1.flush();  // 반자동
                press_Time = millis(); key_Flag = 100; break;
        
        case  19: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101020202001B")); Serial1.flush(); // 판들림
                  press_Time = millis(); key_Flag = 102; break;
        
        case  18: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002200019")); Serial1.flush();  // Option
                  press_Time = millis(); key_Flag = 100; break;
        
        case  17: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002010018")); Serial1.flush();  // 잉크덮개
                  press_Time = millis(); key_Flag = 100; break;
        
        case  16: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002001018")); Serial1.flush();  // 진공흡착
                  press_Time = millis(); key_Flag = 100; break;
        
        case  15: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000200801F")); Serial1.flush();  // 전인쇄/후인쇄
                  press_Time = millis(); key_Flag = 100; break;
        
        case  14: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000200041B")); Serial1.flush();  // 진공모터
                  press_Time = millis(); key_Flag = 100; break;
 
        case  36: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101020210001A")); Serial1.flush(); HT.setLedNow(59); break; // 커서 위
        case -36: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202000019")); Serial1.flush(); HT.clearLedNow(59); break;
  
        case  35: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101020220001B")); Serial1.flush(); HT.setLedNow(58); break; // 커서 아래
        case -35: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202000019")); Serial1.flush(); HT.clearLedNow(58); break; 
   
        case  34: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101020240001D")); Serial1.flush(); HT.setLedNow(57); break; // 바꾼값을 설정
        case -34: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202000019")); Serial1.flush(); HT.clearLedNow(57); break;

        case  33: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202800021")); Serial1.flush(); HT.setLedNow(56); break; // 다음화면 이동 
        case -33: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010202000019")); Serial1.flush(); HT.clearLedNow(56); break;
      
        case  32: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000208001F")); Serial1.flush();  // 뒤쪽위치 설정
                  press_Time = millis(); key_Flag = 100; break;
        
        case  31: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000240001B")); Serial1.flush(); HT.setLedNow(54); break; //뒤로(좌로)
        case -31: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000019")); Serial1.flush(); HT.clearLedNow(54); break;
       
        case  30: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002020019")); Serial1.flush(); HT.setLedNow(53); break; // 앞으로(우로)
        case -30: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000019")); Serial1.flush(); HT.clearLedNow(53); break;
        
        case  29: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000204001B")); Serial1.flush();  // 앞쪽위치 설정
                  press_Time = millis(); key_Flag = 100; break;
        
        case  28: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000118")); Serial1.flush(); HT.setLedNow(51); break; // 작업수량 지움
        case -28: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("1010002000019")); Serial1.flush(); HT.clearLedNow(51); break;
        
        case  27: while(!(UCSR1A&(1<<UDRE1))); Serial1.print(String("101000200081F")); Serial1.flush(); // 운전 준비
                  press_Time = millis(); key_Flag = 100; break;
  
        default : break; 
      } lastkey = key;
    } 
  }
}
