// Set variables - These variables effect the operation of the printer base. Make changes to compensate for different build types.

const int desktopApp = 0;   // 0 - Off - Shut off the desktop App
                            // 1 - Turns on serial for OpenDTG Desktop App          
                      
const int asfInterval = 50;  // Time delay for the ASF signal

//float stepperFactor = 17;   // The total encoder position is divided by this number to calculate motor steps in relationship to encoder movement
                          // Based on microstepping of 800 steps per motor revolution
 float stepperFactor = 8.53; // For Epson 1430 - shared by randyb123 

                          
const int ejectSpeed = 250;  // Step delay that controls platen eject speed. Lower is faster
const int homeSpeed = 250;   // Step delay that controls platen speed when homing at startup. Lower is faster
const int slowSpeed = 750;   // Slows the step down when close to either end
const int bootSpeed = 250;   // Speed of platen during boot up
const int alignSpeed = 12500;  // Step speed off of back limit switch

unsigned int maxSteps = 19500; // Maximum steps the motor takes to get from the back to the front. Controls where platen stops on eject. 
                      // NOTE: This max steps is on a base with 26.5 inches of travel, 30 teeth pulley, and 800 steps per stepper motor revolution.
                      
//#define platenEncoder 240000L // Encoder position from back until end of platen / print area. This triggers the eject after printing.

#define platenEncoder 130000L // For Epson 1430 - shared by randyb123 


// R3000 and P600 PE sensor Settings
const int peMinEncoder = 950; // MIN: Sets the range when to trigger the PE (Paper in) signal. NOTE: These default settings were tested on the Epson R3000 / P600. May be different on other models.
const int peMaxEncoder = 1050; // MAX:
