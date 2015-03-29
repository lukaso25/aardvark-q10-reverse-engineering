/*! \file uart.h \brief Library handling UART0 unit */

/*
 * Copyright (c) 2010, Lukas Otava, http://lukaso.iglu.cz, All rights reserved.
 * This code is distributed under the GNU Public License which can be found at http://www.gnu.org/licenses/gpl.txt
*/

#ifndef __UART_H__
#define __UART_H__

//! macro counting init function parametr according to speciffied bauderate
#define SPEED_FROM_BAUD(baud) (uint8_t)(((F_CPU)/16/(baud))-1)

//! dual usart device compatibility
#ifndef UCSRA // => remap sfr to USART0 if USART not defined
#define UCSRA	UCSR0A
#define UCSRB	UCSR0B
#define UCSRC	UCSR0C
#define UDR	UDR0
#define UBRRH	UBRR0H
#define UBRRL	UBRR0L
#define UDRE	UDRE0
#define RXEN	RXEN0
#define TXEN	TXEN0
#define URSEL	URSEL0
#define UCSZ0	UCSZ00
#define RXC	RXC0
#define RXCIE	RXCIE0
#endif


//!inicialization of module
void UART_init( uint8_t baudc);
//!function enable rx interrupt
void UART_rxint( void);
//!function disable rx interrupt
void UART_rxint_dis( void);

//!function send 1byte
void UART_tx( uint8_t data);

//!function waiting for 1byte data
uint8_t UART_rx( void);

//!fuction sent string from ram
void UART_tx_s( char *text);

void UART_tx_int( uint8_t num);

#endif //__UART_H__
