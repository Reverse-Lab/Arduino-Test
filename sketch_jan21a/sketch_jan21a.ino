#include <AFMotor.h>

// Connect a stepper motor with 20 steps per revolution (18 degree)
// to motor port #1 (M1 and M2)  #2 (M3,M4)
AF_Stepper motor(400, 1);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");



  motor.setSpeed(150); 
}

void loop() {
  Serial.println("Single coil steps");
  motor.step(8000, FORWARD, SINGLE);
  delay(1000);
  motor.step(8000, BACKWARD, SINGLE);
  delay(1000);
  
//  Serial.println("Double coil steps");
//  motor.step(2000, FORWARD, DOUBLE);
//  motor.step(5000, BACKWARD, DOUBLE);

  Serial.println("Interleave coil steps");
  motor.step(8000, FORWARD, INTERLEAVE);
  delay(1000);
  motor.step(8000, BACKWARD, INTERLEAVE);
  delay(1000);
//  Serial.println("Micrsostep steps");
//  motor.step(2000, FORWARD, MICROSTEP);
//  motor.step(2000, BACKWARD, MICROSTEP);
}
