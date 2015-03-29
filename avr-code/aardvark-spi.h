/*
 * \defgroup AardvarkSPI AardvarkSPI Module
 * @{
 * \date 24.5.2014
 * \author Lukas
 * \brief This module controls all SPI slaves in Aardvark Q10 external module
 *
 * This module contain low-level layer for SPI slaves communication
 *
 * \par Chip List:
 *
 * \par Pinout:
 *
 *
 */

#ifndef _AARDVARK_SPI_H_
#define _AARDVARK_SPI_H_

#include <avr/io.h>

// AKM registers
enum {
	AKM_PowerDownReg_Adr = 0|0b10100000,
	AKM_ResetControl_Adr = 1|0b10100000,
	AKM_ClockAndFormat_Adr = 2|0b10100000,
	AKM_DeemAndVolume_Adr = 3|0b10100000,
	AKM_LIPGA_Adr = 4|0b10100000,
	AKM_RIPGA_Adr = 5|0b10100000,
	AKM_LAtt_Adr = 6|0b10100000,
	AKM_RAtt_Adr = 7|0b10100000
} AKM4524_Registers;
enum {
	AKM_PWVR = (1<<2), AKM_PWAD = (1<<1), AKM_PWDA = (1<<0)
} AKM_PowerDownReg;
enum {
	AKM_RSTAD = (1<<1), AKM_RSTDA = (1<<0)
} AKM_ResetControl;
enum {
	AKM_DIF2 = (1<<7),
	AKM_DIF1 = (1<<6),
	AKM_DIF0 = (1<<5),
	AKM_CMODE = (1<<4),
	AKM_CKS1 = (1<<3),
	AKM_CKS0 = (1<<2),
	AKM_DFS1 = (1<<1),
	AKM_DFS0 = (1<<0)
} AKM_ClockAndFormat;
enum {
	AKM_SMUTE = (1<<7),
	AKM_ZCEI = (1<<4),
	AKM_ZTM1 = (1<<3),
	AKM_ZTM0 = (1<<2),
	AKM_DEM1 = (1<<1),
	AKM_DEM0 = (1<<0)
} AKM_DeemAndVolume;


//CS8427 registers
enum {
	CS8427_Control1 = 1,
	CS8427_Control2 = 2,
	CS8427_DataFlowControl = 3,
	CS8427_ClockSorceControl = 4,
	CS8427_SerialInputFormat = 5,
	CS8427_SerialOutputFormat = 6,
	CS8427_Interupt1Status = 7,
	CS8427_Interupt2Status = 8,
} CS8427_Registers;

// CS identifiers
enum {
	AKM1 = 0, AKM2, AKM3, AKM4,
	VOL1, VOL2, VOL3, VOL4,
	GAIN595,
	CS8427
} Aardvark_Chips;


/*! Initialization function */
uint8_t aardvark_spi_init( void);


void aardvark_select_chip(uint8_t cs);
void aardvark_unselect_chip(uint8_t cs);
void aardvark_spi_write(const uint8_t * data, const uint8_t n);
uint8_t aardvark_spi_read(uint8_t * data, uint8_t * n);


/*! @}*/
#endif /* _AARDVARK_SPI_H_ */
