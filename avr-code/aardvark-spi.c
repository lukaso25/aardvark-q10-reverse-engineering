/*
 * aardvark-spi.c
 *
 *  Created on: 24.5.2014
 *      Author: Lukas
 */

#include "aardvark-spi.h"
#include <util/delay.h>

uint8_t aardvark_spi_init( void)
{
	//! GPIO init

	// CS3310 chip selects
	DDRC  |= 0x0f;
	PORTC |= 0x0f;

	// AKM4524 chip selects
	DDRB  |= (1<<0);
	PORTB |= (1<<0);
	DDRD  |= (0b111<<5);
	PORTD |= (0b111<<5);

	// CS8427 chip select
	DDRB  |= (1<<2);
	PORTB |= (1<<2);

	// 74HC595 chip select
	DDRD  |= (1<<4);
	PORTD |= (1<<4);

	// CS8427 INTERUPT
	DDRD  &= ~(1<<3);
	PORTD |= (1<<3); // PULL UP

	// SPI GPIO
	DDRB  |= (1<<3)|(1<<5);
	PORTB |= (1<<3)|(1<<5);
	DDRB  &= ~(1<<4);
	PORTB = (1<<4);// pullup na MISO

	//! SPI INIT sck =  fclk /16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	//budeme nastavovat mosi se sestupnou hranou, samplovat s nastupnou hranou CPOL=0, CPHA=0

	//595 data sampled on rising, nástupná hrana CS pøenese na výstup
	//8427 data is sampled on rising edge of SCK, faling edge to DOUT, CS low, MSB first
	//3310 data is sampled on rising edge of SCK, CS low, MSB first
	//AKM data is sampled on rising edge of SCK, CS low, MSB first

	//! reset pulse
	//! kontrola stavu
	DDRD  &= ~(1<<2);
	PORTD |= (1<<2);

	if ((PIND&(1<<2)) == 0)
	{
		return 1;
	}

	DDRD  |= (1<<2);
	PORTD &= ~(1<<2);

	_delay_ms(100);

	PORTD |= (1<<2);
	DDRD  &= ~(1<<2);

	return 0;
}

void aardvark_select_chip(uint8_t cs)
{
	switch(cs)
	{
	case AKM1:
		PORTB &= ~(1<<0);
		break;
	case AKM2:
		PORTD &= ~(1<<7);
		break;
	case AKM3:
		PORTD &= ~(1<<6);
		break;
	case AKM4:
		PORTD &= ~(1<<5);
		break;
	case VOL1:
		PORTC &= ~(1<<3);
		break;
	case VOL2:
		PORTC &= ~(1<<2);
		break;
	case VOL3:
		PORTC &= ~(1<<1);
		break;
	case VOL4:
		PORTC &= ~(1<<0);
		break;
	case GAIN595:
		PORTD &= ~(1<<4);
		break;
	case CS8427:
		PORTB &= ~(1<<2);
		break;
	}
}

void aardvark_unselect_chip(uint8_t cs)
{
	switch(cs)
	{
	case AKM1:
		PORTB |= (1<<0);
		break;
	case AKM2:
		PORTD |= (1<<7);
		break;
	case AKM3:
		PORTD |= (1<<6);
		break;
	case AKM4:
		PORTD |= (1<<5);
		break;
	case VOL1:
		PORTC |= (1<<3);
		break;
	case VOL2:
		PORTC |= (1<<2);
		break;
	case VOL3:
		PORTC |= (1<<1);
		break;
	case VOL4:
		PORTC |= (1<<0);
		break;
	case GAIN595:
		PORTD |= (1<<4);
		break;
	case CS8427:
		PORTB |= (1<<2);
		break;
	}
}


void aardvark_spi_write(const uint8_t * data, const uint8_t n)
{
	uint8_t toWrite = n;
	while (toWrite>0)
	{
		toWrite--;
		SPDR = *(data++);
		//loop_until_bit_is_set(SPSR, SPIF);
		while(!(SPSR & (1<<SPIF)));
	}
}

uint8_t aardvark_spi_read(uint8_t * data, uint8_t * n);
