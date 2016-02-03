/*! \file 4blcd.h \brief Library for communication with Hitachi HD44780 based LCD's in 4bit mode - writing only */
/*
 * Copyright (c) 2011, Lukas Otava, http://lukaso.iglu.cz, All rights reserved.
 * This code is distributed under the GNU Public License which can be found at http://www.gnu.org/licenses/gpl.txt
*/

/* Temporary version, only basic funcionality */

#ifndef __4BLCD_H__
#define __4BLCD_H__

//hw defs.
#ifndef lcdport

#define LCDPORT	PORTC	//data
#define	LCDPORTDDR	DDRC
#define LCDPORT_C PORTD	// control
#define LCDPORTDDR_C DDRD
#define	LCD_E	(1<<7)
#define LCD_RS	(1<<6)

#endif//LCDPORT

#define	cls()		LCD_ins(0x01)		//vymaž displej
#define cursor_off()	LCD_ins(0x0c)		//vypni kurzor
#define cursor_on()	LCD_ins(0x0e)		//zapni kurzor
#define cursor_blink()	LCD_ins(0x0f)		//kurzor bliká
#define line_1()	LCD_ins(0x80)		//jdi na øádek 1
#define line_2()	LCD_ins(0xc0)		//jdi na øádek 2
#define shift_lcd_left()	LCD_ins(0x18)		//posuò text doleva
#define shift_lcd_right()	LCD_ins(0x1c)		//posuò text doprava

void LCD_ins(uint8_t zapsat);
void LCD_data(uint8_t zapsat);
void LCD_init(void);
void LCD_goto(uint8_t x, uint8_t y);
void LCD_str( char *text );

void LCD_defchar(uint8_t id, char  buffer);

void LCD_linebar_peak(uint8_t value, uint8_t peak );

#endif // __4BLCD_H__
