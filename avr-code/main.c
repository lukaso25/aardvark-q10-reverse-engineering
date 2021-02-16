#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

#include "aardvark-spi.h"
#include "midi_control.h"

// MIDI buffer
uint8_t MIDI_CC_data[9] = {63,63,63,63,63,63,63,63,63};
volatile uint16_t MIDI_CC_ch_update;


ISR( USART_RXC_vect)
{
	midi_control_process_byte(UDR);
}

void midi_control_event_callback( uint8_t controller, uint8_t value)
{
	MIDI_CC_data[controller] = value;
	MIDI_CC_ch_update |= (1<<controller);
}


ISR( INT1_vect )
{
	UART_tx_s("CS8427 interrupt occurs.\r\n");
}

void aardvark_select_write( const uint8_t chip, const uint8_t * data, const uint8_t n)
{
	aardvark_select_chip(chip);
	aardvark_spi_write(data,n);
	aardvark_unselect_chip(chip);
}


//void AKM_init_format( mck, format );
//void AKM_set_volume( chip, channel ,volume );

enum { CLOCK_256fs = 0 , CLOCK_512fs } Masterclock_Divider;


void AKM_conf_clock( const uint8_t clock)
{
	uint8_t data[2];
	// AKM konfigurace

	// format selection - MODE2: 2x 24bit LJ
	data[1] = AKM_DIF1;

	// master clock selection
	switch (clock)
	{
	case CLOCK_256fs:
		data[1] |= 0;
		break;
	case CLOCK_512fs:
	default:
		data[1] |= AKM_CKS0;
		break;
	}

	data[0] = AKM_ClockAndFormat_Adr;
	aardvark_select_write(AKM1,data,2);
	aardvark_select_write(AKM2,data,2);
	aardvark_select_write(AKM3,data,2);
	aardvark_select_write(AKM4,data,2);

	//power
	/*data[0] = AKM_PowerDownReg_Adr;
	data[1] = 0;
	aardvark_select_write(AKM1,data,2);
	aardvark_select_write(AKM2,data,2);
	aardvark_select_write(AKM3,data,2);
	aardvark_select_write(AKM4,data,2);*/

	//reset
	data[0] = AKM_ResetControl_Adr;
	data[1] = AKM_RSTDA | AKM_RSTAD;
	aardvark_select_write(AKM1,data,2);
	aardvark_select_write(AKM2,data,2);
	aardvark_select_write(AKM3,data,2);
	aardvark_select_write(AKM4,data,2);

}

void AKM_conf_volume_out(const uint8_t channels, const uint8_t volume )
{
	uint8_t data[2];
		data[1] = volume;
		if (channels&(1<<0))
		{
			data[0] = AKM_LAtt_Adr;
			aardvark_select_write(AKM1,data,2);
		}
		if (channels&(1<<1))
		{
			data[0] = AKM_RAtt_Adr;
			aardvark_select_write(AKM1,data,2);
		}
		if (channels&(1<<2))
		{
			data[0] = AKM_LAtt_Adr;
			aardvark_select_write(AKM2,data,2);
		}
		if (channels&(1<<3))
		{
			data[0] = AKM_RAtt_Adr;
			aardvark_select_write(AKM2,data,2);
		}
		if (channels&(1<<4))
		{
			data[0] = AKM_LAtt_Adr;
			aardvark_select_write(AKM3,data,2);
		}
		if (channels&(1<<5))
		{
			data[0] = AKM_RAtt_Adr;
			aardvark_select_write(AKM3,data,2);
		}
		if (channels&(1<<6))
		{
			data[0] = AKM_LAtt_Adr;
			aardvark_select_write(AKM4,data,2);
		}
		if (channels&(1<<7))
		{
			data[0] = AKM_RAtt_Adr;
			aardvark_select_write(AKM4,data,2);
		}
}


