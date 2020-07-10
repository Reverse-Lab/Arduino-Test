/*
 
 Created Febuary 2011
 
 This code is in the public domain.
 
*/

#include "PrimeVfd.h"
#include "WProgram.h"

// First instance for interrupt handler to use.
/* static */ PrimeVfd *PrimeVfd::_firstInstance = NULL;
/* static */ bool PrimeVfd::_haveMultipleInstances = false;
/* static */ bool PrimeVfd::_inInterruptHandler = false;
/* static */ uint16_t PrimeVfd::_blinkInterruptCount = 0;

PrimeVfd::PrimeVfd(PinConfig pinConfig)
{
  _addressCounter = 0;
  _displayOffset = 0;
  for (uint8_t i=0; i<80; i++)
  {
    _displayData[i] = ' ';
  }
  for (uint8_t i=0; i<NUM_CUSTOM_CHARACTERS; i++)
  {
    for (uint8_t j=0; j<CHARACTER_PIXEL_WIDTH; j++)
    {
      _customCharacterSet[i][j] = 0;
    }
  }

  _flags = 0;
  setFlag(FLAG_DISPLAY_ON);
  setFlag(CURSOR_MOVE_INCREMENT);

  _currentUpdateColumn = 0;
  
  // Setup pins.
  selectRegisterPinsForConfig(pinConfig);
  setupPins();
 
  // Blank the display, otherwise we get get a  
  // flash on startup.
  blankDisplay();
  
  _haveMultipleInstances = _firstInstance != NULL;
	
  // Install into the instance linked list.
  uint8_t oldSREG = SREG;
  cli();
  _nextInstance = _firstInstance;
  _firstInstance = this;
  SREG = oldSREG;
}

PrimeVfd::~PrimeVfd()
{
  // TODO: Destructor has not been tested.
  
  // Unhook from linked list.
  uint8_t oldSREG = SREG;
  cli();

  if (_firstInstance == this)
  {
    _firstInstance = _nextInstance;
  }
  else
  {
    for (PrimeVfd *current = _firstInstance; current!=NULL; current = current->_nextInstance)
    {
      if (current->_nextInstance == this)
      {
        // Found ourselves so unlink.
        current->_nextInstance = _nextInstance;
        break;
      }
    }
  }

  if (_firstInstance == NULL)
  {
    // This was the last instance so disable timer interrupt.
    TIMSK2 = 0;
  }

  // Clear display.
  blankDisplay();

  // Restore pin states.
  restorePins();
  
  // For all other instances, setup pins again 
  // in case the restore we just did collided with
  // something.
  for (PrimeVfd *current = _firstInstance; current!=NULL; current = current->_nextInstance)
  {
    current->setupPins();
  }
  
  _haveMultipleInstances = (_firstInstance!=NULL) && (_firstInstance->_nextInstance != NULL);
	
  SREG = oldSREG;
}

void PrimeVfd::selectRegisterPinsForConfig(PinConfig pinConfig)
{
  switch (pinConfig)
  {
    case PIN_CONFIG_PORT_D_OPTION_1: // Port D/Arduino pins 2, 3, 4.
      _ddr = &DDRD;
      _port = &PORTD;
      _dataPinMask = (1<<2);
      _clockPinMask = (1<<3);
      _latchPinMask = (1<<4);
      break;
      
    case PIN_CONFIG_PORT_D_OPTION_2: // Port D/Arduino pins 2, 3, 5.
      _ddr = &DDRD;
      _port = &PORTD;
      _dataPinMask = (1<<2);
      _clockPinMask = (1<<3);
      _latchPinMask = (1<<5);
      break;
      
    case PIN_CONFIG_PORT_D_OPTION_3: // Port D/Arduino pins 2, 3, 6.
      _ddr = &DDRD;
      _port = &PORTD;
      _dataPinMask = (1<<2);
      _clockPinMask = (1<<3);
      _latchPinMask = (1<<6);
      break;
      
    case PIN_CONFIG_PORT_D_OPTION_4: // Port D/Arduino pins 2, 3, 7.
      _ddr = &DDRD;
      _port = &PORTD;
      _dataPinMask = (1<<2);
      _clockPinMask = (1<<3);
      _latchPinMask = (1<<7);
      break;

    case PIN_CONFIG_PORT_B_OPTION_1:
      _ddr = &DDRB;
      _port = &PORTB;
      _dataPinMask = (1<<3); // MOSI for regular Arduino.
      _clockPinMask = (1<<5); // SCK for regular Arduino.
      _latchPinMask = (1<<2); // SS for regular Arduino.
      break;
        
    case PIN_CONFIG_PORT_B_OPTION_2:
      _ddr = &DDRB;
      _port = &PORTB;
      _dataPinMask = (1<<2); // MOSI for Arduino Mega, Teensy, etc.
      _clockPinMask = (1<<1); // SCK for Arduino Mega, Teensy, etc.
      _latchPinMask = (1<<0); // SS for Arduino Mega, Teensy, etc.
      break;
      
    default:
      // We had best not continue:
      while (1) {}
      break;   
  } 
}

