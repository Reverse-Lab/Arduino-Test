int PP= 5;
int NP= 6;
int SON= 7;
unsigned long counter = 0;
int mdelay;
int CW =0;

void setup() {
  DDRB = B11111111;
  
}

void spin()
{
    bitSet(PORTB, 0);
    bitClear(PORTB, 1);
    delayMicroseconds(500);
    bitSet(PORTB, 1);
    bitClear(PORTB, 0);
    delayMicroseconds(500);
    
}

void loop() {

  spin(); 
}