// TODO: zkontrolovat, jak sed� lev� a prav� kan�l
void AKM_conf_volume_in(const uint8_t channels, const uint8_t volume )
{
	uint8_t data[2];
	data[1] = volume;
	if (channels&(1<<0))
	{
		data[0] = AKM_LIPGA_Adr;
		aardvark_select_write(AKM1,data,2);
	}
	if (channels&(1<<1))
	{
		data[0] = AKM_RIPGA_Adr;
		aardvark_select_write(AKM1,data,2);
	}
	if (channels&(1<<2))
	{
		data[0] = AKM_LIPGA_Adr;
		aardvark_select_write(AKM2,data,2);
	}
	if (channels&(1<<3))
	{
		data[0] = AKM_RIPGA_Adr;
		aardvark_select_write(AKM2,data,2);
	}
	if (channels&(1<<4))
	{
		data[0] = AKM_LIPGA_Adr;
		aardvark_select_write(AKM3,data,2);
	}
	if (channels&(1<<5))
	{
		data[0] = AKM_RIPGA_Adr;
		aardvark_select_write(AKM3,data,2);
	}
	if (channels&(1<<6))
	{
		data[0] = AKM_LIPGA_Adr;
		aardvark_select_write(AKM4,data,2);
	}
	if (channels&(1<<7))
	{
		data[0] = AKM_RIPGA_Adr;
		aardvark_select_write(AKM4,data,2);
	}
}


// TODO: zkontrolovat, jak sed� lev� a prav� kan�l
void sw4052_595_input_gain(const uint8_t channels, const uint8_t volume )
{
	static uint8_t registers[4];

	/*uint8_t data[2];
	data[1] = volume;
	if (channels&(1<<0))
	{
	}
	if (channels&(1<<1))
	{
	}
	if (channels&(1<<2))
	{
	}
	if (channels&(1<<3))
	{
	}
	if (channels&(1<<4))
	{
	}
	if (channels&(1<<5))
	{
	}
	if (channels&(1<<6))
	{
	}
	if (channels&(1<<7))
	{
	}*/

}

// TODO: zkontrolovat, jak sed� lev� a prav� kan�l
void cs3310_atenuator(const uint8_t channels, const uint8_t volume )
{
	static uint8_t registers[4];

	/*uint8_t data[2];
	data[1] = volume;
	if (channels&(1<<0))
	{
	}
	if (channels&(1<<1))
	{
	}
	if (channels&(1<<2))
	{
	}
	if (channels&(1<<3))
	{
	}
	if (channels&(1<<4))
	{
	}
	if (channels&(1<<5))
	{
	}
	if (channels&(1<<6))
	{
	}
	if (channels&(1<<7))
	{
	}
	*/
}


void CS8427_conf_clock(const uint8_t clock)
{
	uint8_t data[3];

	// CS8427 konfigurace SPDIF out - mck = 256*fs
	data[0] = 0b00100000; // adresa

	// control 1
	data[1] = 0x01;
	data[2] = 0;//(1<<7);//(1<<7)|(1<<2); // SWCLK, INT1
	aardvark_select_write(CS8427,data,3);

	// Control 2 net�eba m�nit

	// Data Flow Control net�eba m�nit

	// Clock Source Control - STOP
	data[1] = 0x04;
	//data[2] = (1<<5)|(1<<3)|(1<<1)|(1<<0); //CLK1, OUTC, RXD0, RXD1, RXD0
	data[2] = 0;//(1<<3)|(1<<0); // OUTC, RXD0
	aardvark_select_write(CS8427,data,3);

	// Serial Audio Input Port Data Format (05h)
	data[1] = 0x05;
	//data[2] = (1<<7)|(1<<2)|(1<<5); // SIMS, SIDEL, SIRES
	data[2] = (1<<7)|(1<<5); // SIMS, SIRES
	aardvark_select_write(CS8427,data,3);

	//Serial Audio Output Port Data Format (06h)
	data[1] = 0x06;
	data[2] = (1<<5);// SORES1 - 16bit LJ
	aardvark_select_write(CS8427,data,3);

	// Clock Source Control - RUN
	data[1] = 0x04;
	//data[2] = (1<<6)|(1<<5)|(1<<3)|(1<<1)|(1<<0); //RUN, CLK1, OUTC, RXD0, RXD1, RXD0
	//data[2] = (1<<6)|(1<<3)|(1<<0); // OUTC, RXD0 - toto funguje se spdif
	data[2] = (1<<6)|(1<<2)|(1<<1)|(1<<5);

	// master clock selection
	switch (clock)
	{
	case CLOCK_256fs:
		data[2] = (1<<6)|(1<<2)|(1<<1);
		break;
	case CLOCK_512fs:
	default:
		data[2] = (1<<6)|(1<<2)|(1<<1)|(1<<5);
		break;
	}

	aardvark_select_write(CS8427,data,3);
}


