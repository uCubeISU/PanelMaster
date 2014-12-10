
#include <msp430.h>

// Serial Layout: MCU1 -> TLC0 -> TLC1 -> MCU2 -> TLC2 -> TLC3 -> TLC4 -> TLC5 -> MCU1
//         Bytes:          36      36              36      36      36      36


#include "spi.h"

/// The contents of this file would go into main probably.
void setup_spi_example()
{
	SpiConfig spi_config;
	spi_config.Module = UCB0;
	spi_config.ClockSettings.ClockPhase = 1;		// Data captured on first edge
	spi_config.ClockSettings.ClockPolarity = 0;		// Idle low
	spi_config.ClockSettings.ClockSource = 2; 		// SMCLK
	spi_config.DataSettings.ShiftDirection = 1;		// MSB first
	spi_config.DataSettings.DataWidth = 0;			// 8-bit data
	spi_config.ModeSettings.MasterMode = 1;			// Master Mode
	spi_config.ModeSettings.Mode = 0;				// 3-pin mode
	spi_config.ModeSettings.Loopback = 0;			// disable loopback
	spi_config.Prescaler = 2;						// prescaller of 2
	PIN_SET_PIN(spi_config.SlaveSelectPin, 3, 0);
	P3OUT;
	spi_init(&spi_config);
}

