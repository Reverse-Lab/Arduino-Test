// https://github.com/cacycleworks/Arduino_LS7166/blob/master/ArduinoDRO.ino
// Quadrature counter decoder LS7166 control lines
byte ic_nRD = 27; //PG1
byte ic_CnD = 43; //PA1
byte ic_nWR = 26; //PG0
byte ic_nCS = 44; //PA0

#define in1   5   //PE5
#define in2   15  //PB7
#define en    16  //PG3
#define val   45  //PF0

#define C2CMINMICRON 221200
#define LS7166INISET 0
/////////////////////////////////////////////////////////////////////////
// Control registers are top two bits of the byte, MCR = 0
#define MCR 0
#define ICR 1
#define OCCR 2
#define QR 3
//Readable registers
#define OL 0
#define OSR 1

unsigned long EncoderCount = 0; // this is current encode position read from LS7166

void setup() {
  pinMode(ic_nRD, OUTPUT);
  pinMode(ic_CnD, OUTPUT);
  pinMode(ic_nWR, OUTPUT);
  pinMode(ic_nCS, OUTPUT);

  digitalWrite(ic_nRD, HIGH);
  digitalWrite(ic_CnD, HIGH);
  digitalWrite(ic_nWR, HIGH);
  digitalWrite(ic_nCS, HIGH);
  delayMicroseconds(50);
  
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (en, OUTPUT);
  pinMode (val, INPUT);

  digitalWrite(en, HIGH);
  digitalWrite(in1, LOW);

  Serial.begin(115200);
  
  EncoderCount=0;
  init_7166();
}

void latchWR_7166(){
  delay_t();
  digitalWrite(ic_nWR, LOW);
  delay_t();
  digitalWrite(ic_nWR, HIGH);
}

void write_7166(unsigned long Data ){
  DDRC = 0xFF; // sets Arduino Digital pins 0-7 all as outputs
  PORTC =  (unsigned char)Data;
    latchWR_7166();
    
  Data >>= 8;
  PORTC =  (unsigned char)Data;
    latchWR_7166();

  Data >>= 8;
  PORTC =  (unsigned char)Data;
    latchWR_7166();
}

/* write_ctrl: writes a byte to the given control register */
void write_ctrl(unsigned char reg, unsigned char bte){
  digitalWrite(ic_nCS, LOW);  // NCS = 0;  //Select chip
  digitalWrite(ic_nRD, HIGH); // NRD = 1;
  digitalWrite(ic_CnD, HIGH); // C_ND = 1; //Setup to write to control register
  DDRC=0xFF;                  // bus_out(op);
  unsigned char op = (reg << 6) | bte;
  PORTC = op;
  latchWR_7166();             //write value
  digitalWrite(ic_nCS, HIGH); // NCS = 1;  //Unselect chip 
} 

void init_7166() {
  write_ctrl(MCR,0x20);   //Performs master reset
  write_ctrl(MCR,0x04); //Sub-reset
  write_ctrl(ICR,0x18);   //Enables A/B, sets up pin 3-4
  write_ctrl(OCCR, 0x34); //Divide by n mode  0x04
  write_ctrl(QR,0x03);  //x4 quadrature mode  0x04
  //0x3840->Pr
  write_7166(0x3840);   //Upper limit  
}

byte latchRD_7166(){
  byte dataread=0;        // nRD should already be high
  DDRC = 0x00;
  digitalWrite(ic_nRD,LOW); // enable reading
  delay_t();
  dataread=PINC;          // read 1st byte, the LSB
  delay_t();
  digitalWrite(ic_nRD,HIGH);
  return dataread;
}

unsigned long read_7166(unsigned char reg){
  unsigned long read_value;  // Tell MCR we want to read the OL
  if (reg == OSR){
    digitalWrite(ic_nCS, LOW);  // Select chip
    digitalWrite(ic_CnD, HIGH);
    digitalWrite(ic_nWR, HIGH);
    read_value = latchRD_7166();
    digitalWrite(ic_nCS, HIGH);  // Unselect chip
    return read_value;
  }
  if (reg == OL){
    write_ctrl(MCR,0x03);  //Prepare for read
    digitalWrite(ic_CnD, LOW);
    digitalWrite(ic_nRD, HIGH);
    digitalWrite(ic_nWR, HIGH);
    digitalWrite(ic_nCS, LOW);  // Select chip
    unsigned long read_byte;
    read_byte = latchRD_7166(); // byte 0, lsB
    read_value = read_byte;
    delay_t();
    read_byte = latchRD_7166(); // byte 1, lsB
    long tmp = read_byte;
    tmp = tmp << 8;
    read_value |= tmp;
    delay_t();
    read_byte = latchRD_7166(); // byte 2, lsB
    tmp = read_byte;
    tmp = tmp << 16;
    read_value |= tmp;
    digitalWrite(ic_nCS, HIGH);  // Select chip
    return read_value;
  }
  //no other regs can be read  
}
/////////////////////////////////////////////////////////////////////////
void loop() {

  int vol = analogRead(val);
  int pwm = map(vol, 0, 1023, 0, 255);
  analogWrite(in2, pwm);
  
  EncoderCount = read_7166(OL);
  Serial.print("Encoder Count = ");
  Serial.println(EncoderCount);
  delay(500);
}

void delay_t() {
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}
