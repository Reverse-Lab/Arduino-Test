
 
void setup(){
  Serial.begin(115200);
//  bluetooth.begin(9600);
}
 
void loop(){
  
  if (Serial.available()) {
    Serial.write(Serial.read());
  }
}
