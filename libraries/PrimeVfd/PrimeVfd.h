/*
 
 Created Febuary 2011
 
 This code is in the public domain.
 
*/

#ifndef PRIME_VFD_H
#define PRIME_VFD_H

#include <inttypes.h>
#include <avr/pgmspace.h> // Allows storing of data in program memory, used for charater bitmaps.
#include "Print.h"

// API is designed to be identical to the standard LiquidCrystal library.
// The only exception are:
// 
// * The initalization (constructor and begin method) are different since
//   the VFD has a different number of pins and the display dimensions are fixed.
// * We don't support the command() method to simplify implementation.
// 
// All other HD44780 behaviors and idiosyncrasies are mimicked.

class PrimeVfd : public Print {

public:
  
  enum PinConfig
  {
    // See the implementation of selectRegisterPinsForConfig() for how these values are used.
    // On the display, the pins are:
    //
    // 2/TP2   - +5v
    // 4/TP4   - data
    // 6/TP6   - clock
    // 8/TP8   - latch (simplified: low = blanks output, low->high = latch data, high display latched data)
    // 10/TP10 - gnd
    
    PIN_CONFIG_PORT_D_OPTION_1, // Port D/Arduino pins 2(data), 3(clock), 4(latch).
    PIN_CONFIG_PORT_D_OPTION_2, // Port D/Arduino pins 2(data), 3(clock), 5(latch).
    PIN_CONFIG_PORT_D_OPTION_3, // Port D/Arduino pins 2(data), 3(clock), 6(latch).
    PIN_CONFIG_PORT_D_OPTION_4, // Port D/Arduino pins 2(data), 3(clock), 7(latch).
    
    // When using multiple displays, Port B options can not be used at the same time.
    PIN_CONFIG_PORT_B_OPTION_1,
    // Option 1 uses SS (latch), SCK (clock) and MOSI (data) of Arduino Uno, etc.
    PIN_CONFIG_ARDUINO_SPI_PINS = PIN_CONFIG_PORT_B_OPTION_1, 
    PIN_CONFIG_PORT_B_OPTION_2,
    // Option 2 uses SS (latch), SCK (clock) and MOSI (data) of Arduino Mega, Teensy, etc.
    PIN_CONFIG_ARDUINO_MEGA_SPI_PINS = PIN_CONFIG_PORT_B_OPTION_2,
        
    NUM_PIN_CONFIGS
  };
  
  PrimeVfd(PinConfig pinConfig);
  ~PrimeVfd();
  
  void begin();

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t characterIndex, uint8_t pixels[] /* 8 bytes, 1 for each row */);
  void setCursor(uint8_t column, uint8_t row); 
  virtual size_t write(uint8_t character);
  // command() not implemented. See note above.
  //void command(uint8_t commandCode);
  
  // Do not call this directly , bad things will happen. 
  // It is a private method made public so the interrupt
  // handler can get at it.
  static void handleInterruptPrivate();
  
private:

  enum Flag
  {
    FLAG_BEGIN_HAS_BEEN_CALLED,
    FLAG_DISPLAY_ON,
    FLAG_CURSOR_ON,
    FLAG_BLINK_ON,
    CURSOR_MOVE_INCREMENT,
    DISPLAY_SHIFT_ON,
    
    NUM_FLAGS
  };

  static const uint8_t CHARACTER_PIXEL_WIDTH = 5;
  static const uint8_t CHARACTER_PIXEL_PRETEND_HEIGHT = 8;
  static const uint8_t CHARACTER_PIXEL_ACTUAL_HEIGHT = 7;
  static const uint8_t DISPLAY_NUM_COLUMNS = 20;
  static const uint8_t DISPLAY_NUM_ROWS = 2;
  static const uint8_t FIRST_CHARACTER_IN_ROM = 30;
  static const uint8_t LAST_CHARACTER_IN_ROM = 127;
  static const uint8_t NUM_ROM_CHARACTERS = LAST_CHARACTER_IN_ROM + 1 - FIRST_CHARACTER_IN_ROM;
  static const uint8_t NUM_CUSTOM_CHARACTERS = 8;
  static const uint16_t CURSOR_BLINK_INTERRUPT_ON_COUNT = 1000; // 1/2 second.
  static const uint16_t CURSOR_BLINK_INTERRUPT_LOOP_COUNT = 2000; // 1 second.
  static const uint8_t BLINK_CHARACTER = 30;
  static const uint8_t CURSOR_CHARACTER = 31;
  
  // For 500 us interval (100Hz) refresh and clock of 16MHz, prescalar of /16,
  // We get a remainder of 125, or start TCNT of (256-125).
  static const uint8_t TIMER_START_COUNT = (256-125);
  
  static const prog_uint8_t _characterSet[NUM_ROM_CHARACTERS][CHARACTER_PIXEL_WIDTH] PROGMEM;

  volatile uint8_t *_port;
  volatile uint8_t *_ddr;
  uint8_t _dataPinMask;
  uint8_t _clockPinMask;
  uint8_t _latchPinMask;

  uint8_t _flags;
  uint8_t _addressCounter;
  int8_t _displayOffset;
  
  // Linked list of all PrimeVfd instances, in theory to allow multiple displays
  // at the same time. Mutliple displays is untested.
  static bool _haveMultipleInstances;
  static bool _inInterruptHandler;
  static uint16_t _blinkInterruptCount;
  static PrimeVfd *_firstInstance;
  PrimeVfd *_nextInstance;
  uint8_t _currentUpdateColumn;

  // These large arrays are at the bottom of the class member variables 
  // in case large address offsets are expensive.
  uint8_t _displayData[80]; // Mimics layout of LCD module RAM.
  uint8_t _customCharacterSet[NUM_CUSTOM_CHARACTERS][CHARACTER_PIXEL_WIDTH];

  inline void setFlag(Flag flag) { _flags |= (1<<flag); }
  inline void clearFlag(Flag flag) { _flags &= ~(1<<flag); }
  inline bool getFlag(Flag flag) { return (_flags & (1<<flag)) ? true : false; }

  void selectRegisterPinsForConfig(PinConfig pinConfig);
  void setupPins();
  void restorePins();

  void blankDisplay();
  
  void displayUpdate();
  void clockCharacter(uint8_t character);
  void clockCustomCharacter(uint8_t character);  
  void clockColumnSelect(uint8_t column);
  uint8_t getCursorCharacter(uint8_t originalCharacter);
};

#endif // PRIME_VFD_H

