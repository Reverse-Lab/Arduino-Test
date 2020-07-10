//https://blog.naver.com/kiatwins/220804644450

#define BAUD        9600
#define uart_DIR    DDRB
#define uart_OUT    PORTB
#define uart_TX_PIN 0x02  // PB1 (MISO)
#define TX_PIN_1    uart_OUT |= uart_TX_PIN
#define TX_PIN_0    uart_OUT &=~uart_TX_PIN
#define delay_B     (1200000 / BAUD) - 1

unsigned int r1stat = 0;
unsigned int r1stat_temp = 0;
    
void bit_bang_tx(unsigned char data) {
    unsigned char i;
    TX_PIN_0;                    // Start bit
    TCNT0=0;
    while(TCNT0<delay_B);
    for(i=0; i<8; i++) {        // LSBFIRST
        if (data & (0x01 << i)) TX_PIN_1;
        else TX_PIN_0;
        while(TCNT0<delay_B);
    }
    TX_PIN_1;                    // Stop bit
    while(TCNT0<delay_B);
}

void bit_bang_tx_str(char *str) {
    while(*str) bit_bang_tx(*str++);
}

unsigned int adc_read (void) {
  int i;
  r1stat = 0;
  for(i=0; i<64; i++){
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    r1stat_temp = ADCW;
    //r1stat_temp = ADCL;
    //r1stat_temp |= (int)ADCH;
    r1stat += r1stat_temp;
  }
  r1stat = r1stat / 64;
  return r1stat;
}

void setup()
{
    
    TCCR0A = 0x02;  // CTC
    TCCR0B = 0x02;  // 9600000 / 8 = 1200000Hz = 0.83333us
    OCR0A  = delay_B; // (1+124) x 0.8333 = 104.166us
    ADCSRA = 0xD5;  // ADC Enable, 9600000/128=75000Hz=13.33us
    ADMUX  = 0x01;  // ADC1 (PB2)
    ADCSRB = 0x00;
    
    uart_DIR |= uart_TX_PIN;
    TX_PIN_1; 
}

void loop()
{
    char str[5];
    unsigned int val;
    float temp;

    val = adc_read();
    temp = -46.85+175.72*val/1024;
    
    utoa(val, str, 10);
    bit_bang_tx_str("ADC  = ");
    bit_bang_tx_str((char *)str);
    bit_bang_tx_str(" , ");

    itoa(temp, str, 10);
    bit_bang_tx_str("Temp = ");
    bit_bang_tx_str((char *)str);
    bit_bang_tx_str("C\r\n");
    delay(1000);
}
