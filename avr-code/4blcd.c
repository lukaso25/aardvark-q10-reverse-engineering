/*! \file 4blcd.c \brief Library for communication with Hitachi HD44780 based LCD's in 4bit mode - writing only */

/*
 * Copyright (c) 2011, Lukas Otava, http://lukaso.iglu.cz, All rights reserved.
 * This code is distributed under the GNU Public License which can be found at http://www.gnu.org/licenses/gpl.txt
*/

/* Temporary version, only basic funcionality */

#include <avr/io.h>
#include <util/delay.h>

#include "4blcd.h"

void LCD_ins(uint8_t zapsat)
{
	LCDPORT_C &=~LCD_RS;
	LCDPORT &= 0xF0;
	LCDPORT |= ((zapsat>>4) & 0x0F);
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &=~LCD_E;
	LCDPORT &= 0xF0;
	LCDPORT |= (zapsat & 0x0F);
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &=~LCD_E;
	if((zapsat&0b11)!=0) _delay_ms(2); else _delay_us(40);
}

void LCD_data(uint8_t zapsat)
{
	LCDPORT_C |=LCD_RS;
	LCDPORT &= 0xF0;
	LCDPORT |= ((zapsat>>4) & 0x0F);
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &=~LCD_E;
	LCDPORT &= 0xF0;
	LCDPORT |= (zapsat & 0x0F);
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &=~LCD_E;
	_delay_us(40);
}

void LCD_init(void)
{
	LCDPORTDDR |= 0b1111;
	LCDPORTDDR_C |= LCD_E|LCD_RS;

	LCDPORT_C &= ~(LCD_E|LCD_RS);

	_delay_ms(100);
	_delay_ms(100);

	LCDPORT &= ~0x0F;

	LCDPORT |= 0b0011;
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &= ~LCD_E;
	_delay_ms(4);

	LCDPORT |= 0b0011;
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &= ~LCD_E;
	_delay_ms(4);

	LCDPORT |= 0b0011;
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &= ~LCD_E;
	_delay_ms(4);

	LCDPORT &= ~0x0F;
	LCDPORT |= 0b0010;
	LCDPORT_C |= LCD_E;
	asm("nop");
	LCDPORT_C &= ~LCD_E;
	_delay_ms(3);

	//0 0 1 DL N F — — Sets interface data length (DL), number of display lines (N), and character font (F).
	LCD_ins(0x20|(1<<3)); //2line

	//0 0 0 0 1 D C B Sets entire display (D) on/off,cursor on/off (C), and blinking of cursor positioncharacter (B).
	LCD_ins(0x08|(1<<2)|(1<<1)|(1<<0)); //zapnem display, cursor - blinking

	//0 0 0 0 0 1 I/D S Sets cursor move direction and specifies display shift. These operations are performed during data write and read.
	LCD_ins(0x04|(1<<1)|(0<<0));//increment, no shift

	//0 0 0 1 S/C R/L — — Moves cursor and shifts display without changing DDRAM contents.
	LCD_ins(0x10|(0<<3)|(0<<2));

}

void LCD_goto(uint8_t x, uint8_t y)
{
	if(x==0){
		LCD_ins(y|0b10000000);
	} else LCD_ins(y|0b11000000);
}

void LCD_str(char *text)
{
	do
	{
		LCD_data(*text);
		text++;
	} while ((*text)!=0);
}

void LCD_defchar(uint8_t id, char  buffer)
{
	///char * b = buffer;
	LCD_ins((id<<3)|0b01000000);
	LCD_data(buffer);
	LCD_data(0);
	LCD_data(buffer);
	LCD_data(0);
	LCD_data(buffer);
	LCD_data(0);
	LCD_data(buffer);
	LCD_data(0);
}

#define VALUES_PER_SECTOR 5

void LCD_linebar_peak(uint8_t value, uint8_t peak)
{
	uint8_t value_sector, peak_sector;

	value_sector = value/VALUES_PER_SECTOR;
	peak_sector = peak/VALUES_PER_SECTOR;

	if (value_sector != peak_sector)
	{
		if (peak_sector > value_sector)
		{

		}
	}
	else
	{ // same sector
		if (peak_sector > value_sector)
		{

		}
	}
}
