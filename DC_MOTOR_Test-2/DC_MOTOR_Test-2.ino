
#define in1   5   //PE5
#define in2   15  //PB7
#define en    16  //PG3

void setup() {

    DDRE  = 0x20;
    DDRG  = 0x08;
    DDRB  = 0x80;
    DDRB |= (1<<DDB4); // PB4, OC0

    PORTG = 0x08;         //en PIN 출력
    PORTB |= (1<<DDB7);

    TCCR0 = 0x64;
    TCNT0 = 0;

    OCR0  = 0x30;
}
void loop() 
{
    if((PINB&0x10) == 0x10){ // PB4(OC0)의 출력이 있으면 모터 ON
      PORTE = 0x20;
    } else {
      PORTE = 0x00;
    }
  delay(1000);
}
