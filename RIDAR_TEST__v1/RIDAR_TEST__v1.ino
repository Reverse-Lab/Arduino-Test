



void setup() {
  // put your setup code here, to run once:
  pinMode (A0, INPUT);
  pinMode (13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int i = map (analogRead(A0), 0, 1023, 0, 255);
  if (i >= 100) digitalWrite (13, HIGH); else digitalWrite (13, LOW);
}
