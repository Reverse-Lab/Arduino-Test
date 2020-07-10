// 수정일자 2020-02-29
/****Arduino MODBUS RTU***
 * 
 * dejan.rakijasic@gmail.com
 * 
 * MODBUS library written by Jason Vreeland [CodeRage]
 * Released 3/7/2010 under GNU license
 * https://code.google.com/archive/p/arduino-modbus-slave/downloads
 */
#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
modbusDevice regBank;
modbusSlave slave;

#define GREEN_LED 12
#define ANALOG_LED_1 10
#define ANALOG_LED_2 11
#define BUTTON 4
#define POTENTIOMETER A0

void setup()
{   
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ANALOG_LED_1, OUTPUT);
  pinMode(ANALOG_LED_2, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //Assign the Modbus Slave ID.
  regBank.setId(1);

  //Add Digital Output registers
  regBank.add(1);

  //Add Digital Input registers
  regBank.add(10001);
  
  //Add Analog Output registers  
  regBank.add(40001);   
  regBank.add(40002);   
  
  //Analog input registers
  regBank.add(30001);
  
  slave._device = &regBank; 
  // Initialize the baudrate serial port 
  // Set in SCADA: Baud rate=9600,Data bits=8,Stop bits=1,Parity=None,Flow control=None
  slave.setBaud(9600);  
}

void loop()
{
  //Digital output (세트비트 출력)
  int DO3 = regBank.get(1); // 스위치 및 버튼, 센서 출력 처리 방법 (03-01)
  if (DO3 <= 0 && digitalRead(GREEN_LED) == HIGH)digitalWrite(GREEN_LED,LOW);
  if (DO3 >= 1 && digitalRead(GREEN_LED) == LOW)digitalWrite(GREEN_LED,HIGH);
  
  //Digital Input
  byte DI2 = digitalRead(BUTTON);
  if (DI2 >= 1)regBank.set(10001,0);
  if (DI2 <= 0)regBank.set(10001,1);
    
  //Analog input
  regBank.set(30001, (word)analogRead(POTENTIOMETER));
  // word temp = map(analogRead(POTENTIOMETER),0,1023,0,255);
  // analogWrite(ANALOG_LED_2, temp);
  
  //Analog output 
  word AO10 = regBank.get(40001);
  analogWrite(ANALOG_LED_1, AO10);
  word AO11 = regBank.get(40002);
  analogWrite(ANALOG_LED_2, AO11);
  
  slave.run();
  delay(100);
}
