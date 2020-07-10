//https://blog.naver.com/kiatwins/220804644450  -> RS232 bit-banging 참고

//https://gist.github.com/adnbr/9289235 -> ADC 참고

#define F_CPU       9600000
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

uint16_t adc_read (void) {
  unsigned int i;
  r1stat = 0;
  for(i=0; i<64; i++){
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    //return ADCW;
    
    r1stat_temp =  ADCL; //하위 8비트
    r1stat_temp |=(int)ADCH <<8; //상위 8비트
    r1stat += r1stat_temp; 
  }
  r1stat = r1stat/64;
  return r1stat;
}

void setup()
{
    
    TCCR0A = 0x02;  // CTC
    TCCR0B = 0x02;  // 9600000 / 8 = 1200000Hz = 0.83333us
    OCR0A  = delay_B; // (1+124) x 0.8333 = 104.166us
    // Set the ADC input to PB2/ADC1
    ADMUX |= (1 << MUX0);
    //ADMUX |= (1 << ADLAR);
 
    // Set the prescaler to clock/128 & enable ADC
    // At 9.6 MHz this is 75 kHz.
    // See ATtiny13 datasheet, Table 14.4.
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
    
    uart_DIR |= uart_TX_PIN;
    TX_PIN_1; 
}

void loop()
{
    char str[5];
    uint16_t val;
    int temp;

    val = adc_read();
    temp = map(val, 200, 840, -40, 120);
    
    itoa(val, str, 10);
    bit_bang_tx_str("ADC  = ");
    bit_bang_tx_str((char *)str);
    bit_bang_tx_str(" , ");

    itoa(temp, str, 10);
    bit_bang_tx_str("Temp = ");
    bit_bang_tx_str((char *)str);
    bit_bang_tx_str("C\r\n");
    delay(1000);
}
