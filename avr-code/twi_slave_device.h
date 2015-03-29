#ifndef __TWI_SLAVE_DEVICE_H__
#define __TWI_SLAVE_DEVICE_H__

#include <avr/io.h>

/*!
 * \defgroup TWISlaveDevice TWISlaveDevice Module
 * @{
 *
 * \brief This is a driver for TWI (I2C compatible) slave device that looks like memory with linear adress space
 * \author Lukas Otava
 * \date  2014
 *
 * Driver is callback based. Callback is called when write operation was performed.
 *
 * Adress for reading is be set by write function.
 * Adress register is automaticly incremented.
 * When adress register overflows, operation is not acknowladged.
 *
 *
 * Changelog
 * ---------
 * - Driver added to Aardvark project.
 * - Tested through LittleWire USB-I2C interface.
 * - Out of memory access handling tested with succesfull result.
 * - NACK during time consuming callback function call.
 * - Base driver written.
 *
 *
 * \warning Driver not tested during power down mode. There are restrictions to power down modes, that are mentioned in device datasheet.
 *
 * */

/*!
 \brief  This is code example for module initialization
 \ingroup TWIslave
 \code
 #define TWI_SLAVE_DEVICE_MEMORY_SIZE		(10)
 #define TWI_SLAVE_ADRESS (0x15)
 uint8_t twi_slave_device_memory[TWI_SLAVE_DEVICE_MEMORY_SIZE];
 twi_slave_device_init( TWI_SLAVE_ADRESS, twi_slave_device_memory, TWI_SLAVE_DEVICE_MEMORY_SIZE);
 \code
 \param slave_7bit_adress this is slave device 7bit adress
 \param slave_buffer this is pointer to prealocated memory pool of slave device
 \param memory_size this is size of memory pool in bytes
 \warning 7-bit adress is shifted left internally
 * */
void twi_slave_device_init( uint8_t slave_7bit_adress, uint8_t * slave_buffer, uint8_t memory_size);


/*!
 \brief On write event callback function prototype

 Example of callback function
 \code
void twi_slave_write_callback(const uint8_t index, const uint8_t len)
{
	uint8_t i, last_index;

	if (len == 0)
	{
		// only adress of memory pool received (next i2C operation will be probably reading)
		// memory items could be updated after this event
		return;
	}

	last_index = index + len;
	for( i = index; i<last_index; i++ )
	{
		// each memory item processing
		if(i>1)
		{
			// update something
		}
	}
}

 \code
 \param index this is index of slave memory which was modified by master
 \param len amounth of modified memory
 \warning this function is called from interrupt routine - should be as short as possible
 * */
extern void twi_slave_write_callback(const uint8_t index, const uint8_t len);

/*! @} */

#endif//__TWI_SLAVE_DEVICE_H__
