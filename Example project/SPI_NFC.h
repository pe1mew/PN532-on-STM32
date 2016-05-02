/*
 * \file SPI_NFC.h
 *
 *  Created on: Apr 27, 2016
 *  \author Remko Welling, 541858
 *          remko@rfsee.nl
 *
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *	                This version is using the following hardware configuration:
 *	                SPI interface: SPI4
 *	                SCK: 	GPIOE, GPIO_Pin_2
 *	                MOSI: 	GPIOE, GPIO_Pin_6
 *	                MISO: 	GPIOE, GPIO_Pin_5
 *	                SEL/CE: GPIOF, GPIO_Pin_6
 *	                IRQ: 	GPIOF, GPIO_Pin_8
 *
 *
 *
 */

#ifndef SPI_NFC_H_
#define SPI_NFC_H_

#include <stdint.h>

/// \class SPI_NFC SPI_NFC.h <SPI_NFC.h>
/// \brief Class for all activities using a PN532 NFC reader.
///
///
/// \todo comment
class SPI_NFC
{
public:
	SPI_NFC();
	virtual ~SPI_NFC();

	/// \brief Initialize SPI interface
	void initSpi4(void);

	/// \brief Send single byte
	/// \param byte byte to be sent over SPI
	void sendByte(uint8_t byte);

	/// \brief receive single byte
	/// \return received byte.
	uint8_t getByte(void);

	/// \brief set SPI Chip Enable signal
	/// Setting the CES line will select the PN532. true is select, false is not selected.
	/// \param state state of select signal
	void setSelect(const bool state);

	/// \brief test PN532 IRQ signal
	/// The PN532 will toggle the IRQ to active when data is ready to be received form PN532
	/// \param true if QRQ signal is active, false if not active
	bool isIRQ(void);
};

#endif /* SPI_NFC_H_ */
