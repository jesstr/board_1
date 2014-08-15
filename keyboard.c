#include "keyboard.h"

/* Current state of the state machine */
unsigned char keyState;
/* Last pressed key code */
unsigned char keyCode;
/* Last pressed key value */
unsigned char keyValue;
/* Flag, set if key is held */
unsigned char keyDown;
/* Flag, set if new key is pressed */
unsigned char keyNew;

/* Lookup table */
unsigned char keyTable[][2] = {
{ 0xEE, '1'},
{ 0xED, '2'},
{ 0xEB, '3'},
{ 0xDE, '4'},
{ 0xDD, '5'},
{ 0xDB, '6'},
{ 0xBE, '7'},
{ 0xBD, '8'},
{ 0xBB, '9'},
{ 0x7E, '*'},
{ 0x7D, '0'},
{ 0x7B, '#'},
{ 0xE7, 'A'},
{ 0xD7, 'B'},
{ 0xB7, 'C'},
{ 0x77, 'D'}
};


/* Function returns true if any key is pressed */
unsigned char AnyKey(void);
/* Function returns true if the pressed key
 * is the same as the last pressed key */
unsigned char SameKey(void);
/* Scan keyboard and get key code */
void ScanKey(void);
/* Find key value by key code, set flags */
unsigned char FindKey(void);
/* Clear key held flag */
void ClearKey(void);


/* Keyboard initialization */
void InitKeyboard(void)
{
  DDRB= 0xF0;
  PORTB= 0x0F;

  keyState = 0;
  keyCode = 0;
  keyValue = 0;
  keyDown = 0;
  keyNew = 0;
}

/* State machine for keyboard polling, contact bounce protection,
 * and pressed key recognition */
void ScanKeyboard(void)
{
   switch (keyState){
     case 0:
       if (AnyKey()) {
         ScanKey();
         keyState = 1;
       }
       break;

     case 1:
       if (SameKey()) {
           FindKey();
           keyState = 2;
       }
       else keyState = 0;
       break;

     case 2:
        if (SameKey()){}
        else keyState = 3;
        break;

     case 3:
       if (SameKey()) {
         keyState = 2;
       }
       else {
         ClearKey();
         keyState = 0;
       }
       break;

     default:
        break;
   }

}

/* Function returns true if any key is pressed */
unsigned char AnyKey(void)
{
  PORTB = 0x0F;
  _delay_us(1);
  return (~(0xF0 | (PINB & 0x0F)));
}

/* Function returns true if the pressed key
 * is the same as the last pressed key */
unsigned char SameKey(void)
{
  PORTB = (PORTB & 0x0f) | ( keyCode & 0xf0);
  _delay_us(1);
  return (!(PINB & (~keyCode)));
}

/* Scan keyboard and get key code */
void ScanKey(void)
{
  unsigned char activeRow = 0x10;
  while (activeRow>0) {
    PORTB= (~activeRow);
    _delay_us(1);
    if ((PINB & 0x0F)!=0x0F) {
      keyCode = PINB;
    }
    activeRow <<= 1;
  }
}

/* Find key value by key code, set flags */
unsigned char FindKey(void)
{
  unsigned char index;
  for (index = 0; index < 16; index++) {
    if (keyTable [index][0] == keyCode) {
      keyValue = keyTable [index][1];
      keyDown = 1;
      keyNew = 1;
      return 1;
    }
  }
  return 0;
}

/* Clear key held flag */
void ClearKey(void)
{
  keyDown = 0;
}

/* Function returns key code if new key is pressed */
unsigned char GetKey(void)
{
  if (keyNew){
    keyNew = 0;
    return keyValue;
  }
  else
    return 0;
}
