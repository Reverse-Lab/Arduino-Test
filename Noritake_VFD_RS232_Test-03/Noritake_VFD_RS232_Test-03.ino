// Noritake Itron GU-7k Test
//https://forum.arduino.cc/index.php?topic=198201.0

#include <SoftwareSerial.h>
SoftwareSerial VFD(5,6); // RX, TX

void setup()
{
  VFD.begin(38400); // set speed
  resetVFD();
  VFDclearscreen();
  VFD.write(0x1F);
//  VFD.write(0x01); // Overwrite mode, default
  VFD.write(0x02); // vertical scroll mode
//  VFD.write(0x03); // Horizontal scroll mode 
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
//  VFD.write(0x0B); // form feed
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
//  delay(5); // brightness setting appears to be expensive? -- needed at 38400bps
}

void VFDchars()
// run through characters for selected font
{
  for (int i = 21 ; i <256; i++)
  {
   VFD.write(i);
   delay(5);
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

/*
graphics. I have no idea.
*/

/* 67 bits by 4 bytes image "ArduinoLogo" */
byte image_ArduinoLogo[] = {
    0xff,0xf0,0x0f,0xff,0xff,0x80,0x01,0xff,0xfe,0x00,0x00,0x7f,0xfc,0x00,0x00,0x3f,
    0xf8,0x00,0x00,0x1f,0xf0,0x00,0x00,0x0f,0xe0,0x0f,0xf0,0x07,0xc0,0x3f,0xfc,0x07,
    0xc0,0x7f,0xfe,0x03,0x80,0xff,0xff,0x01,0x81,0xff,0xff,0x01,0x81,0xff,0xff,0x81,
    0x01,0xfc,0x3f,0x81,0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,
    0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,
    0x03,0xfc,0x3f,0x80,0x81,0xfc,0x3f,0x81,0x81,0xff,0xff,0x81,0x80,0xff,0xff,0x01,
    0xc0,0x7f,0xfe,0x03,0xc0,0x7f,0xfe,0x03,0xe0,0x3f,0xf8,0x07,0xf0,0x0f,0xf0,0x0f,
    0xf0,0x07,0xe0,0x1f,0xf8,0x03,0xc0,0x1f,0xfc,0x00,0x00,0x3f,0xfe,0x00,0x00,0x7f,
    0xff,0x80,0x01,0xff,0xff,0xc0,0x03,0xff,0xff,0x80,0x01,0xff,0xfe,0x00,0x00,0x7f,
    0xfe,0x00,0x00,0x7f,0xf8,0x03,0x80,0x1f,0xf0,0x07,0xe0,0x1f,0xf0,0x0f,0xf0,0x0f,
    0xe0,0x3f,0xf8,0x07,0xc0,0x7f,0xfc,0x03,0xc0,0x7f,0xfe,0x03,0x80,0xff,0xff,0x01,
    0x81,0xff,0xff,0x81,0x81,0xfc,0x3f,0x81,0x03,0xfc,0x3f,0x80,0x03,0xfc,0x3f,0xc0,
    0x03,0xe0,0x07,0xc0,0x03,0xe0,0x07,0xc0,0x03,0xe0,0x07,0xc0,0x03,0xe0,0x07,0xc0,
    0x03,0xfc,0x3f,0xc0,0x03,0xfc,0x3f,0xc0,0x01,0xfc,0x3f,0x81,0x81,0xff,0xff,0x81,
    0x81,0xff,0xff,0x81,0x80,0xff,0xff,0x01,0xc0,0x7f,0xfe,0x03,0xc0,0x1f,0xf8,0x07,
    0xe0,0x0f,0xf0,0x07,0xf0,0x00,0x00,0x0f,0xf8,0x00,0x00,0x1f,0xfc,0x00,0x00,0x3f,
    0xfe,0x00,0x00,0x7f,0xff,0x80,0x01,0xff,0xff,0xf8,0x1f,0xff
};

/* 16 bits by 2 bytes image "globe" */
byte image_globe[] = {
    0xf8,0x1f,0xe7,0xa7,0xcb,0xd3,0x9d,0xe9,0xa0,0x0d,0x5d,0xe2,0x3d,0xec,0x3d,0xec,
    0x40,0x00,0x3d,0xec,0x5d,0xea,0xa5,0xe5,0x98,0x09,0xcb,0xd3,0xe7,0xa7,0xf8,0x1f
};

/* immediately draw graphics at cursor. X is width in
bits, Y is height in bytes of 8 bits */

void VFDpaint(byte bmp[], int X, int Y)
{
  VFD.write(0x1F);
  VFD.write(0x28);
  VFD.write(0x66);
  VFD.write(0x11);
  VFD.write(X % 256);
  VFD.write(X / 256);
  VFD.write(Y % 256);
  VFD.write(Y / 256);
  VFD.write(0x01);
 
  for(int i = 0; i < X*Y; i++)
{
  VFD.write(bmp[i]);
}
}

void VFDreverse(int val) // 0 = no 1 = yes
{
  VFD.write(0x1f);
  VFD.write(0x72);
  VFD.write(val);
}


void loop()
{
  VFDcursor(1);
  delay(100);
  VFDcursor(0);
  VFDclearscreen();
  VFDpaint(image_ArduinoLogo,67,4);
//  VFDpaint(image_globe,16,2);
  delay(20000);
  VFDclearscreen();
 
  VFD.print("Hello, Nurse! - 1");
  delay(1000);
  VFDnewline();

  VFDreverse(1);
  VFD.print("Hello, Nurse! - 2");
  delay(1000);
  VFDnewline();

  VFDreverse(0);
  VFD.print("Hello, Nurse! - 3");
  delay(1000);
  VFDnewline();

  VFDreverse(1);
  VFD.print("Hello, Nurse! - 4");
  delay(1000);
  VFDclearscreen();

  VFDreverse(0);
 
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
 
  */
 
  VFDfontsel(8,1); // Japan
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(9,5); // Norway
  VFDchars();
  VFDclearscreen();
 
  /*
 
  VFDfontsel(10,0); // Denmark 2
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(11,0); // Spain 2
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(12,0); // Latin America
  VFDchars();
  VFDclearscreen();
 
  */
 
  VFDfontsel(13,1); // Korea
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(0,17); // Cyrillic
  VFDchars();
  VFDclearscreen();
 
  VFDfontsel(0,0);

   
}
