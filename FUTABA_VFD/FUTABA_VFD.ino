// Arduino FUTABA VFD Driver Library
// Author: Peter Loron, peterl at standingwave.org
//
// v0.1 - 27 SEP 2008
//
// Developed for the Futaba NA202MD13AA VFD
//
// Information gathered from http://wiki.ovccorp.com/index.php?pagename=Futaba%20NA202MD13AA
//

//define IO pins
int pin_data = 2;
int pin_clock = 3;
int pin_enable = 4;
boolean enableHigh;

void setup() 
{ 
  //setup control pins
  pinMode(pin_data, OUTPUT);
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_enable, OUTPUT);

  digitalWrite(pin_clock, HIGH);
  digitalWrite(pin_enable, LOW);
  digitalWrite(pin_data, LOW);

  digitalWrite(13,HIGH);

  enableHigh = false;
}

///////////////////////////////////////////////////////////////

void loop() 
{ 
  for(int i = 0; i < 20; i++) {
    sendColumn(i);
  }

  delayMicroseconds(100);
}

//writes out a column
void sendColumn(int col) {
  boolean lastBit = false;

  //char 1
  for(int i = 0; i < 35; i++) {
    writeToDisplay(1);
  }

  //char 2
  for(int i = 0; i < 35; i++) {
    writeToDisplay(1);
  }

  //zero bits
  writeToDisplay(0);
  writeToDisplay(0);

  //column select
  for(int i = 0; i < 20; i++) {
    if(i == 19) { 
      lastBit = true; 
    }
    if(col == i) {
      writeToDisplay(1,lastBit);
    } 
    else {
      writeToDisplay(0,lastBit);
    }
  }

}

//writes a bit to the display
void writeToDisplay(int val) {
  writeToDisplay(val,false); 
}

void writeToDisplay(int val, boolean trigger) {

  //start the clock cycle
  digitalWrite(pin_clock, LOW);
  delayMicroseconds(1); 
  
  //drop the enable pin if it is high
  if(enableHigh) {
    digitalWrite(pin_enable, LOW);
    enableHigh = false;
  }

  //write out the data bit
  if(val == 0) {
    digitalWrite(pin_data, LOW);
  } 
  else {
    digitalWrite(pin_data, HIGH);
  }

  delayMicroseconds(1);

  //raise the clock pin
  digitalWrite(pin_clock, HIGH);

  //if this is the last bit, trigger the shift by raising the enable pin?
  if(trigger) {
    digitalWrite(pin_enable, HIGH);
    enableHigh = true;
  }

  digitalWrite(pin_data, LOW);
}
