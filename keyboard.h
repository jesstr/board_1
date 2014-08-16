#ifndef KEYBOARD_H
#define KEYBOARD_H


/* Keyboard initialization */
void InitKeyboard(void);

/* State machine for keyboard polling, contact bounce protection,
 * and pressed key recognition */
void ScanKeyboard(void);

/* Function returns key code if new key is pressed */
unsigned char GetKey(void);

#endif //KEYBOARD_H
