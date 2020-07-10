/*
 
Created Febuary 2011

This code is in the public domain.

Note that the display requires a maximum clock rate
of 4MHz which means a minimum of 2 AVR instructions
between writes to the clock pin. Delays have
been added in places after examining the disassembly 
of the compiler generated code. It is possible that
compiler changes could change things enough to 
throw the timing out.
*/

#include "PrimeVfd.h"
#include "WProgram.h"

ISR(TIMER2_OVF_vect) {
  
  PrimeVfd::handleInterruptPrivate();
}

/* static */ void PrimeVfd::handleInterruptPrivate() {
  
  // Restart timer.
  TCNT2 = TIMER_START_COUNT;  
  
  if (!_inInterruptHandler)
  {
    _inInterruptHandler = true;
        
    // Run update for each instance.
    for (PrimeVfd *current = _firstInstance; current!=NULL; current = current->_nextInstance)
    {
      if (current->getFlag(FLAG_BEGIN_HAS_BEEN_CALLED))
      {
        current->displayUpdate();
      }
    }

    // Update blink count. All instances use the same count.
    uint16_t newBlinkCount = _blinkInterruptCount + 1;
    if (newBlinkCount > CURSOR_BLINK_INTERRUPT_LOOP_COUNT)
    {
      newBlinkCount = 0;  
    }
    _blinkInterruptCount = newBlinkCount;
    
    _inInterruptHandler = false;
  }
}

void PrimeVfd::clockCharacter(uint8_t character)
{
  if (character < FIRST_CHARACTER_IN_ROM || character > LAST_CHARACTER_IN_ROM)
  {
    character = ' ';
  }
    
  const prog_uint8_t *pixels = _characterSet[character - FIRST_CHARACTER_IN_ROM];

  volatile uint8_t *port = _port;
  uint8_t dataPinSetMask = _dataPinMask;
  uint8_t clockPinSetMask = _clockPinMask;
  uint8_t dataPinClearMask = ~dataPinSetMask;
  uint8_t clockPinClearMask = ~clockPinSetMask;

  uint8_t portValueData0 = *port & dataPinClearMask;
  uint8_t portValueData0Clock0 = portValueData0 & clockPinClearMask;
  uint8_t portValueData0Clock1 = portValueData0 | clockPinSetMask;  
  
  for (uint8_t i=5; i; i--)
  {
    uint8_t columnPixels = ~ pgm_read_byte(pixels++);
    uint8_t count = 7;

    do
    {
      columnPixels >>= 1;

      // This fancy logic is a trick to avoid having to branch.
      // Branching is more expensive.
      uint8_t selectMask = (columnPixels & 0b00000001) - 1; // Result = 0 or 0xFF.
      uint8_t dataPinSetMaskMasked = dataPinSetMask & selectMask;
      
      *port = portValueData0Clock1 | dataPinSetMaskMasked;
      *port = portValueData0Clock0 | dataPinSetMaskMasked;
      
    } while (--count);
  }    
}

void PrimeVfd::clockCustomCharacter(uint8_t character)
{
  const uint8_t *pixels = _customCharacterSet[character];
  
  volatile uint8_t *port = _port;
  uint8_t dataPinSetMask = _dataPinMask;
  uint8_t clockPinSetMask = _clockPinMask;
  uint8_t dataPinClearMask = ~dataPinSetMask;
  uint8_t clockPinClearMask = ~clockPinSetMask;

  uint8_t portValueData0 = *port & dataPinClearMask;
  uint8_t portValueData0Clock0 = portValueData0 & clockPinClearMask;
  uint8_t portValueData0Clock1 = portValueData0 | clockPinSetMask;  
    
  for (uint8_t i=5; i; i--)
  {
    uint8_t columnPixels = ~ *(pixels++);
    uint8_t count = 7;

    do
    {
      columnPixels >>= 1;

      // This fancy logic is a trick to avoid having to branch.
      // Branching is more expensive.
      uint8_t selectMask = (columnPixels & 0b00000001) - 1; // Result = 0 or 0xFF.
      uint8_t dataPinSetMaskMasked = dataPinSetMask & selectMask;
      
      *port = portValueData0Clock1 | dataPinSetMaskMasked;
      *port = portValueData0Clock0 | dataPinSetMaskMasked;
      
    } while (--count);
  }    
}

