#define encoder_a 2 //keep this on and interrupt pin
#define encoder_b 3 //keep this on and interrupt pin 
#define motor_step 9 
#define motor_direction 8 
#define senzor_front 6 
#define senzor_rear 7 
#define load_button 4 
#define eject_button 5 
int home_position = 0; 
int load_position = 0; 
int pe_load = 0; 

void setup() {
  pinMode(encoder_a, INPUT);
  pinMode(encoder_b, INPUT); // disable pullup as we aren't using an open collector encoder
  digitalWrite(encoder_a, LOW); 
  digitalWrite(encoder_b, LOW); //set up the various outputs
  pinMode(motor_step, OUTPUT);
  pinMode(motor_direction, OUTPUT); // then the senzors inputs
  pinMode(senzor_front, INPUT_PULLUP);
  pinMode(senzor_rear, INPUT_PULLUP);
  pinMode(load_button, INPUT_PULLUP);
  pinMode(eject_button, INPUT_PULLUP); 
  } 
  
  void loop() 
  { // Control platen position of boot up and move to front 
    if (home_position == 0) {
      digitalWrite(motor_direction, HIGH); 
      TurnStepper();
      if (digitalRead(senzor_front) == LOW) {
        home_position = 1;
        }
     } // eject platen 
     
     if (digitalRead(eject_button) == LOW) {
      home_position = 0;
      } // load platen 
     
     if (digitalRead(load_button) == LOW) {
      load_position = 1; 
     } 
     
     if (load_position == 1) {
      digitalWrite(motor_direction, LOW);
      TurnStepper();
      if (digitalRead(senzor_rear) == LOW) {
        load_position = 0;
        pe_load = 1; 
        }
      }
    }
    
void TurnStepper() 
    {
      digitalWrite(motor_step, HIGH);
      delayMicroseconds(40); //speed delay, the lower it is t 
      digitalWrite(motor_step, LOW); 
      delayMicroseconds(40); //speed delay, the lower it is the faster 
    }
