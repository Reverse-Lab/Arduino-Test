//https://gigglehd.com/gg/lifetech/4872763

#include <Keyboard.h>

 

typedef struct{

  byte Pin;

  byte Key;

  boolean Press;

} BUTTON;

 

void ButtonRead(BUTTON* Btn){

   int State = digitalRead(Btn->Pin);

   if(State == HIGH &&  State != Btn->Press){

     Btn->Press = State;

     Keyboard.release(Btn->Key);

   }else if(State == LOW && State != Btn->Press){

     Btn->Press = State;

     Keyboard.press(Btn->Key);

   }

 }

 

BUTTON Btn1 ={7,100,false}; // D

BUTTON Btn2 ={6,102,false}; // F

BUTTON Btn3      ={A3,106,false}; // J

BUTTON Btn4      ={A2,107,false}; // K

BUTTON Btn5     ={A1,108,false}; // L

 

void setup() {

  pinMode (Btn1.Pin,INPUT_PULLUP);

  pinMode (Btn2.Pin,INPUT_PULLUP);

  pinMode (Btn3.Pin,INPUT_PULLUP);

  pinMode (Btn4.Pin,INPUT_PULLUP);

  pinMode (Btn5.Pin,INPUT_PULLUP);

  Keyboard.begin();

}

 

void loop() {

  ButtonRead(&Btn1);

  ButtonRead(&Btn2);

  ButtonRead(&Btn3);

  ButtonRead(&Btn4);

  ButtonRead(&Btn5);

  delay(1);

}
//아두이노로 간단 자작 키보드(?) 만들기 - 생활 / 가젯 / 테크 - 기글하드웨어 : https://gigglehd.com/gg/?mid=lifetech&document_srl=4872763