void PrimeVfd::clockColumnSelect(uint8_t column)
{
  volatile uint8_t *port = _port;
  uint8_t dataPinSetMask = _dataPinMask;
  uint8_t clockPinSetMask = _clockPinMask;
  uint8_t dataPinClearMask = ~dataPinSetMask;
  uint8_t clockPinClearMask = ~clockPinSetMask;
  
  uint8_t portClock0Value = *port & dataPinClearMask & clockPinClearMask;
  uint8_t portClock1Value = (*port & dataPinClearMask) | clockPinSetMask;
    
  // Send two zero bits as required by the display.
  *port = portClock1Value;
  __asm__("nop\n\t"); // Timing delay.
  *port = portClock0Value;
  __asm__("nop\n\t"); // Timing delay.
  *port = portClock1Value;
  __asm__("nop\n\t"); // Timing delay.
  *port = portClock0Value;
    
  // Count out zeros until we hit column.
  uint8_t i = 0;
  for(; i < column; ) 
  {
    // Clock in more zeros.
    *port = portClock1Value;

	i++; // Between writes to add a timing delay.

    *port = portClock0Value;
  }
  
  // Set column bit.
  i++;
  *port = portClock1Value | dataPinSetMask;
  *port = portClock0Value | dataPinSetMask; // OR instruction adds timing delay.
  
  // Set rest of zeros.
  for(; i < 20; ) 
  {
    *port = portClock1Value;

    i++; // Between writes to add a timing delay.

    *port = portClock0Value;
  }
}

uint8_t PrimeVfd::getCursorCharacter(uint8_t originalCharacter)
{
  if (getFlag(FLAG_BLINK_ON))
  {
    if (_blinkInterruptCount < CURSOR_BLINK_INTERRUPT_ON_COUNT)
    {
      return getFlag(FLAG_CURSOR_ON) ? CURSOR_CHARACTER : BLINK_CHARACTER;
    }
  }
  else if (getFlag(FLAG_CURSOR_ON))
  {
    return CURSOR_CHARACTER;
  }

  return originalCharacter;
}

void PrimeVfd::displayUpdate()
{
  if (!_haveMultipleInstances)
  {
    // Latch in the result from the previous update. We do this here instead of at 
    // the bottom of this method so chracter brightness isn't as sensitive to run 
    // time of this method. Only works if we have a single instance.
    *_port &= ~_latchPinMask;
    delayMicroseconds(1);
    *_port |= _latchPinMask;
  }
	
  // Figure out what index into the display data buffer.  
  uint8_t displayDataIndex = _currentUpdateColumn + _displayOffset;
  if (displayDataIndex >= 40)
  {
    displayDataIndex -= 40;
  }

  // First row character.
  uint8_t character = _displayData[displayDataIndex];
  if (_addressCounter == displayDataIndex)
  {
    // This is the cursor.
    character = getCursorCharacter(character);
  }
  if (character < NUM_CUSTOM_CHARACTERS)
  {
    clockCustomCharacter(character);
  }
  else
  {
    clockCharacter(character);
  }
  
  // Second row character.
  character = _displayData[displayDataIndex+40];
  if (_addressCounter == (displayDataIndex + 64))
  {
    // This is the cursor.
    character = getCursorCharacter(character);
  }
  if (character < NUM_CUSTOM_CHARACTERS)
  {
    clockCustomCharacter(character);
  }
  else
  {
    clockCharacter(character);
  }
  
  clockColumnSelect(DISPLAY_NUM_COLUMNS - 1 - _currentUpdateColumn); // Display column indexing is right to left.

  if (_haveMultipleInstances)
  {
    // We must latch data in now if we have multple instances. Otherwise the
    // next instance can clock in our data.
    *_port &= ~_latchPinMask;
    delayMicroseconds(1);
    *_port |= _latchPinMask;
  }

  // Increment to next column.
  _currentUpdateColumn++;
  if (_currentUpdateColumn >= DISPLAY_NUM_COLUMNS)
  {
    _currentUpdateColumn = 0; 
  }
}

