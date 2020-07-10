void setup()
{
  Serial.begin(9600);
}
void loop()
{
  unsigned long millisTime = millis(); // 1초 단위면 millisTime = millis()/1000;
  int v1 = millisTime%10;
  int v2 = (millisTime/10)%10;
  int v3 = (millisTime/100)%10;
  int v4 = (millisTime/1000)%10;

  Serial.print(v4);
  Serial.print(" : ");
  Serial.print(v3);
  Serial.print(" : ");
  Serial.print(v2);
  Serial.print(" : ");
  Serial.println(v1);
}
