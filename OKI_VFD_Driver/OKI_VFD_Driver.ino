//This code is for the MSM9202-01 chip from OKI
#define clockpin 8
#define datapin 7
#define sspin 9 //slave select/chip select
#define bitorder MSBFIRST //arduino has MSB on left and LSB on right and the datasheet has these reversed so I reverse the bit order here in order to more 
                          //easily code the bit information how I see it in the datasheet
byte bitsofCharacter = 0;

void setup() {
pinMode(8, OUTPUT);
pinMode(7, OUTPUT);
pinMode(9, OUTPUT);

delayMicroseconds(300); //power up delay

sendData(0b00000010); //set P1 and P2 output port to low (This is to control other IO devices and LED; we don't want this feature)

sendData(0b11100110); //configure number of grids on display (15)

sendData(0b11101010); //sets Display duty cycle for contrast (8/16) / 0b11101010 (15/16

sendData(0b11001110); //test mode: light all

sendData(0b00001110); //normal mode

}

//simple function to send out a command
void sendData(unsigned char a){ 
digitalWrite(sspin, LOW);
delayMicroseconds(1);  
shiftOut(datapin, clockpin, bitorder, a);
digitalWrite(sspin, HIGH);
delayMicroseconds(60);
}

//Alternate version of ShiftOut() function that only shifts a bit at a time
void Shift1Out(char val){
digitalWrite(datapin, val);
digitalWrite(clockpin, HIGH);
digitalWrite(clockpin, LOW);
}

//simple function to send out data
void dcram(unsigned char b){
digitalWrite(sspin, LOW);
delayMicroseconds(1);
shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM data write (COM1)
shiftOut(datapin, clockpin, bitorder, b); //character data
digitalWrite(sspin, HIGH);
delayMicroseconds(60);
}

void PrintString(String l){ //to reverse bit order from LSB to MSB or vice versa you either decrement or increment in the for-loops
  digitalWrite(sspin, LOW);
  shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM command starting at COM1
for(int i=0; i<l.length(); i++){ //length() returns the length of the string
  char Character = l.charAt(i); //the number in charAt returns the respective character of the string i.e. 0 = H, then that character is put into another variable
for(int k=0; k<8; k++){
  bitsofCharacter = bitRead(Character, k); //bitRead reads the bit from the character, which will be 0-7 starting at the LSB (rightmost) bit 
  Shift1Out(bitsofCharacter); //shifts out the incoming bits one at a time         
}
}
  digitalWrite(sspin, HIGH);
}

/*
 * These following functions are for CGRAM and clearing the screen but they aren't tested thoroughly so may not work as expected however might be useful for someone
 * as reference. CGRAM is also quite interesting because it allows you to turn on or off any individual dot in the grid. You do this by first issuing a CGRAM command
 * to let the chip know you are storing information into the RAM area you selected (RAM0-7), then shifting out 5 bytes of column data, and finally calling that RAM
 * data through the DCRAM command.
 */

/*
//This function uses the CGRAM command to insert blank data into RAM0 and then we can call RAM0 with DCRAM command to clear the screen
//this wont clear/overwrite any current DCRAM data or ADRAM data, you will need to use their respective commands to clear their respective registers to clear the screen
void clear_screen(void){
digitalWrite(sspin, LOW);
delayMicroseconds(1);
shiftOut(datapin, clockpin, bitorder, 0b00000100); //CGRAM data write (RAM0)
delayMicroseconds(10);
shiftOut(datapin, clockpin, bitorder, 0b00000000); //1st column data
delayMicroseconds(10);
shiftOut(datapin, clockpin, bitorder, 0b00000000); //2nd column data
delayMicroseconds(10);
shiftOut(datapin, clockpin, bitorder, 0b00000000); //3rd column data
delayMicroseconds(10);
shiftOut(datapin, clockpin, bitorder, 0b00000000); //4th column data
delayMicroseconds(10);
shiftOut(datapin, clockpin, bitorder, 0b00000000); //5th column data
delayMicroseconds(10);
//dcram(0b00000000); //Displays what we put in RAM0
digitalWrite(sspin, HIGH);
delayMicroseconds(60);

}
*/
/*
void clear_VFD(void){ 
digitalWrite(sspin, LOW);
delayMicroseconds(1);
shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM command
for(int i = 0; i < 16; i++){
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //blank data to clear tube
}
delayMicroseconds(60);
digitalWrite(sspin, HIGH);
delayMicroseconds(60);
digitalWrite(sspin, LOW);
shiftOut(datapin, clockpin, bitorder, 0b00001100); //ADRAM command
for(int i = 0; i < 16; i++){
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //blank data to clear tube
}
delayMicroseconds(60);
digitalWrite(sspin, HIGH);
delayMicroseconds(60);
}
*/

