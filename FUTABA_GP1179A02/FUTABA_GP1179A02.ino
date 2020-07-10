// Noritake Itron GU-7k Test
//https://forum.arduino.cc/index.php?topic=198201.0

#include <SoftwareSerial.h>
SoftwareSerial VFD(5,6); // RX, TX

void setup()
{
  VFD.begin(19200); // set speed
  resetVFD();
  VFDclearscreen();
  
  VFD.write(0x1B);
  VFD.write(0x6B);
  VFD.write(0x55);
  VFD.write(0x11);
  VFD.write(0x01);
  VFD.write(0x01);
  VFD.write(0x33);
}

void resetVFD()
// performs a software reset on the VFD controller
{
  VFD.write(0x1B);
  VFD.write(0x4A); 
  VFD.write(0x52);
  VFD.write(0x43);

}

void VFDclearscreen()
{
  VFD.write(0x1B);
  VFD.write(0x4A);
  VFD.write(0x43);
  VFD.write(0x44);

}

void loop() {
  ;
}
