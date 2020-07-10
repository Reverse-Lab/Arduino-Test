//#include <7166.h>

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
#define ICR   0x40  // 0b0100 0000
#define OCCR  0x80  // 0b1000 0000
#define QR    0xC0  // 0b1100 0000

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

void cdcs_7166(boolean CnD, boolean nCS ) {
  digitalWrite(ic_CnD,CnD);
  digitalWrite(ic_nCS,nCS);
  delayMicroseconds(50);
}
void ctrl_7166( byte control ){ 
  DDRC = 0xFF; // sets Arduino Digital pins 0-7 all as outputs
  PORTC = control;
    latchWR_7166();
}
void latchWR_7166(){
  delayMicroseconds(20);
  digitalWrite(ic_nWR,0);
  delayMicroseconds(20);
  digitalWrite(ic_nWR,1);
  delayMicroseconds(20);
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

void init_7166() {
  EncoderCount = 0;  
  DDRC = 0xFF; // sets Arduino Digital pins 0-7 all as outputs
  digitalWrite(ic_nRD,1);
  delayMicroseconds(20);
  cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR, ICR, OCCR, QR & to read OSR. Only needs setting once, so moved out of ctrl_7166()
  ctrl_7166(0x20);    //Performs master reset
  ctrl_7166(0x04);   //Sub-reset
  ctrl_7166(ICR|0x18);    //Enables A/B, sets up pin 3-4
  ctrl_7166(OCCR|0x34); //Divide by n mode   0x04
  ctrl_7166(QR|0x03);   //x4 quadrature mode   0x04

  ctrl_7166( 1 ); // tell MCR to load PR
  cdcs_7166(0,0); // somewhat rare usage of 0,0: read OL and write PR
  write_7166( LS7166INISET );
  cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR
  ctrl_7166( 8 );  // xfer PR to CNTR
}

//  latchRD_7166() -- unlike latching out, which is done after, reading
//  gets the data inside a signal sandwich
byte latchRD_7166(){
  byte dataread=0;
  // nRD should already be high
  digitalWrite(ic_nRD,0); // enable reading
  delayMicroseconds(50);
  dataread=PINC;
  // read 1st byte, the LSB
  delayMicroseconds(50);
  digitalWrite(ic_nRD,1);
  delayMicroseconds(50);
  return dataread;
}

unsigned long read_7166(){
  unsigned long tmp=0, Data=0;
  // Tell MCR we want to read the OL
  DDRC = 0xFF; // sets Arduino Digital pins 0-7 all as outputs
  //cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR
  ctrl_7166(3);   // send 3 to MCR, transfers CNTR to OL
  digitalWrite(ic_CnD, LOW);
  digitalWrite(ic_nRD, HIGH);
  digitalWrite(ic_nWR, HIGH);
  digitalWrite(ic_nCS, LOW);  // Select chip
  // set up the OL for reading
  //cdcs_7166(0,0);
  DDRC = 0x00; // sets Arduino Digital pins 0-7 all as inputs
  // read 1st byte, LSB
  tmp = latchRD_7166(); // byte 0, lsB
  Data |= tmp;
  delayMicroseconds(10);
  // read 2nd byte, the middle byte
  tmp = latchRD_7166(); // byte 1
  tmp = tmp << 8; // shift over 8 bits
  Data |= tmp;
  delayMicroseconds(10);
  // read 3rd byte, the MSB
  tmp = latchRD_7166(); // byte 1
  tmp = tmp << 16;     // shift over 16 bits this tmp is MSB
  Data |= tmp;
  digitalWrite(ic_nCS, HIGH);  // Unselect chip
  return Data;
}
/////////////////////////////////////////////////////////////////////////
void loop() {

  int vol = analogRead(val);
  int pwm = map(vol, 0, 1023, 0, 255);
  analogWrite(in2, pwm);
  
  EncoderCount = read_7166();
  Serial.print("Encoder Count = ");
  Serial.println(EncoderCount);
  delay(500);
}
