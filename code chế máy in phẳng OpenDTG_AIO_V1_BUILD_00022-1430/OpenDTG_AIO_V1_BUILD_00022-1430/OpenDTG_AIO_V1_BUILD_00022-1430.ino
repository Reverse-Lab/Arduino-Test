
/* OpenDTG.com DIY DTG Arduino All in One V1 board for Epson 1430, R3000 and P600 Models
 * http://www.opendtg.com/
 *
 * Any changes or improvements required to be made publicly available @ opendtg.com
 * Not intended for commercial reproduction. For commercial permission please contact: info@opendtg.com
 * 
 * Created by: Andy McDaniel - Copyright 2016
 * Created: 7/24/2016
 * Updated: 7/30/2016 - Add soft step off back limit switch for better registration
 *                    - Debounce buttons using bounce2 library
 *          8/2/2016  - Added automatic rewind for 2nd pass. Reads rocker switch to determine if it should do 1 printing pass and eject or do 2 printing passes then eject.
 *         9/11/2016  - Changed Encoder pins to match OpenDTG Shield configuration. 
 *                    - Changed PE Signal code to a variable pePWM - This allows adjustment in 1 variable.
 *                    - Added additional comments
 *         9/30/2016  - Added header.h and settings.h. Now most users only need to edit settings.h 
 *         10/2/2016  - Added Debug Mode. Turn on and off in settings.h. Turn off debug mode for normal use.
 *         10/4/2016  - Added Epson 1400 PE info in settings.h. 
 *                    - Added debugInterval to settings.h to control debug serial output frequency
 *                    - Added pePaperIn and pePaperOut variables in settings.h to control voltage of the PE switch
 *        10/11/2016  - Changed button functions to prevent eject during printing.
 *        11/16/2016  - Added interrupt function for PCINT12 on pin 27 for MotorSen1 sensor.
 *                    - ASF timing code in Interrupt
 *        11/21/2016  - Added TB6600 Motor functionality
 *        12/23/2016  - Added support for using OpenDTG Desktop App along with physical buttons
 *        12/27/2016  - Added slowSpeed to slow stepper before it hits the limit switch or end of maxSteps in the front
 *                    - Added bootSpeed for a slower step to find the back switch without hitting it hard
 *        01/18/2017  - Updated ASF code 
 *        02/12/2017  - Changed microstepping to 1/16th
 *                    - Motor is now disabled when in front. 
 *        06/06/2017  - Added ASF code shared by jeremyroyer (No longer need ASF stepper motor plugged in)
 *                    - Added 1430 info in settings.h  shared by randyb123
 *                    - Removed Debug for now. Will implement better later
 *        07/28/2017  - Changed Motor timing to smooth out platen movements
 *                    - Adjusted variables to ensure platen returns for 2 pass fast enough
 *        08/07/2017  - Fixed Desktop App eject.
 *        09/06/2017  - Code Clean up
 *        02/14/2018  - Changed LOW to HIGH for bstop
 *                    - Flipped Encoder numbers
 *  
 *                    
 *
 * Version: 1.00020
 *              
 */
#include "header.h"
#include "settings.h"


#include <Encoder.h>
#include <Bounce2.h>


Encoder epsonEncoder(encoderA, encoderB); // Arduino Uno uses pins 2,3 for interrupts.

int stepperPosition;  // Current position of the stepper motor
int stepperCount;     // Where the stepper motor should be

int platenState;         // Stores what the platen is currently doing. 
                            // 1 = Needs to be sent to back for homing at startup
                            // 2 = Send platen in
                            // 3 = Print Ready / Printing
                            // 4 = Ejecting platen

int peState;   // Holds the variable we set when we set the PE sensor high and low

int passCount; // Keeps track of how many times the platen has went in with 1 button press
int maxPasses; // Should be 1 or 2

long positionEncoder  = -800; 
long newEncoder;     

int message = 0; // This will hold one byte of the serial message

  // Instantiate a Bounce object
    Bounce debouncer_inBtn = Bounce(); 
  // Instantiate another Bounce object
    Bounce debouncer_outBtn = Bounce(); 
   // Instantiate another Bounce object
    Bounce debouncer_backStop = Bounce();

    // Timer for debug mode
unsigned long previousMillis = 0;        // will store last time was updated

// Timer for eject step
unsigned long previousMicros = 0;        // will store last time was updated



int asfState = LOW;                     
                            