return_status spi_init(SpiConfig* config)
{
	/// @todo add multiple slave mode somehow (althoug we don't need it right now)
	PIN_SET_HIGH(config->SlaveSelectPin);		// set slave select high
	PIN_SET_OUTPUT(config->SlaveSelectPin);  	// Set slave select to output

	switch (config->Module)
	{
#ifdef SPI_HAS_A0
	case UCA0:
		config->Registers.control0 = (struct SPI_CONTROL_REG0 *)&UCA0CTL0;
		config->Registers.control1 = (struct SPI_CONTROL_REG1 *)&UCA0CTL1;
		config->Registers.bitrate_control0 = (uint8_t *)&UCA0BR0;
		config->Registers.bitrate_control1 = (uint8_t *)&UCA0BR1;
		config->Registers.status = (struct SPI_STATUS_REG *)&UCA0STAT;
		config->Registers.rx_buffer = (uint8_t *)&UCA0RXBUF;
		config->Registers.tx_buffer = (uint8_t *)&UCA0TXBUF;
		break;
#endif
#ifdef SPI_HAS_A1
	case UCA1:
		config->Registers.control0 = (struct SPI_CONTROL_REG0 *)&UCA1CTL0;
		config->Registers.control1 = (struct SPI_CONTROL_REG1 *)&UCA1CTL1;
		config->Registers.bitrate_control0 = (uint8_t *)&UCA1BR0;
		config->Registers.bitrate_control1 = (uint8_t *)&UCA1BR1;
		config->Registers.status = (struct SPI_STATUS_REG *)&UCA1STAT;
		config->Registers.rx_buffer = (uint8_t *)&UCA1RXBUF;
		config->Registers.tx_buffer = (uint8_t *)&UCA1TXBUF;
		break;
#endif
#ifdef SPI_HAS_B0
	case UCB0:
		config->Registers.control0 = (struct SPI_CONTROL_REG0 *)&UCB0CTL0;
		config->Registers.control1 = (struct SPI_CONTROL_REG1 *)&UCB0CTL1;
		config->Registers.bitrate_control0 = (uint8_t *)&UCB0BR0;
		config->Registers.bitrate_control1 = (uint8_t *)&UCB0BR1;
		config->Registers.status = (struct SPI_STATUS_REG *)&UCB0STAT;
		config->Registers.rx_buffer = (uint8_t *)&UCB0RXBUF;
		config->Registers.tx_buffer = (uint8_t *)&UCB0TXBUF;
		break;
#endif
#ifdef SPI_HAS_B1
	case UCB1:
		config->Registers.control0 = (struct SPI_CONTROL_REG0 *)&UCB1CTL0;
		config->Registers.control1 = (struct SPI_CONTROL_REG1 *)&UCB1CTL1;
		config->Registers.bitrate_control0 = (uint8_t *)&UCB1BR0;
		config->Registers.bitrate_control1 = (uint8_t *)&UCB1BR1;
		config->Registers.status = (struct SPI_STATUS_REG *)&UCB1STAT;
		config->Registers.rx_buffer = (uint8_t *)&UCB1RXBUF;
		config->Registers.tx_buffer = (uint8_t *)&UCB1TXBUF;
		break;
#endif
	}
	config->Registers.ie0 = (struct SPI_INTERRUPT_ENABLE_REG0 *)&IE2;
	config->Registers.if0 = (struct SPI_INTERRUPT_FLAG_REG0 *)&IFG2;
#if defined(SPI_HAS_A1) || defined(SPI_HAS_B1)
	config->Registers.ie1 = (struct SPI_INTERRUPT_ENABLE_REG1 *)&UC1IE;
	config->Registers.if1 = (struct SPI_INTERRUPT_FLAG_REG0 *)&UC1IFG;
#endif
	config->Registers.control1->SoftwareReset = 1;								// Put the serial module into reset mode
	config->Registers.control0->ClockPhase = config->ClockSettings.ClockPhase;
	config->Registers.control0->ClockPolarity = config->ClockSettings.ClockPolarity;
	config->Registers.control0->Data7Bit = config->DataSettings.DataWidth;
	config->Registers.control0->MasterMode = config->ModeSettings.MasterMode;
	config->Registers.control0->Mode = config->ModeSettings.Mode;
	config->Registers.control0->SynchronousMode = 1;							// SPI mode
	config->Registers.control1->ClockSource = config->ClockSettings.ClockSource;
	config->Registers.bitrate_control0 = config->Prescaler;
	config->Registers.bitrate_control1 = config->Prescaler >> 8;
	config->Registers.status->Loopback = config->ModeSettings.Loopback;

	// needs to be made more general:
	P1SEL |= BIT5 | BIT6 | BIT7;					// set the MISO, MOSI, and CLK pins to the correct mode
	P1SEL2 |= BIT5 | BIT6 | BIT7;

	config->Registers.control1->SoftwareReset = 0;								// Start the state machine

	if(config->ModeSettings.TxInterruptEnable)
	{
		switch (config->Module)
		{
#ifdef SPI_HAS_A0
		case UCA0:
			config->Registers.if0->A0Tx = 0;
			config->Registers.ie0->A0Tx = 1;
			break;
#endif
#ifdef SPI_HAS_A1
		case UCA1:
			config->Registers.if1->A1Tx = 0;
			config->Registers.ie1->A1Tx = 1;
			break;
#endif
#ifdef SPI_HAS_B0
		case UCB0:
			config->Registers.if0->B0Tx = 0;
			config->Registers.ie0->B0Tx = 1;
			break;
#endif
#ifdef SPI_HAS_B1
		case UCB1:
			config->Registers.if1->B1Tx = 0;
			config->Registers.ie1->B1Tx = 1;
			break;
#endif
		}
	}

	if(config->ModeSettings.RxInterruptEnable)
	{
		switch (config->Module)
		{
#ifdef SPI_HAS_A0
		case UCA0:
			config->Registers.if0->A0Rx = 0;
			config->Registers.ie0->A0Rx = 1;
			break;
#endif
#ifdef SPI_HAS_A1
		case UCA1:
			config->Registers.if1->A1Rx = 0;
			config->Registers.ie1->A1Rx = 1;
			break;
#endif
#ifdef SPI_HAS_B0
		case UCB0:
			config->Registers.if0->B0Rx = 0;
			config->Registers.ie0->B0Rx = 1;
			break;
#endif
#ifdef SPI_HAS_B1
		case UCB1:
			config->Registers.if1->B1Rx = 0;
			config->Registers.ie1->B1Rx = 1;
			break;
#endif
		}
	}

	return EXIT_SUCCESS;
}

