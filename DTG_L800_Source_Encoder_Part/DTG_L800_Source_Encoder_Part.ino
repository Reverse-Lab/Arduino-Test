/* DIY DTG Arduino Uno code for Epson L800 DIY may in ph?ng UV(c?n th?n mua m?c x?n n?u mua m?c d?m ; h? đ?u in rang ch?i nha)
 * http://www.cncprovn.com
 * http://forum.cncprovn.com/forums/80-Arduino
 * Created: 26/4/2018 
 */

#define ENCODER_OPTIMIZE_INTERRUPTS
#define pin_MS1 5     // pin 5 on UNO is connected to the Micro Step 1 of TB6560
#define pin_MS2 6     // pin 6 on UNO is connected to the Micro Step 2 of TB6560
#define pin_MS3 7     // pin 7 on UNO is connected to the Micro Step 3 of TB6560

#include <Encoder.h>
#include <Bounce2.h>
//Khai bao chan encoder
Encoder epsonEncoder(2, 3); 
//Khai bao cac nut nh?n (l?y khay in ra nut 5 )(đ?y khay in vao nut 4)(in tr?ng 2 pass nut 6)(c?m bi?n nh?n in nut 10 start print)

int motorEnable = 11;     //  enable driver(step hay servo)
int motorDirection = 10;  // driver dir pin
int motorStep = 9;       // driver Pulse pin

// khai bao cac pulse (kho?ng cach l?y khay in )ra vao 
int stepperPosition;  
int stepperCount;    
int stepperFactor = 35; // <<                      
int platenState;         
int passCount;
int maxPasses;
int ejectSpeed = 550;  
int homeSpeed = 750;
int maxSteps = 9000;
long platenEncoder = 260000; 
int peMinEncoder = 12000;
int peMaxEncoder = 12500;
long positionEncoder  = -800; 
long newEncoder; 
                        
void setup() {   

      pinMode(pin_MS1, OUTPUT);
      pinMode(pin_MS2, OUTPUT);
      pinMode(pin_MS3, OUTPUT);

      digitalWrite(pin_MS1, LOW);  // 1/2 스텝 : 2500us
      digitalWrite(pin_MS2, LOW);  // 1/4 step
      digitalWrite(pin_MS3, LOW);  //
      
  Serial.begin(115200);
  Serial.println("Epson Encoder:");
  pinMode(motorStep, OUTPUT);
  pinMode(motorDirection, OUTPUT);

}
void loop() {

      platenState = 3;
     // passCount = 0;  // Reset the pass counter to 0.    
      //}
     //if (value_outBtn == LOW & (stepperPosition) < (maxSteps)) {
      //platenState = 4;      
      //}
switch (platenState) { 
   /*case 1:
          if (value_backStop == LOW) { 
            epsonEncoder.write(0);
            stepperPosition = 0;
            platenState = 4; 
            break;
          }          
        digitalWrite(motorDirection, HIGH); // Set motor to reverse
        digitalWrite(motorStep, HIGH); // Take a step
          delayMicroseconds(homeSpeed); // Step Speed
        digitalWrite(motorStep, LOW);  
     break;   
     case 2: 
          if (value_backStop == LOW) {
            epsonEncoder.write(0); 
            stepperPosition = 0; 
            platenState = 3;
            break;
          }          
        digitalWrite(motorDirection, HIGH);
        digitalWrite(motorStep, HIGH);
          delayMicroseconds(homeSpeed);
        digitalWrite(motorStep, LOW);
        stepperPosition--;       
     break;    */
     case 3: 
     /*if (value_backStop == LOW) { 
        digitalWrite(motorDirection, LOW); 
        digitalWrite(motorStep, HIGH);
          delayMicroseconds(12500);
        digitalWrite(motorStep, LOW);     
      break;
      }*/
       newEncoder = epsonEncoder.read(); 
       if (newEncoder != positionEncoder) {
      stepperCount = newEncoder / stepperFactor;
   Serial.print("Encoder = ");
   Serial.print(newEncoder);
   Serial.println();
   Serial.print("Stepper Count = ");
   Serial.print(stepperCount);
   Serial.println();
   Serial.println(); 
    positionEncoder = newEncoder;
     }
      if (stepperCount > stepperPosition) { 
        digitalWrite(motorDirection, LOW);      
        digitalWrite(motorStep, HIGH); 
          digitalWrite(motorStep, LOW);
          stepperPosition++;
      }
      if (stepperCount < stepperPosition) {  
          digitalWrite(motorDirection, HIGH);                 
          digitalWrite(motorStep, HIGH);
          digitalWrite(motorStep, LOW);
          stepperPosition--;
      }
       /*if ((newEncoder) > peMinEncoder & (newEncoder) < peMaxEncoder) {
            analogWrite(peSignal, 0);     
       }
        if ((newEncoder) > platenEncoder) {
            analogWrite(peSignal, 165);
              passCount = passCount + 1;
            if (digitalRead(passBtn) == HIGH){ 
                maxPasses = 1;
            }
              else
            {
                maxPasses = 2;
            }
            if (passCount == maxPasses){ 
                 platenState = 4;   
            }
               else
            {
                platenState = 2; // Send the platen back in for automatic 2nd pass
            }                    
            }   */  
     break;    
     case 4:
       if (stepperPosition > maxSteps) {
        platenState = 0;        
        }
       digitalWrite(motorDirection, LOW); 
        digitalWrite(motorStep, HIGH); 
          delayMicroseconds(ejectSpeed);
        digitalWrite(motorStep, LOW);
        stepperPosition++;
     break;    
     default: 
     break; 
  }
}


//Read more: http://forum.cncprovn.com/threads/13868-CODE-ARDUINO-DIY-may-in-phang-epson-L800#ixzz6Chco4ISS
