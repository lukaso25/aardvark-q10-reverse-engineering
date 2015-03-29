#include <util/twi.h>
#include <avr/interrupt.h>

#include "twi_slave_device.h"

// private variables
static volatile uint8_t twi_slave_mem_adr;
static volatile uint8_t twi_slave_mem_adr_actual;
static volatile uint8_t twi_slave_state;

static volatile uint8_t * slave_memory_p;
static volatile uint8_t slave_memory_size;

enum TWI_SLAVE_STATES {TWI_SLAVE_RESET = 0, TWI_SLAVE_ADRESSED, TWI_SLAVE_TRANSMIT, TWI_SLAVE_RECEIVE, TWI_SLAVE_ERROR, TWI_SLAVE_OUTOFADRESS };

void twi_slave_device_init( uint8_t slave_7bit_adress, uint8_t * slave_buffer, uint8_t memory_size)
{
	// ulo��me slave adresu do registru
	//! tady by se dalo doplnit general call
	TWAR = (slave_7bit_adress<<1);

	// inicializace prom�nn�ch
	twi_slave_mem_adr = 0;
	twi_slave_mem_adr_actual = 0;
	twi_slave_state = TWI_SLAVE_RESET;

	// ulo��me si velikost bufferu a pointer na n�j
	slave_memory_p = slave_buffer;
	slave_memory_size = memory_size;
	
	// nastav�me GPIO piny - vstup s pullupem
	//DDRC &= ~((1<<5)|(1<<4));
	//PORTC |= (1<<5)|(1<<4);


	// nastav�me ��dic� registry
	TWBR = 0; // prescaler SCL only in master mode
	TWCR = (1<<TWIE)|(1<<TWEN)|(1<<TWEA); // povolen� periferie, p�eru�en�, acknowladge bitu

	// povol�me p�eru�en�
	sei();

	DDRB |= 3;
	PORTB |= 3;
}

ISR( TWI_vect)
{
	PORTB &= ~1;
	//TWI_DEBUG_STR("TWI p�eru�en�.\r\n");
	switch (TW_STATUS)
	{

	//! read from memory
	case TW_ST_SLA_ACK: // SLA+R received, ACK returned
	case TW_ST_DATA_ACK: //data transmitted, ACK received
	case TW_ST_LAST_DATA: // last data byte transmitted, ACK received
		// TODO : kdy toto nastane?
		TWDR = slave_memory_p[twi_slave_mem_adr_actual];
		twi_slave_mem_adr_actual++;
		if (twi_slave_mem_adr_actual >= slave_memory_size)
		{
			twi_slave_mem_adr_actual = slave_memory_size-1;
			// d�l u� ��st nebudeme, jsme na konci pam�ti
		}
		twi_slave_state = TWI_SLAVE_TRANSMIT;
		//TWI_DEBUG_STR("TWI po�adavek dat.\r\n");
		break;

	case TW_ST_DATA_NACK: //data transmitted, NACK received
		// nenad�l�me nic - posledn� data
		break;

	//! write into memory
	case TW_SR_DATA_NACK: // data received, NACK returned
		//�ekli jsme, �e nic nechceme
		TWCR |= (1<<TWEA);
		PORTB &= ~2;
		break;

	//! write into memory
	case TW_SR_DATA_ACK: // data received, ACK returned
		if ( twi_slave_state == TWI_SLAVE_ADRESSED) // prvn� bajt je adresa v pam�ti
		{
			twi_slave_mem_adr = TWDR;
			twi_slave_mem_adr_actual = twi_slave_mem_adr;
			if (twi_slave_mem_adr_actual >= slave_memory_size)
			{
				TWCR &= ~(1<<TWEA);
				twi_slave_state = TWI_SLAVE_OUTOFADRESS;
			}
			else
			{
				twi_slave_state = TWI_SLAVE_RECEIVE;
			}
			//TWI_DEBUG_STR("TWI adr.\r\n");
		}
		else // dal�� bajty jsou u� data k z�pisu
		{
			slave_memory_p[twi_slave_mem_adr_actual] = TWDR;
			twi_slave_mem_adr_actual++;
			if (twi_slave_mem_adr_actual >= slave_memory_size)
			{
				// d�l u� zapisovat nebudeme, jsme na konci pam�ti
				TWCR &= ~(1<<TWEA);
			}
			//TWI_DEBUG_STR("TWI data.\r\n");
		}
		break;

	case TW_SR_SLA_ACK: //SLA+W received, ACK returned
		twi_slave_state = TWI_SLAVE_ADRESSED;
		break;

	case TW_SR_STOP: // stop or repeated start condition received while selectec
		if (twi_slave_state == TWI_SLAVE_OUTOFADRESS)
		{
			TWCR |= (1<<TWEA);
		}
		// vynulovat nack
		if (twi_slave_state == TWI_SLAVE_RECEIVE)
		{
			TWCR &= ~(1<<TWEA);
			twi_slave_write_callback(twi_slave_mem_adr, twi_slave_mem_adr_actual-twi_slave_mem_adr);
		}
		TWCR |= (1<<TWEA);
		//TWI_DEBUG_STR("TWI stop od mastera.\r\n");
		twi_slave_state = TWI_SLAVE_RESET;
		break;

	//! other situations
		//case TW_REP_START:
		//case TW_START: // start transmitted, ne�e��me
		//case TW_ST_ARB_LOST_SLA_ACK: //arbitration lost in SLA+RW, SLA+R received, ACK returned
		//case TW_SR_ARB_LOST_SLA_ACK:
		//case TW_SR_GCALL_ACK  : //general call received, ACK returned
		//case TW_SR_ARB_LOST_GCALL_ACK : // arbitration lost in SLA+RW, general call received, ACK returned
		//case TW_SR_GCALL_DATA_ACK: //general call data received, ACK returned
		//case TW_SR_GCALL_DATA_NACK: //general call data received, NACK returned
		//case TW_BUS_ERROR:
		//case TW_NO_INFO:

	default:
		//budeme nulovat po�et bytu, nulovat p��znak chyby - default state
		twi_slave_state = TWI_SLAVE_ERROR;
		break;
	}

	//! sma�eme p��znak p�eru�en�
	TWCR |= (1<<TWINT);
	PORTB |= 3;
}

