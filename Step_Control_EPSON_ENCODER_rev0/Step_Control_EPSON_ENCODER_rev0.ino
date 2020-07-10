//엡손 엔코더 연동 스텝모터 제어
#define pha_a 2  // pin 2 on UNO is connected to phase a encoder
#define pha_b 3   // pin 3 on UNO is connected to phase b encoder
#define step_CLK 9 // pin 9 on UNO is connected to the CLK pin of TB6560
#define dir_CW 10   // pin 10 on UNO is connected to the CLK pin of TB6560
#define pin_EN 11       // pin 11 on UNO is connected to the CLK pin of TB6560
#define pin_MS1 5     // pin 5 on UNO is connected to the Micro Step 1 of TB6560
#define pin_MS2 6     // pin 6 on UNO is connected to the Micro Step 2 of TB6560
#define pin_MS3 7     // pin 7 on UNO is connected to the Micro Step 3 of TB6560

static boolean rotating=false;
volatile long motor_position,encoder;
boolean A_set = false;
boolean B_set = false;
int motor_dealy = 2000;  // Mocroseconds

void setup() {
      pinMode(step_CLK,OUTPUT);
      pinMode(dir_CW, OUTPUT);
      pinMode(pin_EN, OUTPUT);
      pinMode(pin_MS1, OUTPUT);
      pinMode(pin_MS2, OUTPUT);
      pinMode(pin_MS3, OUTPUT);
      digitalWrite(pin_EN, LOW);
      delay(1);
      digitalWrite(pin_MS1, HIGH);  // 1/2 스텝 : 2500us
      digitalWrite(pin_MS2, LOW);  // 1/4 step
      digitalWrite(pin_MS3, LOW);  //

      pinMode(pha_a,INPUT);
      pinMode(pha_b,INPUT);

      digitalWrite(pha_a, HIGH);
      digitalWrite(pha_b, HIGH);
      // encoder pin on interrupt 0 (pin 2)
      attachInterrupt (0, encoderPinChangeA, CHANGE);
      // encoder pin on interrupt 1 (pin 3)
      attachInterrupt(1, encoderPinChangeB, CHANGE);
      encoder = 0;
         Serial.begin (115200);
}
  void encoderPinChangeA()                
{
      if (digitalRead(pha_a)== digitalRead (pha_b))
      {
        encoder--;
      }
        else
      {
        encoder++;
      }
}  

void encoderPinChangeB()
{
      if (digitalRead(pha_a)!= digitalRead (pha_b))
      {
        encoder--;
      }
      else
      {
        encoder++;
      }
}

void loop ()   {
  
      // run when detecting notion

      if ( rotating ) delayMicroseconds (20);
      if (encoder >0)
      {
      digitalWrite(dir_CW, HIGH);
      
      digitalWrite(step_CLK,HIGH);
      digitalWrite(step_CLK,LOW);
 
      _delay_us(motor_dealy);
      motor_position++;
      Serial.println (motor_position);
      encoder =0;
      }
      else if (encoder <0)
      {
       digitalWrite(dir_CW, LOW);
       digitalWrite(step_CLK,HIGH);
       digitalWrite(step_CLK,LOW);
       
       _delay_us(motor_dealy);
        motor_position--;
        Serial.println (motor_position);
       encoder =0;
   }
}