/*
	case 88200:
		LL_GPIO_ResetOutputPin(CLK_48_GPIO_Port,CLK_48_Pin);
		LL_GPIO_ResetOutputPin(CLK_x2_GPIO_Port,CLK_x2_Pin);
		return fs;
	case 44100:
		LL_GPIO_ResetOutputPin(CLK_48_GPIO_Port,CLK_48_Pin);
		LL_GPIO_SetOutputPin(CLK_x2_GPIO_Port,CLK_x2_Pin);
		return fs;
	case 96000:
		LL_GPIO_SetOutputPin(CLK_48_GPIO_Port,CLK_48_Pin);
		LL_GPIO_ResetOutputPin(CLK_x2_GPIO_Port,CLK_x2_Pin);
		return fs;
	case 48000:
		LL_GPIO_SetOutputPin(CLK_48_GPIO_Port,CLK_48_Pin);
		LL_GPIO_SetOutputPin(CLK_x2_GPIO_Port,CLK_x2_Pin);
		return fs;
 * */



int main (void)
{
	uint8_t data[5];

	// chipselects high
	if (aardvark_spi_init() > 0)
	{
		while(1);
	}

	UART_init(15); // 15 pro 31250
	UART_rxint();
	UART_tx_s("Ahoj, tady mod Aardvark!\r\n");

	// Preamp 4052 gain
	data[0] = 0b11111111;
	data[1] = 0b11111111;
	aardvark_select_write(GAIN595,data,2);

	// Preamp 3310 volume ~ 0 dB
	data[0] = 0b11000000;
	data[1] = 0b11000000;
	aardvark_select_write(VOL1,data,2);
	aardvark_select_write(VOL2,data,2);
	aardvark_select_write(VOL3,data,2);
	aardvark_select_write(VOL4,data,2);


	// hodiny oscil�tory 44.1* a 48*
	DDRB |= (1<<6)|(1<<7);
	PORTB |= (1<<6)|(1<<7);
	UART_tx_s("Clock 44.1 x.\r\n");
	//UART_tx_s("Clock 48.0 x.\r\n");


	//CS8427_conf_clock( CLOCK_256fs);
	//AKM_conf_clock( CLOCK_256fs);
	//UART_tx_s("Divider 256fs.\r\n");

	CS8427_conf_clock( CLOCK_512fs);
	AKM_conf_clock( CLOCK_512fs);
	UART_tx_s("Divider 512fs.\r\n");

	AKM_conf_volume_out(0xFF, 127);
	AKM_conf_volume_in(0xFF, 127);


	//LED on
	DDRB |= (1<<1);
	PORTB |= (1<<1);


	while(1)
	{
		//PORTB ^= (1<<1);
		if (MIDI_CC_ch_update)
		{
			MIDI_CC_ch_update = 0;
			UART_tx_s("MIDI CC received.\r\n");
		}
	}
	return 0;
}


/*void twi_slave_write_callback(const uint8_t index, const uint8_t len)
{
	uint8_t i, last_index;

	//UART_tx_s("TWI event callback flag!\r\n");

	if (len == 0)
	{
		// status register update
	}

	last_index = index + len;

	for( i = index; i<last_index; i++ )
	{
		if (i == 0)// conf register (0)
		{
			// hodiny
			if (1)
			{

			}
			else
			{

			}
			// 2x hodiny

			//
			LCD_goto(1,0);
			LCD_str("conf   ");
		}
		if(i>0)// gain registers (1-8)
		{
			LCD_goto(1,0);
			//AKM_conf_volume_in((1<<(i-1)),mem[i]&0x3F);
			//sw4052_595_input_gain((1<<(i-1)),mem[i]>>6);
			LCD_str("gain   ");
		}
		if(i>8)// input volume registers (9-16)
		{
			LCD_goto(1,0);
			//AKM_conf_volume_in((1<<(i-9)),mem[i]);
			cs3310_atenuator((1<<(i-9)),mem[i]);
			LCD_str("input  ");
		}
		if(i>16)// output volume registers (17-25)
		{
			LCD_goto(1,0);
			//AKM_conf_volume_out((1<<(i-17)),mem[i]);
			LCD_str("output ");
		}
	}
}*/

