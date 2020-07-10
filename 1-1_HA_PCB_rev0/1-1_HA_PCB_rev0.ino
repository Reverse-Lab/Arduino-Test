#include <Key.h>
#include <Keypad.h>
#include <EEPROM.h>

#include "Wire.h"

#define HT16K33_ADDRESS         0x70
#define HT16K33_ON              0x21  // Commands
#define HT16K33_STANDBY         0x20
#define HT16K33_DISPLAYON       0x81
#define HT16K33_DISPLAYOFF      0x80
#define HT16K33_BLINKON         0x85 // Blink is off (00), 2 Hz (01), 1 Hz (10), or 0.5 Hz (11) for bits (21) 
#define HT16K33_BLINKOFF        0x81
#define HT16K33_DIM             0xE0 | 0x08 // Set dim from 0x00 (1/16th duty ccycle) to 0x0F (16/16 duty cycle)

static const byte numberTable[] =
{
  0x3F, // 0 = 0011 1111
  0x06, // 1 = 0000 0110
  0x5B, // 2 = 0101 1011
  0x4F, // 3 = 0100 1111
  0x66, // 4 = 0110 0110
  0x6D, // 5 = 0110 1101
  0x7D, // 6 = 0111 1101
  0x07, // 7 = 0000 0111
  0x7F, // 8 = 0111 1111
  0x6F, // 9 = 0110 1111
  0x80, // decimal point
  0x00, // blank  1000 0000
  0x40, // minus sign 0100 0000
  0x77,  // A = 13
  0x7F,  // B = 14
  0x2D,  // C = 15
  0x6F,  // D = 16
  0x3D,  // E = 17
  0x35,  // F = 18
  0x5F,  // G = 19
  0x76,   // H = 20
  0x42,   // I = 21
  0x6A,   // J = 22
  0x00,   // No K!
  0x2C,   // L = 24
  0x00,   // No M!
  0x67,   // N = 26
  0x6F,   // O = 27
  0x37,   // P = 28
  0x57,   // Q = 29
  0x77,   // R = 30
  0x5D    // S = 31
};

#define display1 1
#define display2 2
#define display3 3
#define display4 4
//----------------------------------------------------------------------//
#define DI2 22
#define DI3 23
#define DI4 24
#define DI5 25
#define DI6 26

#define DO13 53
#define DO14 52
#define DO15 51
#define DO16 50
#define DO17 10

#define LED1 54
#define LED2 55
#define LED3 56
#define LED4 57
#define LED5 58
#define LED6 59

#define Buz 12

const byte numRows = 2;
const byte numCols = 7;
int FND;
const int E_addr = 0; //EEPROM FND Count Address

char keymap[numRows][numCols]={
  {'1', 'B', 'C', 'D', 'E', 'F', 'G'},
  {'A', '2', '3', '4', '5', '6', '7'}
};
byte rowPins[numRows] = {28, 29};
byte colPins[numCols] = {63, 64, 65, 66, 67, 68, 69};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

class Flasher
{
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time
 
    // These maintain the current state
    int ledState;                     // ledState used to set the LED
    unsigned long previousMillis;      // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);     
      
    OnTime = on;
    OffTime = off;
    
    ledState = LOW; 
    previousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);      // Update the actual LED
    }
  }
};

void writeInteger(uint8_t dsply, int data)
{
  char string[10] = "";                             // define character array to hold the digits
  itoa(data, string);                               // get ascii character string representation of the integer to be displayed
  uint8_t length = strlen(string);                  // get the length of the string; number of digits in integer
  uint8_t blanks = 4 - length;                      // how many blanks do we have?
  
  if (length > 4) return;                           // if length greater than 4 digits we can't display on a four-digit display!
  for (uint8_t digit = 0; digit < blanks; digit++)  // scroll through each digit to determine what to write to the display
  {
      writeDigit(dsply, digit + 1, 11, 0);          // clear digit wherever there are blanks
  }

  for (uint8_t digit = 0; digit < 4; digit++)       // scroll through each digit to determine what to write to the display
  {
     char ch = string[digit];                      // get the ascii character of the next string segment
      if (ch == '-') {
      writeDigit(dsply, digit + 1 + blanks, 12, 0); // check if negative sign needed
      } 
      else {                                        // character must be a digit
      ch -= '0';                                    // convert it to an integer
      writeDigit(dsply, digit + 1 + blanks, ch, 0); // write it to the display; right justify the integer
      } 
  }
}

