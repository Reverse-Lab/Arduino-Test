#define pha_a 2  // pin 2 on UNO is connected to phase a encoder
#define pha_b 3   // pin 3 on UNO is connected to phase b encoder

static boolean rotating=false;
volatile long motor_position,encoder;
boolean A_set = false;
boolean B_set = false;

void setup() {
      
      pinMode(pha_a,INPUT);
      pinMode(pha_b,INPUT);

      digitalWrite(pha_a, HIGH);
      digitalWrite(pha_b, HIGH);
      // encoder pin on interrupt 0 (pin 2)
      attachInterrupt (0, encoderPinChangeA, CHANGE);
      // encoder pin on interrupt 1 (pin 3)
      attachInterrupt(1, encoderPinChangeB, CHANGE);
      encoder = 0;
      Serial.begin(115200);
}
  void encoderPinChangeA()                
{
      if (digitalRead(pha_a)== digitalRead (pha_b))
      {
        encoder--;
      }
        else
      {
        encoder++;
      }
}  

void encoderPinChangeB()
{
      if (digitalRead(pha_a)!= digitalRead (pha_b))
      {
        encoder--;
      }
      else
      {
        encoder++;
      }
}

void loop ()   {
  Serial.print("ENCODER =");
  Serial.println(encoder);
}
