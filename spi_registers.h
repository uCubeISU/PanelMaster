/*
 * spi_registers.h
 *
 *  Created on: Dec 10, 2014
 *      Author: vens
 */

#ifndef SPI_REGISTERS_H_
#define SPI_REGISTERS_H_

struct SPI_CONTROL_REG0
{
	unsigned ClockPhase						: 1;
	unsigned ClockPolarity					: 1;
	unsigned MsbFirst						: 1;
	unsigned Data7Bit						: 1;
	unsigned MasterMode						: 1;
	unsigned Mode							: 2;
	unsigned SynchronousMode					: 1;
};

struct SPI_CONTROL_REG1
{
	unsigned ClockSource						: 2;
	unsigned filler							: 5;
	unsigned SoftwareReset					: 1;
};

struct SPI_STATUS_REG
{
	unsigned Loopback						: 1;
	unsigned FramingErrorFlag				: 1;
	unsigned OverrunErrorFlag				: 1;
	unsigned filler							: 4;
	unsigned Busy							: 1;
};

struct SPI_INTERRUPT_ENABLE_REG0
{
	unsigned filler							: 4;
	unsigned B0Tx							: 1;
	unsigned B0Rx							: 1;
	unsigned A0Tx							: 1;
	unsigned A0Rx							: 1;
};

struct SPI_INTERRUPT_FLAG_REG0
{
	unsigned filler							: 4;
	unsigned B0Tx							: 1;
	unsigned B0Rx							: 1;
	unsigned A0Tx							: 1;
	unsigned A0Rx							: 1;
};

struct SPI_INTERRUPT_ENABLE_REG1
{
	unsigned filler							: 4;
	unsigned B1Tx							: 1;
	unsigned B1Rx							: 1;
	unsigned A1Tx							: 1;
	unsigned A1Rx							: 1;
};

struct SPI_INTERRUPT_FLAG_REG1
{
	unsigned filler							: 4;
	unsigned B1Tx							: 1;
	unsigned B1Rx							: 1;
	unsigned A1Tx							: 1;
	unsigned A1Rx							: 1;
};


#endif /* SPI_REGISTERS_H_ */
