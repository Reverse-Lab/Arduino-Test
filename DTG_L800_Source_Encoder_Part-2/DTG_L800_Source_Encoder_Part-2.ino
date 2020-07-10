/* DIY DTG Arduino Uno code for Epson L800 DIY may in ph?ng UV(c?n th?n mua m?c x?n n?u mua m?c d?m ; h? đ?u in rang ch?i nha)
 * http://www.cncprovn.com
 * http://forum.cncprovn.com/forums/80-Arduino
 * Created: 26/4/2018 
 */

#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Encoder.h>
#include <Bounce2.h>

#include "BasicStepperDriver.h"

// All the wires needed for full functionality
#define DIR 10
#define STEP 9
#define SLEEP 13 // optional (just delete SLEEP from everywhere if not used)

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 400
#define RPM 120

// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000

 #include "A4988.h"
 #define MS1 5
 #define MS2 6
 #define MS3 7
 A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, MS1, MS2, MS3);

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

//Khai bao chan encoder
Encoder epsonEncoder(2, 3); 
//Khai bao cac nut nh?n (l?y khay in ra nut 5 )(đ?y khay in vao nut 4)(in tr?ng 2 pass nut 6)(c?m bi?n nh?n in nut 10 start print)

// 2-wire basic config, microstepping is hardwired on the driver
// BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//int motorEnable = 35;     //  enable driver(step hay servo)
//int motorDirection = 10;  // driver dir pin
//int motorStep = 9;       // driver Pulse pin


int stepperPosition;  
int stepperCount;    
                    int stepperFactor = 57.6; // <<                      
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
long step = 0;
                        
void setup() {   
      
  Serial.begin(115200);
//  Serial.println("Epson Encoder:");
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  stepper.begin(RPM, MICROSTEPS);

    /*
     * Set LINEAR_SPEED (accelerated) profile.
     */
   //stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);


}
void loop() {

       newEncoder = epsonEncoder.read(); 
       if (newEncoder != positionEncoder) {
          stepperCount = newEncoder / stepperFactor;
//          Serial.print("Encoder = ");
          Serial.print(newEncoder);
          //Serial.println();
          Serial.print(",");
          Serial.print(stepperCount);
          //Serial.println();
  //        Serial.print("    Completed = ");
  //        Serial.print(stepper.getStepsCompleted());
  //        Serial.println(); 
          Serial.println(); 
          positionEncoder = newEncoder;
          }
      if (stepperCount > stepperPosition) { 
          //digitalWrite(motorDirection, LOW);      
          //digitalWrite(motorStep, HIGH); 
          //  digitalWrite(motorStep, LOW);
          stepper.move(1);
          //stepperPosition = 0; 
          stepperPosition++;
          }
      if (stepperCount < stepperPosition) {  
          //digitalWrite(motorDirection, HIGH);                 
          //digitalWrite(motorStep, HIGH);
          //digitalWrite(motorStep, LOW);
          stepper.move(-1);
          //stepperPosition = 0; 
          stepperPosition--;
          }
}


//Read more: http://forum.cncprovn.com/threads/13868-CODE-ARDUINO-DIY-may-in-phang-epson-L800#ixzz6Chco4ISS
