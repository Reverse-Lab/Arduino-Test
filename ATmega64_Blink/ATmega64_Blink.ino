/*
  Blink
  
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(16, OUTPUT);  //  pin_16
  pinMode(17, OUTPUT);  //  pin_17
  pinMode(36, OUTPUT);  //  pin_36
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(16, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(17, HIGH);
  digitalWrite(36, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(16, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(17, LOW);
  digitalWrite(36, LOW);
  delay(1000);                       // wait for a second
}
