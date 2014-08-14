#ifndef KEYBOARD_H
#define KEYBOARD_H

//#include <ioavr.h>

//инициализация портов и внутренних переменных
void InitKeyboard(void);

//сканирование клавиатуры
void ScanKeyboard(void);

//возвращает код нажатой кнопки
unsigned char GetKey(void);

#endif //KEYBOARD_H
