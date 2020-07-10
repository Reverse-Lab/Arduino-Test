#include <arduino.h>

unsigned char servo = 0b11111111;
unsigned char cnt = 0;

void setup() {
  DDRB = 0xFF;
  PORTB = servo;

  TIMSK = 0b00000001;
  TCCR0 = 0b00000111;
  TCNT0 = 0b00000000;

  SREG = 0b10000000;
}

interrupt [TIM0 OVF] void timer int0(void)
{
  cnt++;
  if (cnt == 31) {
    servo = 0b1100000;
    PORTB = servo;
    cnt = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
