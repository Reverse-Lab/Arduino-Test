boolean curBut = LOW;
boolean lastBut = LOW;
boolean ledOn = false;

void setup() {
  // put your setup code here, to run once:
pinMode (12, OUTPUT);
pinMode (2, INPUT);
}
void loop() {
  curBut = digitalRead(2);
  if (lastBut  == LOW && curBut == HIGH) {
    ledOn =! ledOn;
  }

  lastBut = curBut;

  digitalWrite (12, ledOn);
}
