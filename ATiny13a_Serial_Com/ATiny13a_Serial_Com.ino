//https://blog.naver.com/kiatwins/220804644450

#define BAUD        9600
#define uart_DIR    DDRB
#define uart_OUT    PORTB
#define uart_TX_PIN 0x02  // PB1 (MISO)
#define TX_PIN_1    uart_OUT |= uart_TX_PIN
#define TX_PIN_0    uart_OUT &=~uart_TX_PIN
#define delay_BAUD  (1200000 / BAUD) - 1
#define adc         0x04 // PB2
char str[5];
unsigned int val;
    
void bit_bang_tx(unsigned char data) {
    unsigned char i;
    TX_PIN_0;                    // Start bit
    TCNT0=0;
    while(TCNT0<delay_BAUD);
    for(i=0; i<8; i++) {        // LSBFIRST
        if (data & (0x01 << i)) TX_PIN_1;
        else TX_PIN_0;
        while(TCNT0<delay_BAUD);
    }
    TX_PIN_1;                    // Stop bit
    while(TCNT0<delay_BAUD);
}

void bit_bang_tx_str(char *str) {
    while(*str) bit_bang_tx(*str++);
}

void setup()
{
    
    TCCR0A = 0x02;  // CTC
    TCCR0B = 0x02;  // 9600000 / 8 = 1200000Hz = 0.83333us
    OCR0A  = delay_BAUD; // (1+124) x 0.8333 = 104.166us
    
    uart_DIR |= uart_TX_PIN;
    TX_PIN_1; 

    pinMode(adc, INPUT);
}

void loop()
{
    val = analogRead(adc);
    utoa(val, str, 10);
    bit_bang_tx_str("ADC Value = ");
    bit_bang_tx_str((char *)str);
    bit_bang_tx_str("\r\n");
    delay(1000);
}
