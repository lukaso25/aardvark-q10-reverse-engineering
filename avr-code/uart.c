/*! \file uart.c \brief Library handling UART0 unit */

/*
 * Copyright (c) 2010, Lukas Otava, http://lukaso.iglu.cz, All rights reserved.
 * This code is distributed under the GNU Public License which can be found at http://www.gnu.org/licenses/gpl.txt
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

void UART_init(uint8_t baudc)
{
	UBRRH = 0;
	UBRRL = baudc;
	UCSRB = (1<<RXEN)|(1<<TXEN);	
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void UART_rxint( void)
{
	UCSRB |= (1<<RXCIE);
	sei();
}

void UART_rxint_dis( void)
{
	UCSRB &= ~(1<<RXCIE);
}

void UART_tx( uint8_t data)
{
	//loop_until_bit_is_set(UCSRA, UDRE);
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}
	
uint8_t UART_rx( void)
{
	//loop_until_bit_is_set(UCSRA, UDRE);
	while ( !(UCSRA & (1<<RXC)));
	return UDR;
}

void UART_tx_s( char *text )
{
	while ( *text)
	{
		//loop_until_bit_is_set(UCSRA, UDRE);
		while ( !(UCSRA & (1<<UDRE)));
		UDR = *(text++);
	}
}

void UART_hex8( uint8_t num )
{
	uint8_t digit = (num>>4);
	UART_tx(digit>9?(digit+87):(digit+'0'));
	digit = num&0x0F;
	UART_tx(digit>9?(digit+87):(digit+'0'));
}
