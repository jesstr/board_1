#include "avr/io.h"
#include "uart.h"

/* UART initialization */
void UART_Init(unsigned int ubrr1)
{
	/* UBRR1H = (unsigned char)(ubrr1>>8); */	/* Setting operation frequency hight byte */
	UBRR1L = (unsigned char) ubrr1;				/* Setting operation frequency low byte */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); 				/* RX enabled, TX enabled */
	UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);	/* Asynchronous mode, 8 bit, 1 stop bit, no parity check */
	UCSR1B |= (1<<RXCIE1);						/* RX interrupt enabled */
}

/* Send a byte over UART */
void UART_SendByte(unsigned char byte1)
{
	while (!( UCSR1A & (1<<UDRE1)));
	UDR1 = byte1;
};

/* Send text string over UART */
void UART_SendString(char *buffer)
{
	while (*buffer!=0) {
		 UART_SendByte(*buffer++);
		 //buffer++;
	}
}

/* Send data over UART */
void UART_SendData(char *buffer, unsigned short nbytes)
{
	while (nbytes > 0) {
		UART_SendByte(*buffer++);
		nbytes--;
	}
}

/* RX Interrupt routine (has to be moved to the main file) */
/*
ISR(USART1_RX_vect)
{
	unsigned char buff=UDR1;
}
*/
