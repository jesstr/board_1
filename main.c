/*
 * main.c
 *
 *  Created on: 30.07.2014
 *      Author: Pavel Cherstvov
 */

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "keyboard.h"
#include "uart.h"

#define nop() asm volatile("nop")

#define PR_CLK_PIN	PG0
#define PR_CLK_PORT	PORTG
#define PR_CLK_DDR	DDRG

#define PR_RES_PIN	PG1
#define PR_RES_PORT	PORTG
#define PR_RES_DDR	DDRG

#define AD1_PIN		PC0
#define AD1_PORT	PORTC
#define AD1_DDR		DDRC

#define AD2_PIN		PC1
#define AD2_PORT	PORTC
#define AD2_DDR		DDRC

#define AD3_PIN		PC2
#define AD3_PORT	PORTC
#define AD3_DDR		DDRC

#define AD4_PIN		PC3
#define AD4_PORT	PORTC
#define AD4_DDR		DDRC

#define SHIFT_REGISTERS_CLOCK_DATA 								\
							do { 								\
							PR_CLK_PORT |= (1 << PR_CLK_PIN); 	\
							nop();						\
							PR_CLK_PORT &= ~(1 << PR_CLK_PIN); 	\
							nop();					\
							} while (0)

#define SHIFT_REGISTERS_RESET 									\
							do { 								\
							PR_RES_PORT &= ~(1 << PR_RES_PIN); 	\
							_delay_us(2);						\
							PR_RES_PORT |= (1 << PR_RES_PIN); 	\
							} while (0)

/* Data array for shift registers */
unsigned char LedDataReg[4];

void UpdateShiftRegisters(void);


/* UART RX Interrupt routine */
ISR(USART1_RX_vect)
{
	unsigned char buff = UDR1;

	/* debug */
	LedDataReg[2] = buff;
	LedDataReg[3] = 0x55;
	UpdateShiftRegisters();
	UART_SendByte(buff);
	/* debug */
}


inline void IO_Init(void) {
	PR_CLK_DDR |= (1 << PR_CLK_PIN);
	PR_RES_DDR |= (1 << PR_RES_PIN);
	PR_RES_PORT |= (1 << PR_RES_PIN);
	AD1_DDR |= (1 << AD1_PIN);
	AD2_DDR |= (1 << AD2_PIN);
	AD3_DDR |= (1 << AD3_PIN);
	AD4_DDR |= (1 << AD4_PIN);
}


void UpdateShiftRegisters(void) {
	unsigned char i, out_data;

	for (i = 0; i < 8; i++) {

		out_data = (LedDataReg[0] >> i) & 0x01;
		out_data |= ((LedDataReg[1] >> i) & 0x01) << 1;
		out_data |= ((LedDataReg[2] >> i) & 0x01) << 2;
		out_data |= ((LedDataReg[3] >> i) & 0x01) << 3;

		PORTC = out_data;

		SHIFT_REGISTERS_CLOCK_DATA;
	}
}


int main(void) {

	IO_Init();
	UART_Init(MYUBRR);
	InitKeyboard();
	sei();

	UART_SendString("Hello\n"); /* debug */

	LedDataReg[0] = 0x00;
	LedDataReg[1] = 0x00;
	LedDataReg[2] = 0x00;
	LedDataReg[3] = 0x00;

	unsigned char i, j, key;

	while (1) {

		ScanKeyboard();
		if ((key = GetKey()) != 0) {
			UART_SendByte(key);
		}

		/*
		for (j = 1; j < 5; j++) {
			for (i = 1; i < 9; i++) {
				LedDataReg[j - 1] = (1 << i) - 1;
				UpdateShiftRegisters();
				_delay_ms(1000);
			}
		}
		*/
	}

}
