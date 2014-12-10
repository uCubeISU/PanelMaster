/*
 * spi.h
 *
 *  Created on: Dec 9, 2014
 *      Author: vens
 */

#ifndef SPI_H_
#define SPI_H_

#include "utils.h"
#include "spi_registers.h"


#define SPI_HAS_A0
#define SPI_HAS_B0
//#define SPI_HAS_A1
//#define SPI_HAS_B1


typedef struct SPI_CONFIG_STRUCT
{
	enum SPI_MODULE
	{
#ifdef SPI_HAS_A0
		UCA0
#endif
#ifdef SPI_HAS_B0
		, UCB0
#endif
#ifdef SPI_HAS_A1
		, UCA1
#endif
#ifdef SPI_HAS_B1
		, UCB1
#endif
	}Module;
	struct SPI_CLOCK_SETTINGS
	{
		/// 0: Data changed on first clock edge and captured on following edge
		/// 1: Data captured on first clock edge and changed on following edge
		unsigned ClockPhase						: 1;
		/// 0: Inactive state is low
		/// 1: Inactive state is high
		unsigned ClockPolarity					: 1;
		/// 1: ACLK
		/// 2: SMCLK
		unsigned ClockSource					: 2;
	}ClockSettings;
	struct SPI_DATA_SETTINGS
	{
		/// 0: LSB first
		/// 1: MSB first
		unsigned ShiftDirection					: 1;
		/// 0: 8-bit data mode
		/// 1: 7-bit data mode
		unsigned DataWidth						: 1;
	}DataSettings;
	struct SPI_MODe_SETTINGS
	{
		/// 0: Slave Mode
		/// 1: Master Mode
		unsigned MasterMode						: 1;
		/// 0: 3-pin SPI
		/// 1: 4-pin SPI w/ UCxSTE active high: slave enabled when UCxSTE = 1
		/// 2: 4-pin SPI w/ UCxSTE active low: slave enabled when UCxSTE = 0
		/// 3: I2C mode
		unsigned Mode							: 2;
		/// 0: Loopback diabled
		/// 1: Loopback enabled: transmitter output internally connected to receiver
		unsigned Loopback						: 1;
		/// 0: TX interrupt disabled
		/// 1: TX interrupt enabled
		unsigned TxInterruptEnable				: 1;
		/// 0: RX interrupt disabled
		/// 1: RX interrupt enabled
		unsigned RxInterruptEnable				: 1;
	}ModeSettings;
	uint16_t Prescaler;
	pin_register_t SlaveSelectPin;
	void (*TransmitCallback)(void);
	void (*RecieveCallback)(char* buffer);

	struct SPI_REGISTERS_STRUCT
	{
		struct SPI_CONTROL_REG0 			*control0;
		struct SPI_CONTROL_REG1 			*control1;
		uint8_t 							*bitrate_control0;
		uint8_t 							*bitrate_control1;
		uint8_t 							*rx_buffer;
		uint8_t								*tx_buffer;
		struct SPI_STATUS_REG 				*status;
		struct SPI_INTERRUPT_ENABLE_REG0 	*ie0;
		struct SPI_INTERRUPT_FLAG_REG0 		*if0;
#if defined(SPI_HAS_A1) || defined(SPI_HAS_B1)
		struct SPI_INTERRUPT_ENABLE_REG1 	*ie1;
		struct SPI_INTERRUPT_FLAG_REG1 		*if1;
#endif
	}Registers;
}SpiConfig;


return_status spi_init(SpiConfig* config);


#endif /* SPI_H_ */
