#define encoder_a 2 //keep this on and interrupt pin
#define encoder_b 3 //keep this on and interrupt pin 
#define motor_step 9 //can be any pin 
#define motor_direction 10 //can be any pin 
#define pin_EN 11       // pin 11 on UNO is connected to the CLK pin of TB6560
#define pin_MS1 5     // pin 5 on UNO is connected to the Micro Step 1 of TB6560
#define pin_MS2 6     // pin 6 on UNO is connected to the Micro Step 2 of TB6560
#define pin_MS3 7     // pin 7 on UNO is connected to the Micro Step 3 of TB6560

volatile long motor_position, encoder; 
unsigned long WaitReverse = 50; //<<< 
unsigned long WaitForward = 50; //<<< 
unsigned long StartReverse = micros(); //<<< 
unsigned long StartForward = micros(); //<<< 
unsigned long microsNow = micros(); //<<< 
unsigned long microsDT = micros(); //<<< 

void setup () { //set up the various outputs 
  pinMode(motor_step, OUTPUT); 
  pinMode(motor_direction, OUTPUT); // then the encoder inputs 
  pinMode(pin_EN, OUTPUT);
  digitalWrite(pin_EN, LOW);
  delay(1);
  pinMode(pin_MS1, OUTPUT);
  pinMode(pin_MS2, OUTPUT);
  pinMode(pin_MS3, OUTPUT);
 
  digitalWrite(pin_MS1, LOW);  // 1/2 스텝 : 2500us
  digitalWrite(pin_MS2, LOW);  // 1/4 step
  digitalWrite(pin_MS3, LOW);  //

  pinMode(encoder_a, INPUT); 
  pinMode(encoder_b, INPUT); // enable pullup as we are using an open collector encoder 
  digitalWrite(encoder_a, HIGH); 
  digitalWrite(encoder_b, HIGH); // encoder pin on interrupt 0 (pin 2) 
  attachInterrupt(0, encoderPinChangeA, CHANGE); // encoder pin on interrupt 1 (pin 3) 
  attachInterrupt(1, encoderPinChangeB, CHANGE); 
  encoder = 0; //reseet the encoder position to 0 
  Serial.begin(115200);
  } 
  
void loop() { 
  //do stuff dependent on encoder position here 
  //such as move a stepper motor to match encoder position 
  //if you want to make it 1:1 ensure the encoder res matches the motor res by dividing/multiplying 
  microsNow = micros(); //<<< 
  microsDT = microsNow - StartReverse;
  if ((encoder > 0) && ( microsDT > WaitReverse)) {  //WaitReverse = 50; //<<< 
    Serial.print("Encoder : ");
    Serial.print(encoder);
    Serial.print("   Now : ");
    Serial.print(microsNow);
    Serial.print("   DT : ");
    Serial.print(microsDT);
    Serial.print("   Reverse!! ");
    Serial.println();
    digitalWrite(motor_direction, HIGH);// move stepper in reverse 
    digitalWrite(motor_step, HIGH); 
    digitalWrite(motor_step, LOW); // 
    delayMicroseconds(4500); //
    //_delay_us(600); //modify to alter speed <<< 
    //motor_position++; 
    encoder = 0; //    
    //encoder--; 
    StartReverse = micros(); //<<< 
    } else if ((encoder < 0) && ( microsDT > WaitForward)) { 
      Serial.print("Encoder : ");
      Serial.print(encoder);
      Serial.print("   Now : ");
      Serial.print(microsNow);
      Serial.print("   DT : ");
      Serial.print(microsDT);
      Serial.print("   Forward! ");
      Serial.println();
      digitalWrite (motor_direction, LOW); //move stepper forward 
      digitalWrite (motor_step, HIGH); 
      digitalWrite (motor_step, LOW); // 
      delayMicroseconds(4500); //
      //_delay_us(600); //modify to alter speed <<< 
      //motor_position--; 
      encoder = 0; //      
      //encoder++; 
      StartForward = micros(); //<<< 
      } 
 } 
 
 void encoderPinChangeA() { 
  if (digitalRead(encoder_a) == digitalRead(encoder_b)) { 
    encoder--; 
    } else { 
      encoder++; 
      } 
    } 
    
 void encoderPinChangeB() { 
  if (digitalRead(encoder_a) != digitalRead(encoder_b)) { 
    encoder--; 
    } else { 
      encoder++; 
      } 
    }
 
