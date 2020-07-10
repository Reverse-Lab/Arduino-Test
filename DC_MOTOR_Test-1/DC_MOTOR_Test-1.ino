
#define in1   5
#define in2   15
#define en    16

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);

  digitalWrite(en,HIGH);

  
}

void spee()
{
  for(int i=0; i<255; i++){
    digitalWrite(in1, HIGH);
    delay(255-i);
    digitalWrite(in1, LOW);
    delay(255+i);
  }
}

void loop() 
{
  
  //digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  spee();
}
