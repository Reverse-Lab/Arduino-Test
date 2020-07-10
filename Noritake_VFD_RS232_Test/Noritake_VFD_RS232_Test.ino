// Noritake Itron GU-7k Test
// https://forum.arduino.cc/index.php?topic=198201.0
#include <SoftwareSerial.h>
SoftwareSerial VFD(10,11); // RX, TX

void setup()
{
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
  for (int i = 21 ; i <256; i++) // i = 21 ~ 256
  {
   VFD.write(i);
   delay(10);
  }
}

void moveCursor(byte position)
// moves the cursor - lines are sequential
{
  VFD.write(0x1F);
  VFD.write(0x24); // move cursor
  VFD.write(position); // location
}

void loop()
{
  delay(1000);
  VFD.print("Hello, Nurse! - 1");
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
 
}
