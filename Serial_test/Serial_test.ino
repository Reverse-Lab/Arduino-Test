
void setup(){
  Serial.begin(9600);
}
 
void loop(){
  Serial.print("E0 01 30");   // LED on
  delay(500);
//  Serial.print("0");   // LED off
//  delay(500);
}
