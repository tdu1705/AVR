
/*
 * Copyright (c) 2006-2012 by Roland Riegel <feedback@roland-riegel.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sfr_defs.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>

//#include "uart.h"

/* some mcus have multiple uarts */
#ifdef UDR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UDR UDR0

#define UCSRA UCSR0A
#define UDRE UDRE0
#define RXC RXC0

#define UCSRB UCSR0B
#define RXEN RXEN0
#define TXEN TXEN0
#define RXCIE RXCIE0

#define UCSRC UCSR0C
#define URSEL 
#define UCSZ0 UCSZ00
#define UCSZ1 UCSZ01
#define UCSRC_SELECT 0
#else
#define UCSRC_SELECT (1 << URSEL)
#endif

#ifndef USART_RXC_vect
#if defined(UART0_RX_vect)
#define USART_RXC_vect UART0_RX_vect
#elif defined(UART_RX_vect)
#define USART_RXC_vect UART_RX_vect
#elif defined(USART0_RX_vect)
#define USART_RXC_vect USART0_RX_vect
#elif defined(USART_RX_vect)
#define USART_RXC_vect USART_RX_vect
#elif defined(USART0_RXC_vect)
#define USART_RXC_vect USART0_RXC_vect
#elif defined(USART_RXC_vect)
#define USART_RXC_vect USART_RXC_vect
#else
#error "Uart receive complete interrupt not defined!"
#endif
#endif

#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define USE_SLEEP 1
#include <util/setbaud.h>

void uart_init0(unsigned int baud){
	//void uart_init0(unsigned int BAUDRATE){
	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	//UCSR0C |= 0b00000011;
	// UCSR0C |= (1<<USBS1)|(1<<UCSZ10);
	//UCSR0C |= (1<<UCSZ00);
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

//8 BIT
void uart_transmit0( unsigned char data ){
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char uart_receive0( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void uart_transmit_s0(char * str)
{
	while(*str) uart_transmit0(*str++);
}

void uart_init1(unsigned int baud){
	//void uart_init0(unsigned int BAUDRATE){
	UBRR1H = (BAUDRATE>>8);
	UBRR1L = BAUDRATE;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 2stop bit */
	//UCSR0C |= 0b00000011;
	//UCSR1C |= (1<<USBS1)|(1<<UCSZ11);
	//UCSR0C |= (1<<UCSZ00);
	UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);
}

//8 BIT
void uart_transmit1( unsigned char data ){
	/* Wait for empty transmit buffer */
	while (!( UCSR1A & (1<<UDRE1)));
	/* Put data into buffer, sends the data */
	UDR1 = data;

}

unsigned char uart_receive1( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	return UDR1;
}



void uart_transmit_s1(char * str)
{
	while(*str) uart_transmit1(*str++);

}

void uart_sendl(uint32_t liczba)
{
	char buf[17];
	ltoa(liczba,buf,10);
	uart_transmit_s1(buf);

}

EMPTY_INTERRUPT(USART_RXC_vect)

