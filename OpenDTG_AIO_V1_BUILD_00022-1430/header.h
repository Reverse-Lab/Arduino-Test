// Assign Arduino Pin Numbers. Use these numbers to wire your arduino correctly. All numbers are to Digital Pins on the Arduino Uno.

#define encoderA 2   // Encoder input from epson printer
#define encoderB 3  // Encoder input from epson printer

#define inBtn 4     // Momentary push button to send the Platen to the Back / Home position - Other switch wire goes to Arduino Ground
#define outBtn 5    // Momentary push button to eject the platen to the front position  - Other switch wire goes to Arduino Ground
#define passBtn 6  // Automatic 2 pass Rocker style switch - Other switch wire goes to Arduino Ground

#define backStop 7   // Back or Home position. This is the rear limit switch used to stop the platen at the back of your machine.

#define peSignal 16  // Write HIGH or LOW - Hardware limits voltage output to 3v to protect Epson
#define asfSignal 17  // Sends HIGH or LOW to Epson to similate ASF sensor. Hardware limits voltage to 3v to protect Epson

#define pause 15 // Normally the pause pin but I'm using it to power the Optical end stop

#define powerLED 19


//Setup for TB6600 Stepper Driver
#define motorEnable 14     // Not used yet: Some motor drivers require a HIGH signal to enable.
#define motorDirection 8  // Wire to motor driver direction pin
#define motorStep 9      // Wire to motor driver Step / Pulse pin

#define motorM1 11   // M1, M2, M3 set the microstepping of the driver
#define motorM2 12        
#define motorM3 13

#define motorReset 10
#define motorTQ 15  
    