void PrimeVfd::setupPins()
{
  uint8_t oldSREG = SREG;
  cli();

  uint8_t combinedMask = _dataPinMask | _clockPinMask | _latchPinMask;
  *_port &= ~combinedMask;
  *_ddr |= combinedMask;

  SREG = oldSREG;
}

void PrimeVfd::restorePins()
{
  uint8_t oldSREG = SREG;
  cli();

  uint8_t combinedMask = ~(_dataPinMask | _clockPinMask | _latchPinMask);
  *_ddr &= combinedMask;
  *_port &= combinedMask;

  SREG = oldSREG;
}

void PrimeVfd::begin()
{
  // Start the timer interrupts. Multiple instances may run this same code but they all
  // will set the same values so all is good.
  uint8_t oldSREG = SREG;
  cli();

  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 0<<CS21 | 0<<CS20; // Prescaler = /64.

  //Timer2 Overflow Interrupt Enable
  TIMSK2 = 1<<TOIE2;

  // Kick off the timer for its first cycle. If the timer
  // was already running, it will just restart.
  TCNT2 = TIMER_START_COUNT;

  setFlag(FLAG_BEGIN_HAS_BEEN_CALLED);

  SREG = oldSREG;
}

void PrimeVfd::clear()
{
  for (uint8_t i=0; i<80; i++)
  {
    _displayData[i] = ' ';
  }
  
  _addressCounter = 0;
  _displayOffset = 0;
  setFlag(CURSOR_MOVE_INCREMENT);
}

void PrimeVfd::home()
{
  _addressCounter = 0;
  _displayOffset = 0;
}

void PrimeVfd::noDisplay()
{
  if (!getFlag(FLAG_BEGIN_HAS_BEEN_CALLED))
  {
    // Bail if begin() hasn't been called yet since
    // begin() sets up the timer. 
    return;
  }

  if (getFlag(FLAG_DISPLAY_ON))
  {
    clearFlag(FLAG_DISPLAY_ON);

    // Turn off timer interrupt.
    TIMSK2 = 0;

    // Blank the display.
    blankDisplay();
  }
}

void PrimeVfd::display()
{
  if (!getFlag(FLAG_BEGIN_HAS_BEEN_CALLED))
  {
    // Bail if begin() hasn't been called yet since
    // begin() sets up the timer. 
    return;
  }
  
  if (!getFlag(FLAG_DISPLAY_ON))
  {
    setFlag(FLAG_DISPLAY_ON);
  
    // Turn on timer interrupt.
    TIMSK2 = 1<<TOIE2;
  }
}  

void PrimeVfd::noBlink()
{
  clearFlag(FLAG_BLINK_ON);
}

void PrimeVfd::blink()
{
  setFlag(FLAG_BLINK_ON);
}

void PrimeVfd::noCursor()
{
  clearFlag(FLAG_CURSOR_ON);
}

void PrimeVfd::cursor()
{
  setFlag(FLAG_CURSOR_ON);
}

