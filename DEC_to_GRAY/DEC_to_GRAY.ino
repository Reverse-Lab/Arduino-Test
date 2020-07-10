

const int ledPin = 13;      // the pin that the LED is attached to

unsigned int integer=0;
byte count=0;
unsigned int gray;
byte chRead;

boolean valueCompleted = false;


void blinkLED (void) {
   digitalWrite(ledPin, HIGH);
   delay(200);
   digitalWrite(ledPin, LOW);
}


unsigned int binaryToGray(unsigned int num)
{
   return (num >> 1) ^ num;
}


void showBits (unsigned int number) {
   byte showBit;
   
   for (int i=0; i<16; i++) {
      showBit = ((number<<i) & 0x8000) ? 1 : 0;
     
      Serial.print (showBit);
     
      if (i == 3 || i == 7 || i == 11) {
         Serial.print (" ");
      } else if (i == 15) {
         Serial.print ("\n");
      }
   }
}

void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
}


void loop() {
 
  if (Serial.available()) {
    chRead = Serial.read();
   
    if (chRead == '\n') {
       valueCompleted = true;
       count = 0;
    }
    else if (chRead >= '0' && chRead <= '9' && count < 4) {
       integer = integer*10 + chRead - '0';
       count++;
    }
   
  }   
  if (valueCompleted == true) {
    valueCompleted = false;
   
    blinkLED();
   
    Serial.print ("Original \"decimal\" number: ");
    Serial.println (integer);
   
    Serial.print ("Binary: ");
    showBits (integer);
   
    gray = binaryToGray( integer );
   
    Serial.print ("Gray  : ");
    showBits (gray);   
    Serial.println (" ");
    integer = 0;
  }
}
