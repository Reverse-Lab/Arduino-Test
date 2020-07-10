//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(10, 2); // RX, TX

const uint8_t sync[]={0x00, 0x00, 0x00,0x00, 0x80, 0x55, 0xf1, 0xa5};

const uint8_t data[]={0x01, 0x10, 0x20,0x30, 0x40, 0x50, 0x60, 0xf2};

const uint8_t rsu = 0xff;

const unsigned int PWMTable[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13,
                                16, 19, 23, 26, 29, 32, 35, 39, 43, 47, 51, 55,
                                60, 66, 71, 77, 84, 91, 98, 106, 114, 123, 133, 143,
                                154, 166, 179, 192, 207, 222, 239, 257, 276, 296, 317, 341,
                                366, 392, 421, 451, 483, 518, 555, 595, 638, 684, 732, 784,
                                840, 900, 964, 1032, 1105, 1184, 1267, 1357, 1453, 1555, 1665, 1782,
                                1907, 2042, 2185, 2339, 2503, 2679, 2867, 3069, 3284, 3514, 3761, 4024, 4096};


void setup()
{
  Serial.begin(38400);
  
  for (int i=0;i<8-1;i++) {
   Serial.print(sync[i]);
  }
  for (int i=0;i<8-1;i++) {
   Serial.print(data[i]);
  }
}
void loop()
{

}
