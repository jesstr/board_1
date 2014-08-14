#include "keyboard.h"

//хранит текущее состояние автомата
unsigned char keyState;
//хранит код нажатой кнопки
unsigned char keyCode;
//хранит символьное значение нажатой кнопки
unsigned char keyValue;
//флаговая переменная - устанавливается, если кнопка удерживается
unsigned char keyDown;
//флаговая переменная -  устанавливается, когда нажата новая кнопка
unsigned char keyNew;

//таблица перекодировки
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

//прототипы функций используемых автоматом
unsigned char AnyKey(void);
unsigned char SameKey(void);
void ScanKey(void);
unsigned char FindKey(void);
void ClearKey(void);

//инициализация портов, обнуление переменных
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

//автомат реализующий опрос клавиатуры, защиту от дребезга
// и распознование нажатой кнопки
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

//возвращает true если какая-нибудь кнопка нажата
unsigned char AnyKey(void)
{
  PORTB = 0x0F;
  _delay_us(1);
  return (~(0xF0 | (PINB & 0x0F)));
}

// возвращает true если удерживается та же кнопка
//что и в предыдущем цикле опроса
unsigned char SameKey(void)
{
  PORTB = (PORTB & 0x0f) | ( keyCode & 0xf0);
  _delay_us(1);
  return (!(PINB & (~keyCode)));
}

//Генерирует нужные сигналы на линиях
//считывает код нажатой кнопки
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

// преобразует код кнопки в соответствующий символ
// устанавивает флаги
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

//сбрасывает флаг
void ClearKey(void)
{
  keyDown = 0;
}

//если зафиксировано нажатие кнопки
//возвращает ее код
unsigned char GetKey(void)
{
  if (keyNew){
    keyNew = 0;
    return keyValue;
  }
  else
    return 0;
}