void setup() { 



// Turn on Power LED
pinMode(powerLED, OUTPUT); 
digitalWrite(powerLED, HIGH);  

if (desktopApp == 1){
  Serial.begin(9600); //set serial to 9600 baud rate This is for the Windows App communication
}

 // Turn on optical sensor 
  pinMode(pause, OUTPUT); 
  digitalWrite(motorM1, HIGH);  

  
  // Setup TB6600 Motor Driver 
  pinMode(motorM1, OUTPUT); 
  digitalWrite(motorM1, HIGH);  
  
  pinMode(motorM2, OUTPUT); 
  digitalWrite(motorM2, LOW);  
  
  pinMode(motorM3, OUTPUT); 
  digitalWrite(motorM3, HIGH);  
  
  pinMode(motorReset, OUTPUT); 
  digitalWrite(motorReset, HIGH);  
  
  pinMode(motorTQ, OUTPUT); 
  digitalWrite(motorTQ, HIGH);  

  pinMode(motorEnable, OUTPUT); 
  digitalWrite(motorEnable, HIGH);  
             
  pinMode(motorStep, OUTPUT);
  pinMode(motorDirection, OUTPUT);

  pinMode(passBtn, INPUT_PULLUP);

  platenState = 1;  // This will send the platen in to the home position.
  stepperPosition = 0;
  stepperCount = 0; 

  passCount = 0;


  pinMode(asfSignal, OUTPUT);

   // Setup the platen in button with an internal pull-up :
  pinMode(inBtn,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer_inBtn.attach(inBtn);
  debouncer_inBtn.interval(8); // interval in ms
  
   // Setup the platen out button with an internal pull-up :
  pinMode(outBtn,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer_outBtn.attach(outBtn);
  debouncer_outBtn.interval(8); // interval in ms

   // Setup the back limit switch button with an internal pull-up :
  pinMode(backStop,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer_backStop.attach(backStop);
  debouncer_backStop.interval(5); // interval in ms

 pinMode(peSignal, OUTPUT);
   digitalWrite(peSignal, HIGH); // set the printer to think it has no paper. (3 volts, don't go over 3.3v)
   peState = 1;
   
}

void ASF(){
  if (asfState==HIGH){
    digitalWrite(asfSignal, LOW);
  }
  else {
    digitalWrite(asfSignal, HIGH);
  }
  asfState=!asfState;
}

void loop() {
    
      if (desktopApp == 1){
          if (Serial.available() > 0) { // Check to see if there is a new message
              message = Serial.read(); // Put the serial input into the message
          }
      }
                      

unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > asfInterval){
     previousMillis = currentMillis;
     ASF();
     }
 
        // Update the Bounce instances :
          debouncer_inBtn.update();
          debouncer_outBtn.update();
          debouncer_backStop.update();
      
         // Get the updated value :
        int value_inBtn = debouncer_inBtn.read();
        int value_outBtn = debouncer_outBtn.read();
        int value_backStop = debouncer_backStop.read();
unsigned long currentMicros = micros();      
      
      switch (platenState) { // Switch statement is the main structure for the platen operation
        
           case 1: // Startup homing and eject
            
              // Check if back limit switch is pressed.
                if (value_backStop == LOW) { // Read pressing of limit swith. NOTE: This assumes wiring the switch as NO (Normally Open)
                  epsonEncoder.write(0); // set encoder to 0
                  stepperPosition = 0; // set stepper position to 0
                  platenState = 4; // Start the eject by going to case 4
                  break;
                } 
                
              digitalWrite(motorDirection, HIGH); // Set motor to reverse
      
              digitalWrite(motorStep, HIGH); // Take a step
                delayMicroseconds(bootSpeed); // Step Speed
              digitalWrite(motorStep, LOW);
         
           break;
           
           case 2: // Send Platen In
              
              // Check if back limit switch is pressed.
                if (value_backStop == LOW) { // Read pressing of limit swith. NOTE: This assumes wiring the switch as NO (Normally Open)
                  epsonEncoder.write(0); // set encoder to 0 
                  stepperPosition = 0; // set stepper position to zero
                
                  platenState = 3; // Ready to print
                  break;
                } 
      
                     /// If Eject / Out button is pressed and the platen isn't already at or beyond the maximum number of steps. Send the platen to the front.
                        if (value_outBtn == LOW) {
                            platenState = 4;
                         }
      
                           if (message == '3'){ // If a 3 is received from desktop app
                             platenState = 4;
                             message = 0;
                           }
      
                if(currentMicros - previousMicros > homeSpeed){
                  previousMicros = currentMicros;


                   digitalWrite(motorDirection, HIGH); // Set motor to forward
      
              digitalWrite(motorStep, HIGH); // Take a step
                delayMicroseconds(3); // Step Speed
              digitalWrite(motorStep, LOW);
              stepperPosition--; // Add step taken
                  }
                
  
       
           break;
           
           case 3: // Print Ready / Printing

            if (value_inBtn == LOW && (value_backStop) == LOW) {
            platenState = 5;
            stepperPosition = 19500;
            }
      
           if (value_backStop == LOW) { // Use this section to slowly step carriage off back limit switch
            
              digitalWrite(motorDirection, LOW); // Set motor to forward
      
              digitalWrite(motorStep, HIGH); // Take a step
                delayMicroseconds(alignSpeed); // Step Speed
              digitalWrite(motorStep, LOW);
              stepperPosition++; // Add the step taken
             
            break;
            }
      
              
              /// If Eject / Out button is pressed and the platen isn't already at or beyond the maximum number of steps. Send the platen to the front.
           if (value_outBtn == LOW & (newEncoder) == 0) {
            platenState = 4;
            
            }
      
            if (message == '3'){ // If a 3 is received from desktop app
                             platenState = 4;
                             message = 0;
             }
              
             
              //Follow Epson encoder signal
              newEncoder = epsonEncoder.read(); 
      
             if (newEncoder != positionEncoder) { // Check to see if the encoder wheel has moved. 
          
            stepperCount = newEncoder / stepperFactor; // Calculate how many steps the motor should be at
          
          positionEncoder = newEncoder;
        }
      
            if (stepperCount > stepperPosition) {  // Clockwise motion of the encoder wheel
      
                digitalWrite(motorDirection, LOW); // Set motor to forward
              
                digitalWrite(motorStep, HIGH); // Take the step
                delayMicroseconds(2);
                digitalWrite(motorStep, LOW);
                stepperPosition++; // Add the step taken
            
            }
      
            if (stepperCount < stepperPosition) {  // Counter clockwise motion of the encoder wheel
      
                digitalWrite(motorDirection, HIGH); // Set motor to reverse          
                
                digitalWrite(motorStep, HIGH); // Take the step
                delayMicroseconds(2);
                digitalWrite(motorStep, LOW);
                stepperPosition--; // Subtract the step taken
            
            }
      
              // At XXX encoder trigger the PE signal
             if ((newEncoder) > peMinEncoder & (newEncoder) < peMaxEncoder) {
                  digitalWrite(peSignal, LOW);
         
                   peState = 0;  
             }
                 
              // At XXXX encoder untrigger the PE Signal and then either eject or rewind for automatic 2nd pass
              if ((newEncoder) > platenEncoder) {
                  digitalWrite(peSignal, HIGH);
                   peState = 1;
      
                    passCount = passCount + 1;
                    
                    // Serial.println(passCount);
               
                  
           
      
                  if (passCount == maxPasses){ // 
                   
                       platenState = 4; // Eject   
                    }
                     else
                    {
                     
                      platenState = 2; // Send the platen back in for automatic 2nd pass
                     }
                           
             }
           
           break;
           
           case 4: // Eject Platen
                 
            // IF maxSteps has been hit stop and send to case 0 (default)
      
            // If In button is pressed and platen isn't already in the back. Send the platen to the back.
          if (value_inBtn == LOW & (stepperPosition) > 0){
            platenState = 2;
            passCount = 0;  // Reset the pass counter to 0.
                if (digitalRead(passBtn) == HIGH){ // Reads the Rocker 2 pass switch to determine if the platen does 1 pass or 2.
                      maxPasses = 1;
                   }
                    else
                  {
                      maxPasses = 2;
                  }
            }
      
             if (message == '1' & (stepperPosition) > 0){ // If a 1 is received
              platenState = 2;
              passCount = 0;  // Reset the pass counter to 0.
              maxPasses = 1;
              message = 0;
            }
          if (message == '2' & (stepperPosition) > 0){ // If a 2 is received
              platenState = 2;
              passCount = 0;  // Reset the pass counter to 0.
              maxPasses = 2;
              message = 0;
            }
      
             if (stepperPosition > maxSteps) {
              
              platenState = 5; // Set to case 5 so it stops and does nothing but waits for the in button to be pressed
                
              }
      
               if ((maxSteps - stepperPosition) < 15) {

                
                  if(currentMicros - previousMicros > ejectSpeed){
                  previousMicros = currentMicros;


                   digitalWrite(motorDirection, LOW); // Set motor to forward
      
              digitalWrite(motorStep, HIGH); // Take a step
                delayMicroseconds(3); // Step Speed
              digitalWrite(motorStep, LOW);
              stepperPosition++; // Add step taken
                  }
      
             
               }
      
               else
               {
                 unsigned long currentMicros = micros();
                  if(currentMicros - previousMicros > ejectSpeed){
                  previousMicros = currentMicros;


                   digitalWrite(motorDirection, LOW); // Set motor to forward
      
              digitalWrite(motorStep, HIGH); // Take a step
                delayMicroseconds(3); // Step Speed
              digitalWrite(motorStep, LOW);
              stepperPosition++; // Add step taken
                  }
                }
      
        
           break;
      
      
           case 5:  // Holding in the front for button press

              digitalWrite(motorEnable, LOW);
           
       // If In button is pressed and platen isn't already in the back. Send the platen to the back.
          if (value_inBtn == LOW & (stepperPosition) > 0){
            digitalWrite(motorEnable, HIGH);
            platenState = 2;
            passCount = 0;  // Reset the pass counter to 0.
      
            if (digitalRead(passBtn) == HIGH){ // Reads the Rocker 2 pass switch to determine if the platen does 1 pass or 2.
                      maxPasses = 1;
                   }
                    else
                  {
                      maxPasses = 2;
                  }
          
            }
      
         if (message == '1'){ // If a 1is received
          digitalWrite(motorEnable, HIGH);
              platenState = 2;
              passCount = 0;  // Reset the pass counter to 0.
              maxPasses = 1;
              message = 0;
            }
          if (message == '2'){ // If a 2 is received
            digitalWrite(motorEnable, HIGH);
              platenState = 2;
              passCount = 0;  // Reset the pass counter to 0.
              maxPasses = 2;
              message = 0;
            }
      
           break;
           
           default: 
             // Nothing happens - Platen should be in front position
           break; 
        }
  

}



