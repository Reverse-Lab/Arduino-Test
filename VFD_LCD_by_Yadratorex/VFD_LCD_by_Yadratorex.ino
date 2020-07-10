/*
 
  The circuit:
  
 * LCD VSS pin to ground 
 * LCD VCC pin to 5V 
 * LCD RS pin to digital pin 12 
 * LCD R/W pin to ground 
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5 
 * LCD D5 pin to digital pin 4 
 * LCD D6 pin to digital pin 3 
 * LCD D7 pin to digital pin 2 
 
CODE REALIZED ALL BY YADRATOREX

18-01-2017
 

 */


#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    //THE PINS WHERE THE DISPLAY IS CONNECTED

//There I created my own characters, the arrows and parts for the final smiley

byte arrow_1[8] = {
  0b01000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b00000
};

byte arrow_2[8] = {
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b01000,
  0b00100,
  0b00010,
  0b00000
};

byte smiley0[8] = {
    0B00100,
    0B01010,
    0B10001,
    0B10001,
    0B10101,
    0B10001,
    0B01010,
    0B00100
};
byte smiley1[8] = {
    0B00000,
    0B00000,
    0B10000,
    0B11000,
    0B01100,
    0B00110,
    0B00011,
    0B00000
};

byte smiley2[8] = {
    0B00000,
    0B00000,
    0B00001,
    0B00011,
    0B00110,
    0B01100,
    0B11000,
    0B00000
};

byte smiley3[8] = {
    0B00000,
    0B01010,
    0B00100,
    0B00000,
    0B00000,
    0B00000,
    0B11111,
    0B00000
};




void setup() {
  
  lcd.begin(20, 2);                 //MY LCD HAS 20 CHARACTERS AND 2 LINES
  lcd.print("Hello my friends");    //FIRST MESSAGE
  delay(1500);                      //WHEN YOU SEE DELAY, ITS THE WAIT TIME IN MILLISECONDS FOR THE NEXT ACTION

  lcd.createChar(0, arrow_1);      //There I set the byte number with the characters I made
  lcd.createChar(1, arrow_2);
  lcd.createChar(4, smiley0);
  lcd.createChar(5, smiley1);
  lcd.createChar(6, smiley2);
  lcd.createChar(7, smiley3);
  
  
}

void loop() {
  
  lcd.setCursor(0, 1);  //I START WRITING IN THE SECOND LINE
  lcd.blink();
  delay(4000);          //BLINKING TIME
  lcd.noBlink();
  lcd.cursor();
  lcd.write("W");       //WRITE ONE CHARACTER
  delay(200);           //WAIT
  lcd.write("e");       //WRITE ANOTHER CHARACTER
  delay(200);           //WAIT...
  lcd.write("l");
  delay(200);
  lcd.write("c");
  delay(200);
  lcd.write("o");
  delay(200);
  lcd.write("m");
  delay(300);
  lcd.write("e");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write("t");
  delay(200);
  lcd.write("o");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write("a");
  delay(200);
  lcd.write("n");
  delay(200);
  lcd.write("o");
  delay(200);
  lcd.write("t");
  delay(200);
  lcd.write("h");
  delay(200);
  lcd.write("e");
  delay(200);
  lcd.write("r");
  delay(200);
  
   for (int positionCounter = 0; positionCounter < 19; positionCounter++) {
    
    lcd.scrollDisplayLeft();    //SCROLL THE DISPLAY TO THE LEFT
    delay(150);                 //TIME IN MILLISECONDS FOR SCROLL ONE CHARACTER TO THE LEFT
    
  }
  lcd.write(" ");
  delay(200);
  lcd.write("v");
  delay(200);
  lcd.write("i");
  delay(200);
  lcd.write("d");
  delay(200);
  lcd.write("e");
  delay(200);
  lcd.write("o");
  delay(200);
  lcd.write(".");
  lcd.noCursor();
  lcd.noBlink();
  delay(1500);
  lcd.clear();
  lcd.setCursor(6, 0);          //I START WRITING AT THE 6TH CHARACTER AT THE FIRST LINE
  lcd.write("This is");
  delay(500);
  lcd.setCursor(3, 1);
  lcd.write("my 2nd project");
  delay(2000);
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.write("You can download");
  delay(500);
  lcd.setCursor(5, 1);
  lcd.write("this code");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(6, 0);
  lcd.write("FOR FREE");
  delay(300);
  lcd.noDisplay();      //THAT ACTIONS "display & noDisplay" WILL DO BLINK THE TEXT
  delay(300);
  lcd.display();
  delay(300);
  lcd.noDisplay();
  delay(300);
  lcd.display();
  delay(300);
  lcd.noDisplay();
  delay(300);
  lcd.display();
  delay(300);
  lcd.noDisplay();
  delay(300);
  lcd.display();
  delay(500);
  lcd.clear();
  
  lcd.setCursor(0, 1);
  lcd.write("bellow at the description.");
  delay(700);
  
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
   
    lcd.scrollDisplayLeft();
    delay(150);
    
  }
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.write(byte(0));         //THERE I WRITE ONE OF MY CREATED CHARACTERS, ONE ARROW IN THIS CASE
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  delay(500);
  lcd.setCursor(6, 0);
  lcd.cursor();
  lcd.write("S");
  delay(200);
  lcd.write("U");
  delay(200);
  lcd.write("B");
  delay(200);
  lcd.write("S");
  delay(200);
  lcd.write("C");
  delay(200);
  lcd.write("R");
  delay(200);
  lcd.write("I");
  delay(200);
  lcd.write("B");
  delay(200);
  lcd.write("E");
  lcd.noCursor();
  delay(2000);

  lcd.clear();

  lcd.setCursor(5, 1);
 lcd.write("YADRAT0REX");
 delay(2000);
 lcd.setCursor(5, 1);
 lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(200);
  lcd.write(" ");
  delay(1000);
  lcd.clear();
 
  lcd.setCursor(7, 0);
  lcd.cursor();
  lcd.write("L");
  delay(200);
  lcd.write("o");
  delay(200);
  lcd.write("a");
  delay(200);
  lcd.write("d");
  delay(200);
  lcd.write("i");
  delay(200);
  lcd.write("n");
  delay(200);
  lcd.write("g");
  lcd.noCursor();
  delay(300);
 
  lcd.setCursor(8, 1);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(800);
  lcd.clear();
  
  lcd.setCursor(7, 0);
  lcd.write("Loading");
  lcd.setCursor(8, 1);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(600);
  lcd.write(".");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(5,0);
  lcd.write("STAY");
  delay(1500);
  lcd.setCursor(5,1);
  lcd.write("HAPPY");
  delay(2000);
  lcd.setCursor(14,0);
  lcd.write(byte(4));
  lcd.setCursor(16,0);
  lcd.write(byte(4));
  lcd.setCursor(14,1);
  lcd.write(byte(5));
  lcd.setCursor(15,1);
  lcd.write(byte(7));
  lcd.setCursor(16,1);
  lcd.write(byte(6));

  delay(5000);
  

 while(1) { }
}
