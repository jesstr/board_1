#include <avr/io.h>
#include <util/delay.h>
#include "keyboard.h"

/* Current state of the state machine */
unsigned char keyState;
/* Last pressed key code */
unsigned int keyCode;
/* Last pressed key value */
unsigned char keyValue;
/* Flag, set if key is held */
unsigned char keyDown;
/* Flag, set if new key is pressed */
unsigned char keyNew;

/* Key value by code lookup table */
unsigned int keyTable[][2] = {
{ 0x0FEE, 0x60},
{ 0x0FED, 0x61},
{ 0x0FEB, 0x62},
{ 0x0FE7, 0x63},
{ 0x0FDE, 0x64},
{ 0x0FDD, 0x65},
{ 0x0FDB, 0x66},
{ 0x0FD7, 0x67},
{ 0x0FBE, 0x68},
{ 0x0FBD, 0x69},
{ 0x0FBB, 0x6A},
{ 0x0FB7, 0x6B},
{ 0x0F7E, 0x6C},
{ 0x0F7D, 0x6D},
{ 0x0F7B, 0x6E},
{ 0x0F77, 0x6F},
{ 0x0EFE, 0x70},
{ 0x0EFD, 0x71},
{ 0x0EFB, 0x72},
{ 0x0EF7, 0x73},
{ 0x0DFE, 0x74},
{ 0x0DFD, 0x75},
{ 0x0DFB, 0x76},
{ 0x0DF7, 0x77},
{ 0x0BFE, 0x78},
{ 0x0BFD, 0x79},
{ 0x0BFB, 0x7A},
{ 0x0BF7, 0x7B},
{ 0x07FE, 0x7C},
{ 0x07FD, 0x7D},
{ 0x07FB, 0x7E},
{ 0x07F7, 0x7F},
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
	DDRB = 0xFF; //Rows
    PORTB = 0x00;

    DDRA &= ~(0x1E); //Cols
    PORTA = 0x1E;

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
         /* Contact bounce protection delay */
         _delay_ms(50);
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
        if (SameKey()) {
        	;
        }
        else {
        	keyState = 3;
        }
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
  PORTB = 0x00;
  _delay_us(1);
  return (~(0xE1 | (PINA & 0x1E)));
}

/* Function returns true if the pressed key
 * is the same as the last pressed key */
unsigned char SameKey(void)
{
  PORTA = (PORTA & 0x1E);
  PORTB = (keyCode >> 4);
  _delay_us(1);
  return ( !( ( (PINB << 4) | ((PINA & 0x1E) >> 1) ) & (~keyCode) ) );
}

/* Scan keyboard and get key code */
void ScanKey(void)
{
  unsigned char activeRow = 0x01;
  while (activeRow > 0) {
    PORTB = (~activeRow);
    _delay_us(1);
    if ((PINA & 0x1E) != 0x1E) {
      keyCode = (PORTB << 4) | ((PINA & 0x1E) >> 1);
    }
    activeRow <<= 1;
  }
}

/* Find key value by key code, set flags */
unsigned char FindKey(void)
{
  unsigned char index;
  for (index = 0; index < 32; index++) {
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
