int PP= 5;
int NP= 6;
int SON= 7;
unsigned long counter = 0;
int mdelay;
int CW =0;

void setup() {
  DDRD = B11111111;
  
  //pinMode(PP, OUTPUT);
  //pinMode(NP, OUTPUT);
  //pinMode(SON, OUTPUT);
  
  //digitalWrite(SON, HIGH);
  
  PORTD=B1000000;
  delay(1000);
  //digitalWrite(NP, LOW);
  PORTD=B10000000;
  delay(1000);
}

void spin()
{
    bitSet(PORTD, 5);
    delayMicroseconds(10);
    bitClear(PORTD, 5);
    delayMicroseconds(10);    
}

void loop() {

  spin(); counter++;
  
    if ((CW==0) && (counter > 100000))
    {
      bitSet(PORTD,6);
      counter = 0;
      CW=1;
      delay(1000);
    } 
    
    if ((CW==1) && (counter > 100000)) {
      bitClear(PORTD,6);
      counter = 0;
      CW=0;
      delay(1000);
    }
}
