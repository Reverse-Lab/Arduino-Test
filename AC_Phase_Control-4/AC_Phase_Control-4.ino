/*

AC Voltage dimmer with Zero cross detection
Author: Charith Fernanado Adapted by DIY_bloke
License: Creative Commons Attribution Share-Alike 3.0 License.
Attach the Zero cross pin of the module to Arduino External Interrupt pin
Select the correct Interrupt # from the below table 
(the Pin numbers are digital pins, NOT physical pins: 
digital pin 2 [INT0]=physical pin 4 and digital pin 3 [INT1]= physical pin 5)
check: <a href="http://arduino.cc/en/Reference/attachInterrupt">interrupts</a>

Pin    |  Interrrupt # | Arduino Platform
---------------------------------------
2      |  0            |  All -But it is INT1 on the Leonardo
3      |  1            |  All -But it is INT0 on the Leonardo
18     |  5            |  Arduino Mega Only
19     |  4            |  Arduino Mega Only
20     |  3            |  Arduino Mega Only
21     |  2            |  Arduino Mega Only
0      |  0            |  Leonardo
1      |  3            |  Leonardo
7      |  4            |  Leonardo
The Arduino Due has no standard interrupt pins as an iterrupt can be attached to almosty any pin. 

In the program pin 2 is chosen
*/
int AC_LOAD = 11;    // Output to Opto Triac pin
int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF
uint8_t pos=0;

const int grayPin[8]={3,4,5,6,7,8,9,10};

void grayPinSet(){
  for(int8_t i=0;i<8;i++)
  {
    pinMode(grayPin[i], INPUT);
  }
}

void setup()
{
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output
  
  Serial.begin(115200);
  grayPinSet();
  enc_update();
  
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
}

//the interrupt function must take no parameters and return nothing
void zero_crosss_int()  //function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms (200us)
  // Every zerocrossing thus: (50Hz)-> 10ms (1/2 Cycle) 
  // For 60Hz => 8.33ms (10.000/120)
  // 10ms=10000us
  // (10000us - 10us) / 128 = 75 (Approx) For 60Hz =>65

  int dimtime = (65*pos);    // For 60Hz =>65    
  delayMicroseconds(dimtime);    // Wait till firing the TRIAC    
  digitalWrite(AC_LOAD, HIGH);   // Fire the TRIAC
  delayMicroseconds(8.335);         // triac On propogation delay 
         // (for 60Hz use 8.33) Some Triacs need a longer period
  digitalWrite(AC_LOAD, LOW);    // No longer trigger the TRIAC (the next zero crossing will swith it off) TRIAC
  
}

void enc_update()
{
  int i;
  int enc[8];
  for(i=0;i<8;i++){
    enc[i] = 1 - digitalRead(grayPin[i]);
    Serial.print(enc[i]);
  }Serial.print(", ");
  
  uint8_t pos=0;
  pos= (enc[7]<<7);
  for(i=6;i>=0;i--)  
  {
    if( (enc[i]+bitRead(pos,i+1))==0 || (enc[i]+bitRead(pos,i+1))==2 )
    {
      pos|=(0<<i);
    }else if( (enc[i]+bitRead(pos,i+1))==1)
    {
      pos|=(1<<i);
    }
  }
  
  Serial.print(pos,BIN);Serial.print(", ");
  Serial.print(pos,DEC);Serial.print(", ");Serial.print(pos*1.40625);
  Serial.println();
  delay(10); 
}

void loop()  {
  enc_update();
}

  
