
class Option
{
  int optionPin;
  long optionOnTime;

  int optionState;
  unsigned long optionpreviousMillis;


 public:
 Option(int pin, long on)
 {
  optionPin = pin;
  pinMode(optionPin, OUTPUT);

  optionOnTime = on;

  optionState = LOW;
  optionpreviousMillis = 0;
 }

 void ON()
 {
  unsigned long optionCurrentMillis = millis();
  if((optionState == HIGH) && (optionCurrentMillis - optionpreviousMillis >= optionOnTime))
  {
    optionState = LOW;
    optionpreviousMillis = optionCurrentMillis;
    digitalWrite(optionPin, optionState);
  } else if((optionState == LOW) && (optionCurrentMillis - optionpreviousMillis >= optionOnTime))
  {
    optionState = HIGH;
    optionpreviousMillis = optionCurrentMillis;
    digitalWrite(optionPin, optionState);    
  }
 }
};

Option AC(15, 1000);

void setup() {
 
}

// the loop function runs over and over again forever
void loop() {
 AC.ON();
}
