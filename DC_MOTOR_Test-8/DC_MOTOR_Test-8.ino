// https://github.com/cacycleworks/Arduino_LS7166/blob/master/ArduinoDRO.ino
// Quadrature counter decoder LS7166 control lines
byte ic_nRD = (PING,1);  //27; //PG1
byte ic_CnD = (PINA,1); //43; //PA1
byte ic_nWR = (PING,0); //26; //PG0
byte ic_nCS = (PINA,0); //44; //PA0

#define in1   5   //PE5
#define in2   15  //PB7
#define en    16  //PG3
#define val   45  //PF0

#define C2CMINMICRON 221200
#define LS7166INISET 0
/////////////////////////////////////////////////////////////////////////
// Control registers are top two bits of the byte, MCR = 0
#define ICR 0x40
#define OCCR 0x80
#define QR 0xC0

unsigned long EncoderCount; // this is current encode position read from LS7166

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
/////////////////////////////////////////////////////////////////////////
//   cdcs_7166(): every time you want to do something with the 7166, CD and CS
//      have to be set or reset, so I made a function for it
void cdcs_7166(boolean CnD, boolean nCS ) {
  digitalWrite(ic_nCS,nCS); // 0인경우 7166 접근 허용
  //delay_t(100);;
  digitalWrite(ic_CnD,CnD); // 0인경우 DATA, 1인경우 CONTROL
  //delay_t(100);;
}
/////////////////////////////////////////////////////////////////////////
//  ctrl_7166() would formerly set cdcs(1,0) in here, however let's move that to the caller
void ctrl_7166( byte control ){ 
  DDRC = 0xff; // sets Arduino Digital pins 0-7 all as outputs
  PORTC = control;
    latchWR_7166();
}
/////////////////////////////////////////////////////////////////////////
//  sends WR pulse after the command to tell the 7166 to latch in the data
void latchWR_7166(){
  //delay_t(100);;
  digitalWrite(ic_nWR,0); // 0인경우 DATA 기록 가능
  delay_t(10);;
  digitalWrite(ic_nWR,1); // 1 인경우 DATA 기록 중단
  //delay_t(100);;
}
/////////////////////////////////////////////////////////////////////////
//  this function ASSumes you already handled CnD, nCS before hand!!!
void write_7166(unsigned long Data ){
  DDRC = 0xff; // sets Arduino Digital pins 0-7 all as outputs
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
  EncoderCount = 0;   // this is encoder's position
  //  Data port manipulation
  // from: http://www.arduino.cc/en/Reference/PortManipulation
  // and also http://hekilledmywire.wordpress.com/2011/02/23/direct-port-manipulation-using-the-digital-ports-tutorial-part-3/
  DDRC = 0xff; // sets Arduino Digital pins 0-7 all as outputs
  digitalWrite(ic_nWR,1);  // 0 인경우 DATA 읽기 가능, 1인경우 DATA 읽기 중단
  delay_t(60);;
  // see http://hades.mech.northwestern.edu/index.php/Using_the_LS7166_Quadrature_Counter for some hints
  cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR, ICR, OCCR, QR & to read OSR. Only needs setting once, so moved out of ctrl_7166()
  ctrl_7166(0x20);    //Performs master reset
  //ctrl_7166(0x10);   //Sub-reset
  //ctrl_7166(0x04);   //Sub-reset
  //ctrl_7166(0x02);   //24bit out
  ctrl_7166(ICR|0x00);    //Enables A/B, sets up pin 3-4
  ctrl_7166(OCCR|0x00); //Divide by n mode   0x04
  ctrl_7166(QR|0x00);   //x4 quadrature mode   0x04
  ctrl_7166(ICR|0x08);    //Enables A/B, sets up pin 3-4

  //ctrl_7166( 1 ); // tell MCR to load PR
  
  //cdcs_7166(0,0); // somewhat rare usage of 0,0: read OL and write PR
  //write_7166( LS7166INISET );
  //cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR
  //ctrl_7166( 8 );  // xfer PR to CNTR

  cdcs_7166(1,1);  //-----------7-7 추가
}

//  latchRD_7166() -- unlike latching out, which is done after, reading
//  gets the data inside a signal sandwich
byte latchRD_7166(){
  byte dataread=0;
  // nRD should already be high 읽기 중단이 유지 되어야 함
  digitalWrite(ic_nRD,0); // enable reading 읽기 가능으로 전환
  delay_t(10);
  dataread=PINC;
  // read 1st byte, the LSB
  delay_t(10);
  digitalWrite(ic_nRD,1); // 읽기 중단으로 변경
  //delay_t(10);
  return dataread;
}

unsigned long read_7166(){
  unsigned long tmp=0, Data=0;
  // Tell MCR we want to read the OL
  DDRC = 0xff; // sets Arduino Digital pins 0-7 all as outputs
  cdcs_7166(1,0); // C/D /CS have to be 1,0 to write to MCR
  ctrl_7166(0x03);   // send 3 to MCR, transfers CNTR to OL
  //cdcs_7166(1,1);
  // set up the OL for reading
  cdcs_7166(0,0);
  DDRC = 0x00; // sets Arduino Digital pins 0-7 all as inputs
  // read 1st byte, LSB
  tmp = latchRD_7166(); // byte 0, lsB
  Data |= tmp;     //  << 8 추가
  // read 2nd byte, the middle byte
  tmp = latchRD_7166(); // byte 1
  tmp = tmp << 8; // shift over 8 bits
  Data |= tmp;
  // read 3rd byte, the MSB
  tmp = latchRD_7166(); // byte 1
  tmp = tmp << 16;     // shift over 16 bits this tmp is MSB
  Data |= tmp;

  cdcs_7166(1, 1); //-----------------추가
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

void delay_t(int t) {
  int i;
  for(i=0; i<t; i++){ 
  asm("nop");
  }
}