void writeFloat(uint8_t dsply, float data, uint8_t dp)
{
  char string[10] = "";  // define character array to hold the digits
  int datanew = 0;

  switch (dp)
  {
    case 0:
    datanew = (int )(1.*data);
    break;

    case 1:
    datanew = (int )(10.*data);
    break;

    case 2:
    datanew = (int )(100.*data);
    break;

    case 3:
    datanew = (int )(1000.*data);
    break;
   }

  itoa(datanew, string);                                    // get ascii character string representation of the integer to be displayed
  uint8_t length = strlen(string);                          // get the length of the string; number of digits in integer
  uint8_t blanks = 4 - length;                              // how many blanks do we have?

  if (length > 4) return;                                   // if length greater than 4 digits we can't display on a four-digit display!

// scroll through each digit to determine what to write to the display
  for (uint8_t digit = 0; digit < blanks; digit++)            // first the blanks
  {
          if( (digit + 1) == (4 - dp) ) {                   // handle special case where blank coincides with decimal point
            writeDigit(dsply, digit + 1, 0, 0x80);          // add leading zero before decimal place
          }
          else {
            writeDigit(dsply, digit + 1, 11, 0x00);         // otherwise clear digit wherever there are blanks
          }
  }

  for (uint8_t digit = 0; digit < 4; digit++)               // now the characters to determine what to write to the display
  {
      char ch = string[digit];                              // get the ascii character of the next string segment

      if (ch == '-') {
        if((digit + 1 + blanks) == (4 - dp) ) {
          writeDigit(dsply, digit + 1 + blanks,  0, 0x80);  // check if negative sign needed, add a decimal point
          writeDigit(dsply, digit + 0 + blanks, 12, 0x00);  // add a leading zero
        }
        else {
          writeDigit(dsply, digit + 1 + blanks, 12, 0x00);  // check if negative sign needed, no decimal point
        }
      }
      else  {                                               // character must be a digit
        ch -= '0';                                          // convert it to an integer
        if((digit + 1 + blanks) == (4 - dp) ) {
          writeDigit(dsply, digit + 1 + blanks, ch, 0x80);  // write it to the display with decimal point; right justify the integer
        } 
        else {
          writeDigit(dsply, digit + 1 + blanks, ch, 0x00);  // write it to the display; right justify the integer
        } 
     }
  }
}

void writeDigit(uint8_t dsply, uint8_t digit, uint8_t data, uint8_t dp) 
{
  if(dsply == 1) {
    digit = (digit - 1)*2 + 0; 
  } 
  if(dsply == 2) {
    digit = (digit - 1)*2 + 8 ;
  }
  if(dsply == 3) {
    digit = (digit - 1)*2 + 1;
  }
  if(dsply == 4) {
    digit = (digit - 1)*2 + 9;
  }
  writeByte(HT16K33_ADDRESS, digit, numberTable[data] | dp);
}

void clearDsplay(int dsply) 
{
  for(int i = 0; i < 8; i++)  {
  writeDigit(dsply, i, 11, 0);  // Clear display, 11 is blank in the numberTable above
  }
}

void initHT16K33()
{
  writeCommand(HT16K33_ADDRESS, HT16K33_ON);         // Turn on system oscillator
  writeCommand(HT16K33_ADDRESS, HT16K33_DISPLAYON);  // Display on
  writeCommand(HT16K33_ADDRESS, HT16K33_DIM);        // Set brightness
}

void blinkHT16K33(int time) 
{
  writeCommand(HT16K33_ADDRESS, HT16K33_BLINKON);  // Turn on blink
  delay(1000*time);
  writeCommand(HT16K33_ADDRESS, HT16K33_BLINKOFF);  // Turn on blink
}

 /* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
         i = 0;
       do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
       } while ((n /= 10) > 0);     /* delete it */

     if (sign < 0)
         s[i++] = '-';
         s[i] = '\0';
         reverse(s);
} 

void reverse(char s[])
 {
     int i, j;
     char c;
 
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
        // Wire.h read and write protocols
void writeCommand(uint8_t address, uint8_t command)
{
 Wire.beginTransmission(address);  // Initialize the Tx buffer
 Wire.write(command);              // Put command in Tx buffer
 Wire.endTransmission();           // Send the Tx buffer
}

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
 Wire.beginTransmission(address);  // Initialize the Tx buffer
 Wire.write(subAddress);           // Put slave register address in Tx buffer
 Wire.write(data);                 // Put data in Tx buffer
 Wire.endTransmission();           // Send the Tx buffer
}

