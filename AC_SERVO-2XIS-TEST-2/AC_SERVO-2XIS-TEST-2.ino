#define SON_1     8       // stepper motor enable, low level effective
#define SON_2     9
#define X_DIR     5       //X axis, stepper motor direction control 
#define Y_DIR     6       //y axis, stepper motor direction control
//#define Z_DIR     7       //z axis, stepper motor direction control
#define X_STP     2       //x axis, stepper motor control
#define Y_STP     3       //y axis, stepper motor control
//#define Z_STP     4       //z axis, stepper motor control
/*
// Function: step   -control the direction and number of steps of the stepper motor
// Parameter: dir  -direction control, dirPin corresponds to DIR pin, stepperPin correspomds to 


step pin, steps is the number of steps.
// no return value
*/
void step(boolean dir, byte dirPin, byte stepperPin, long steps)
{
  digitalWrite(dirPin, dir);
  delay(50);
  for (long i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(20);  
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(20);  
  }
}
void setup(){// set the IO pins for the stepper motors as output 
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  //pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(SON_1, OUTPUT); pinMode(SON_2, OUTPUT);
  digitalWrite(SON_1, HIGH); digitalWrite(SON_2, HIGH);
  delay(2000);
}
void loop(){
  step(false, X_DIR, X_STP, 10000); // x axis motor rotates CCW for 1 circle, as in 200 steps
  step(false, Y_DIR, Y_STP, 10000); // y axis motor rotates CCW for 1 circle, as in 200 steps
  //step(false, Z_DIR, Z_STP, 3200); // z axis motor rotates CCW for 1 circle, as in 200 steps
  delay(1000);
  step(true, X_DIR, X_STP, 10000); // X axis motor rotates CW for 1 circle, as in 200 steps
  step(true, Y_DIR, Y_STP, 10000); // y axis motor rotates CW for 1 circle, as in 200 steps
  //step(true, Z_DIR, Z_STP, 3200); // z axis motor rotates CW for 1 circle, as in 200 steps
  delay(1000);
}