void loop() {
  delay(10);
  sendData(0b11001110); //All on
  delay(1000);
  sendData(0b10001110); //All off
  delay(500);
  sendData(0b11001110); //All on
  delay(1000);
  sendData(0b00001110); //normal mode (need to call this to write characters)
  delay(1000);
  digitalWrite(sspin, LOW);
  delayMicroseconds(1);
  shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM command starting at COM1
  shiftOut(datapin, clockpin, bitorder, 0b00010010); //h
  shiftOut(datapin, clockpin, bitorder, 0b10100010); //e
  shiftOut(datapin, clockpin, bitorder, 0b00110010); //l
  shiftOut(datapin, clockpin, bitorder, 0b00110010); //l
  shiftOut(datapin, clockpin, bitorder, 0b11110010); //o
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //space
  shiftOut(datapin, clockpin, bitorder, 0b11101010); //w
  shiftOut(datapin, clockpin, bitorder, 0b11110010); //o
  shiftOut(datapin, clockpin, bitorder, 0b01001010); //r
  shiftOut(datapin, clockpin, bitorder, 0b00110010); //l
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //space MAP OUT THE COM1-16 DIGITPINS SO U KNOW WHERE A LETTER WILL BE. EVERY SCREEN IS DIFFERENT
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //space
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //space
  shiftOut(datapin, clockpin, bitorder, 0b00000100); //space
  shiftOut(datapin, clockpin, bitorder, 0b00100010); //d
  digitalWrite(sspin, HIGH);
  delay(2000);
  PrintString("               "); //if you dont call this before the next line, any grid slot not overwritten will still have leftover data i.e. the last "d" will be present 
  digitalWrite(sspin, LOW);
  delayMicroseconds(1);
  shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM command starting at COM1
  shiftOut(datapin, clockpin, bitorder, 0b11001010); //s
  shiftOut(datapin, clockpin, bitorder, 0b11000010); //c
  shiftOut(datapin, clockpin, bitorder, 0b10010010); //i
  shiftOut(datapin, clockpin, bitorder, 0b11000010); //c
  shiftOut(datapin, clockpin, bitorder, 0b10011010); //y
  shiftOut(datapin, clockpin, bitorder, 0b01110010); //n
  shiftOut(datapin, clockpin, bitorder, 0b10010010); //i
  shiftOut(datapin, clockpin, bitorder, 0b11000010); //c
  shiftOut(datapin, clockpin, bitorder, 0b10000010); //a
  shiftOut(datapin, clockpin, bitorder, 0b00110010); //l
  digitalWrite(sspin, HIGH);
  delay(2000);
  PrintString("               ");
  digitalWrite(sspin, LOW);
  delayMicroseconds(1);
  shiftOut(datapin, clockpin, bitorder, 0b00001000); //DCRAM command starting at COM1
  shiftOut(datapin, clockpin, bitorder, 0b10010010); //i
  shiftOut(datapin, clockpin, bitorder, 0b01110010); //n
  shiftOut(datapin, clockpin, bitorder, 0b01101010); //v
  shiftOut(datapin, clockpin, bitorder, 0b10100010); //e
  shiftOut(datapin, clockpin, bitorder, 0b01110010); //n
  shiftOut(datapin, clockpin, bitorder, 0b00101010); //t
  shiftOut(datapin, clockpin, bitorder, 0b10010010); //i
  shiftOut(datapin, clockpin, bitorder, 0b01110010); //n
  shiftOut(datapin, clockpin, bitorder, 0b11100010); //g
  digitalWrite(sspin, HIGH);
  delay(2000);
  PrintString("               ");
  PrintString("Enjoy"); //this is shorted than the next string so we dont need to necessarly clear the screen as the next string will automatically overwrite it anyway
  delay(2000);
  PrintString("this free"); //this is longer than the next string so I will need to clear the screen after displaying this
  delay(2000);
  PrintString("               "); //clears the display (I put in 15 spaces because I have 15 grids)
  PrintString("code!");
  delay(2000);
  PrintString("               ");
  
}