uint8_t readByte(uint8_t address, uint8_t subAddress)
{
 uint8_t data; // `data` will store the register data  
 Wire.beginTransmission(address);         // Initialize the Tx buffer
 Wire.write(subAddress);                  // Put slave register address in Tx buffer
 Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
 Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address 
 data = Wire.read();                      // Fill Rx buffer with result
 return data;                             // Return data read from slave register
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
 Wire.beginTransmission(address);   // Initialize the Tx buffer
 Wire.write(subAddress);            // Put slave register address in Tx buffer
 Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
 uint8_t i = 0;

 Wire.requestFrom(address, count);  // Read bytes from slave register address 

    while (Wire.available()) {
    dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
} 

Flasher led1(53, 200, 300);
Flasher led2(52, 500, 300);
Flasher led3(51, 750, 300);
Flasher led4(50, 1000, 300);
Flasher led5(10, 1200, 300);
 


void setup() {
  
  pinMode(28, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP); // 키패드 풀업
  
  pinMode(DI2, INPUT);
  pinMode(DI3, INPUT);
  pinMode(DI4, INPUT);
  pinMode(DI5, INPUT);
  pinMode(DI6, INPUT);


  pinMode(DO14, OUTPUT);
  pinMode(DO15, OUTPUT);
  pinMode(DO17, OUTPUT);
  pinMode(DO16, OUTPUT);
  pinMode(DO13, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  pinMode(Buz, OUTPUT);
  
  Serial.begin(115200);
  Wire.begin();
  
  initHT16K33();  // initialize display
  clearDsplay(display1);  // clear display 1
  //clearDsplay(display2);  // clear display 2

  writeInteger(display1, 8888); // Test display, turn on all segments of display 1
  blinkHT16K33(2); // Blink for 2 seconds
  
  Serial.println();
 
  Serial.println("Reading FND from EEPROM...");
    byte hiByte = EEPROM.read(E_addr); // read(주소)
    byte loByte = EEPROM.read(E_addr+1); // read(주소)
    FND = word(hiByte, loByte);
    FND++;
    Serial.println("EEPROM Address : " + String(E_addr) + "\t Value : " + FND);
   
    delay(10);
  
}

void loop() {

  led1.Update();
  led2.Update(); 
  led3.Update();
  led4.Update();
  led5.Update();
      
  char btn = myKeypad.getKey();
  if(btn != NO_KEY) {
    
    digitalWrite(Buz, HIGH);
    delay(40);
    digitalWrite(Buz, LOW);
    delay(10);
    Serial.print("FND Number : "); Serial.println(FND);
    byte hiByte = highByte(FND);
    byte loByte = lowByte(FND);
    EEPROM.write(E_addr, hiByte); // write(주소, 값)
    Serial.println("EEPROM Address : " + String(E_addr) + "\t Value : " + hiByte);
    EEPROM.write(E_addr+1, loByte); // write(주소, 값)
    Serial.println("EEPROM Address : " + String(E_addr+1) + "\t Value : " + loByte);
    Serial.println();
    delay(10);
    FND++;
  }
 
  writeInteger(display1, FND);   // write integer count to display 1
  
 if(btn == '1')
  {
    digitalWrite(LED1, HIGH);
    delay(100);
  }
  if(btn == 'A')
  {
    digitalWrite(LED1, LOW);
    delay(100);
  }

 if(btn == '2')
  {
    digitalWrite(LED2, HIGH);
    delay(100);
  }
  if(btn == 'B')
  {
    digitalWrite(LED2, LOW);
    delay(100);
  }

 if(btn == '3')
  {
    digitalWrite(LED3, HIGH);
    delay(100);
  }
  if(btn == 'C')
  {
    digitalWrite(LED3, LOW);
    delay(100);
  }

 if(btn == '4')
  {
    digitalWrite(LED4, HIGH);
    delay(100);
  }
  if(btn == 'D')
  {
    digitalWrite(LED4, LOW);
    delay(100);
  }


 if(btn == '5')
  {
    digitalWrite(LED5, HIGH);
    delay(100);
  }
  if(btn == 'E')
  {
    digitalWrite(LED5, LOW);
    delay(100);
  }

  if(btn == '6')
  {
    digitalWrite(LED6, HIGH);
    delay(100);
  }
  if(btn == 'F')
  {
    digitalWrite(LED6, LOW);
    delay(100);
  }
  if(btn == '7')
  {
    FND = 0;
    delay(10);
  }

}