void PrimeVfd::scrollDisplayLeft()
{
  _displayOffset++;
  if (_displayOffset >= 40)
  {
    _displayOffset = 0;
  }
}

void PrimeVfd::scrollDisplayRight()
{
  _displayOffset--;
  if (_displayOffset < 0)
  {
    _displayOffset = 39;
  }
}

void PrimeVfd::leftToRight()
{
  setFlag(CURSOR_MOVE_INCREMENT);
}

void PrimeVfd::rightToLeft()
{
  clearFlag(CURSOR_MOVE_INCREMENT);
}

void PrimeVfd::autoscroll()
{
  setFlag(DISPLAY_SHIFT_ON);
}

void PrimeVfd::noAutoscroll()
{
  clearFlag(DISPLAY_SHIFT_ON);
}

void PrimeVfd::createChar(uint8_t characterIndex, uint8_t pixels[] /* 8 bytes, 1 for each row */)
{
  characterIndex &= NUM_CUSTOM_CHARACTERS-1; // Limit index range.
  uint8_t *dest = _customCharacterSet[characterIndex];
  
  // We need to rotate the pixels.
  uint8_t testBit = 0b10000;
  for (uint8_t i=0; i<CHARACTER_PIXEL_WIDTH; i++)
  {
    uint8_t currentByte = 0;
    
    for (uint8_t i=0; i<CHARACTER_PIXEL_PRETEND_HEIGHT; i++)
    {
      currentByte <<= 1;

      if (pixels[i] & testBit)
      {
        currentByte |= 1;
      }
    }
    
    dest[i] = currentByte;
    testBit >>= 1;
  }
}

void PrimeVfd::setCursor(uint8_t column, uint8_t row)
{
  _addressCounter = ( ((row!=0) ? 64 : 0) + column ) & 0x7F;
}
  
/* virtual */ size_t PrimeVfd::write(uint8_t character)
{
  if (!getFlag(FLAG_BEGIN_HAS_BEEN_CALLED))
  {
    // Refuse to draw anything until begin has been 
    // called to ensure the sketch calls it. 
    return 0;
  }
  
  if (_addressCounter < 40)
  {
    // First row.
    _displayData[_addressCounter] = character;
  }
  else if (_addressCounter >=64 && _addressCounter < (64+40))
  {
    // Second row.
    _displayData[_addressCounter - 64 + 40] = character;
  }

  // Update cursor position.
  if (getFlag(CURSOR_MOVE_INCREMENT))
  {
    _addressCounter++;
    if (_addressCounter==40)
    {
      _addressCounter = 64;
    }
    else if (_addressCounter==(64+40))
    {
      _addressCounter = 0;
    }
    if (getFlag(DISPLAY_SHIFT_ON))
    {
      scrollDisplayLeft();
    }
  }
  else
  {
	if (_addressCounter==00)
	{
	  _addressCounter = 64+40;
	}
	else if (_addressCounter==64)
	{
	  _addressCounter = 40;
	}
	  
	_addressCounter--;
	  
	if (getFlag(DISPLAY_SHIFT_ON))
    {
      scrollDisplayRight();
    }
  }
  
  _addressCounter &= 0x7F;

  return 1;
}

void PrimeVfd::blankDisplay()
{
  volatile uint8_t *port = _port;
  uint8_t dataPinSetMask = _dataPinMask;
  uint8_t clockPinSetMask = _clockPinMask;
  uint8_t dataPinClearMask = ~dataPinSetMask;
  uint8_t clockPinClearMask = ~clockPinSetMask;
  
  uint8_t portClock0Value = *port & dataPinClearMask & clockPinClearMask;
  uint8_t portClock1Value = (*port & dataPinClearMask) | clockPinSetMask;
    
  // Send 92 (35 + 35 + 2 + 20) zero bits. This will blank all grids and anodes.
  uint8_t count = 92;
  do 
  {
    *port = portClock1Value;

    count--; // Between writes to add a timing delay.

    *port = portClock0Value;

  } while (count);

  // Latch in the result.
  *_port &= ~_latchPinMask;
  delayMicroseconds(1);
  *_port |= _latchPinMask;
}


