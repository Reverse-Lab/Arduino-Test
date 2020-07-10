// Noritake Itron GU-7k Test

#include <SoftwareSerial.h>
SoftwareSerial VFD(5,6); // RX, TX

 int encoder0PinA = 2;
 int encoder0PinB = 3;
 int encoder0Pos = 0;
 int encoder0PinALast = LOW;
 int n = LOW;


void setup()
{
   pinMode (encoder0PinA,INPUT);
   pinMode (encoder0PinB,INPUT);
  
  VFD.begin(38400); // set speed
  resetVFD();
  VFDclearscreen();
  VFD.write(0x1F);
  VFD.write(0x02); // vertical scroll mode (on)
}

void resetVFD()
// performs a software reset on the VFD controller
{
  VFD.write(0x1B); // ESC
  VFD.write(0x40); // software reset
}

void VFDnewline()
{
  VFD.write(0x0D); // carriage return
  VFD.write(0x0A); // line feed
}

void VFDclearscreen()
{
  VFD.write(0x0C); // clear display
  VFD.write(0x0B); // form feed
}

void VFDcursor(int mode)
{
  VFD.write(0x1F);
  VFD.write(0x43);
  VFD.write(mode);
}

void VFDbright(int level) // levels 1 to 8
{
  VFD.write(0x1F);
  VFD.write(0x58);
  VFD.write(level);
  delay(5); // brightness setting appears to be expensive?
}

void VFDchars()
// run through characters for selected font
{
  for (int i = 21 ; i <256; i++)
  {
   VFD.write(i);
   delay(10);
  }
  delay(1000);
}

void moveCursor(byte position)
// moves the cursor - lines are sequential
{
  VFD.write(0x1F);
  VFD.write(0x24); // move cursor
  VFD.write(position); // location
}

/* Set kerning
** k=0: Fixed Font Size with 1 dot space
** k=1: Fixed Font Size with 2 dot space
** k=2: Proportional Font Size with 1 dot space
** k=3: Proportional Font Size with 2 dot space
*/

void VFDkerning(int k)
{
  VFD.write(0x1F);
  VFD.write(0x28);
  VFD.write(0x67);
  VFD.write(0x03);
  VFD.write(k % 0x100);
}

/* font magnification
   X = 1,2,3,4
   Y = 1,2
   Default X=1, Y=1
*/

void VFDmagnify(int X, int Y)
{
  VFD.write(0x1f);
  VFD.write(0x28);
  VFD.write(0x67);
  VFD.write(0x40);
  VFD.write(X);
  VFD.write(Y);
} 

/*

font selection - font set and code type

Select int'l font set. Simple symbol only. See Font table. default n=0
n=0 : US, n=1 : France, n=2 : Germany, n=3 : England, n=4 : Denmark1,
n=5 : Sweden, n=6 : Italy, n=7 : Spain1, n=8 : Japan, n=9 : Norway,
n=10 : Denmark2, n=11 : Spain2, n=12 : Latin America, n=13 : Korea

Select font code. See Font table. Default n=0
n=0 : US-Euro std, n=1 : Katakana, n=2 : Multilingual, n=3 : Portuguese,
n=4 : Canadian-French, n=5 : Nordic, n=16 : WPC1252,
n=17 : Cyrillic #2, n=18 : Latin 2, n=19 : PC858

Obviously not all combinations are valid.

*/

void VFDfontsel(int set, int type)
{
  VFD.write(0x1b);
  VFD.write(0x52);
  VFD.write(set);
 
  VFD.write(0x1b);
  VFD.write(0x74);
  VFD.write(type);
}

void loop()
{
  VFDbright(8);
  VFDfontsel(13,1); // Korea

     n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
     } else {
       encoder0Pos++;
     }
       VFDclearscreen(); 
       VFD.print("Encoder Value : ");
       VFD.print(encoder0Pos);
   } 
   encoder0PinALast = n;
  /*
  delay(1000);
  
  VFDnewline();
  VFD.print("Hello, Nurse! - 2");
  delay(1000);
  VFDnewline();
  VFD.print("Hello, Nurse! - 3");
  delay(1000);
  VFDnewline();
  VFD.print("Hello, Nurse! - 4");
  delay(1000);
  VFDclearscreen();

  
  VFDbright(1);
  VFD.print("** Bright 1 of 8 **");   
  delay(1000);
  VFDclearscreen(); 
  VFDbright(2);
  VFD.print("** Bright 2 of 8 **");     
  delay(1000);
  VFDclearscreen();   
  VFDbright(3);
  VFD.print("** Bright 3 of 8 **");       
  delay(1000);
  VFDclearscreen();   
  VFDbright(4);
  VFD.print("** Bright 4 of 8 **");         
  delay(1000);
  VFDclearscreen();
  VFDbright(5);
  VFD.print("** Bright 5 of 8 **");
  delay(1000);
  VFDclearscreen();
  VFDbright(6);
  VFD.print("** Bright 6 of 8 **");
  delay(1000);
  VFDclearscreen();
  VFDbright(7);
  VFD.print("** Bright 7 of 8 **");
  delay(1000);
  VFDclearscreen();
  VFDbright(8);
  VFD.print("** Bright 8 of 8 **");
  delay(1000);
  VFDclearscreen();
 
  VFDchars();
  VFDclearscreen();
 
  VFDkerning(1);
  VFDchars();
  VFDclearscreen();

  VFDkerning(2);
  VFDchars();
  VFDclearscreen();
 
  VFDkerning(3);
  VFDchars();
  VFDclearscreen();
  VFDkerning(0);

  VFDmagnify(2,1);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(3,1);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(4,1);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(1,2);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(2,2);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(3,2);
  VFDchars();
  VFDclearscreen();
 
  VFDmagnify(4,2);
  VFDchars();
  VFDclearscreen();
  VFDmagnify(1,1);
 
  /*
 
  VFDfontsel(1,0); // France
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(2,0); // Germany
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(3,0); // England
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(4,0); // Denmark 1
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(5,0); // Sweden
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(6,0); // Italy
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(7,0); // Spain 1
  VFDchars();
  VFDclearscreen();
 
  
 
  VFDfontsel(8,1); // Japan
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(9,5); // Norway
  VFDchars();
  VFDclearscreen();
 
  
 
  VFDfontsel(10,0); // Denmark 2
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(11,0); // Spain 2
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(12,0); // Latin America
  VFDchars();
  VFDclearscreen();
 
  
 
  VFDfontsel(13,1); // Korea
  VFDchars();
  VFDclearscreen();
  delay (2000);
 
  VFDfontsel(0,17); // Cyrillic
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(0,0); */
   
}
