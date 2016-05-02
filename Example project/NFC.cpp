/*
 * \file NFC.cpp
 *
 *  Created on: Apr 27, 2016
 *  \author Remko Welling
 *          remko@rfsee.nl
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *
 *
 */

#include <NFC.h>

#include <string.h>

NFC::NFC()
:
_uid(),
_uidLen(),
_pn532_packetbuffer(),
_versiondata(),
_initialized(false)
{
	_initialized = initializePN532();
}

NFC::~NFC()
{
	// TODO Auto-generated destructor stub
}

bool NFC::initializePN532(void)
{
	// Initialize PN532
	// As the PN532 can be in sleep mode perfom 4 attempts to wake-up and initialize the PN532.
	bool returnValue { true };
	uint8_t retries { 4 } ;

	while (retries != 0)
	{
		returnValue = true;

		// Activate PN532 by unselecting and selecting the chip
		// Wait for 1 Seconds to allow the PN532 to wake-up
		_spiInterface.setSelect(false);
		_spiInterface.setSelect(true);
		_timer.delay(1000);

		// Compose a command packet
		// Set packet buffer byte 0 to some packet command.
		_pn532_packetbuffer[0] = NFC_CONST::PN532_COMMAND_GETFIRMWAREVERSION;
		sendCommandCheckAck(_pn532_packetbuffer, 1, 1);		// send byte using 1 mS timeout.

		// Get firmware version of PN532 and test if any version information is received.
		_versiondata = getFirmwareVersion();
		if (! _versiondata)		// No valid data received communication failed
		{
			returnValue = false;
		}

		// Set the max number of retry attempts to read from a card
		// This prevents us from waiting forever for a card, which is
		// the default behavior of the PN532.
		if (!setPassiveActivationRetries(0xFF))		// Set retries of PN532
		{
			returnValue = false;	// setting failed
		}

		// configure board to read RFID tags
		// set board to read tags, do not use result of setting.
		if (!SAMConfig())
		{
			returnValue = false;
		}

		// Test if all initialization actions are successful.
		if (returnValue)
		{
			retries = 0;	// Initialization success, quit retries by setting 0
		}
		else
		{
			retries--;		// Initialization failed, increment retries
		}
	}
	return returnValue; 	// return state of initialization
}

bool NFC::sendCommandCheckAck(uint8_t *cmd, const uint8_t cmdLength, const uint16_t timeout)
{
	// write the command
	writeCommand(cmd, cmdLength);

	// Wait for chip to say its ready!
	if (!waitReady(timeout))
	{
		return false;
	}

	// read acknowledge
	if (!readAck())
	{
		return false;
	}

	// Wait for chip to say its ready!
	if (!waitReady(timeout))
	{
		return false;
	}

	return true; // ack'd command
}

bool NFC::waitReady(const uint16_t timeout)
{
	// for as long as the timeout is reached try to detect ready signal formPn532
	_timer.setTimeOut(timeout);
	while (!_timer.isTimeOut())
	{
		if (isReady())	// test for ready signal
		{
			return true;	// ready signal detected!
		}
	}
	return false;
}

bool NFC::isReady(void)
{
	// read IRQ signal from PN532
	if (!_spiInterface.isIRQ())
	{
		return false;
	}
	return true;
}

bool NFC::readAck(void)
{
	uint8_t ackbuff[6];		// array to store read message form PN532
	readData(ackbuff, 6);	// read message form PN532
	// compare message with ACK message syntax and return true when ACK is received.
	return (0 == strncmp((char *)ackbuff, (char *)NFC_CONST::pn532ack, 6));
}

void  NFC::readData(uint8_t* buff, uint8_t n)
{
	// select PN532 and wait 1 mS
	_spiInterface.setSelect(true);
	_timer.delay(1);

	// Send message to PN532 to request information
	_spiInterface.sendByte(NFC_CONST::PN532_SPI_DATAREAD);
	// receive data from PN532 and store in buffer.
	for (uint8_t i=0; i<n; i++)
	{
		buff[i] = _spiInterface.getByte();	// put data in buffer at position i
	}

	// Unselect PN532
	_spiInterface.setSelect(false);
}

void NFC::writeCommand(uint8_t* cmd, uint8_t cmdlen)
{
uint8_t checksum;												// variable to store checksum

	cmdlen++;													// increment command length with checksum

	_spiInterface.setSelect(true);								// Select PN532
	_timer.delay(1);	// or whatever the delay is for waking up the board

	_spiInterface.sendByte(NFC_CONST::PN532_SPI_DATAWRITE); 	// inform SPI port of PN532 that data will be written

	checksum = NFC_CONST::PN532_PREAMBLE + NFC_CONST::PN532_PREAMBLE + NFC_CONST::PN532_STARTCODE2;

	_spiInterface.sendByte(NFC_CONST::PN532_PREAMBLE); 			// Send preamble
	_spiInterface.sendByte(NFC_CONST::PN532_PREAMBLE);			// Send start of packet 1/2
	_spiInterface.sendByte(NFC_CONST::PN532_STARTCODE2);		// Send start of packet 2/2
	_spiInterface.sendByte(cmdlen);								// Send packet length (LEN)
	_spiInterface.sendByte(~cmdlen + 1);						// Send packet length checksum
	_spiInterface.sendByte(NFC_CONST::PN532_HOSTTOPN532);		// inform PN532 that data direction is from host to PN532

	checksum += NFC_CONST::PN532_HOSTTOPN532;

	for (uint8_t i=0; i<cmdlen-1; i++)							// Send data to PN532
	{
		_spiInterface.sendByte(cmd[i]);
		checksum += cmd[i];
	}
	_spiInterface.sendByte(~checksum);							// Send checksum (DCS)
	_spiInterface.sendByte(NFC_CONST::PN532_POSTAMBLE);			// Send postamble

	_spiInterface.setSelect(false);								// Unselect PN532
}

uint32_t NFC::getFirmwareVersion(void)
{
	uint32_t response;

	_pn532_packetbuffer[0] = NFC_CONST::PN532_COMMAND_GETFIRMWAREVERSION;		// Set buffer position 0 with command

	if (! sendCommandCheckAck(_pn532_packetbuffer, 1))		// send command with length of 1 byte
	{														// when result is false byte is not send.
		return 0;											// return when fails
	}
	// data was successfully send now retrieve data
	// read data packet
	readData(_pn532_packetbuffer, 12);						// Receive 12 byte data and put in packet buffer provided by pointer.

	// When buffer is not equal firmware version return
	if (0 != strncmp((char *)_pn532_packetbuffer, (char *)NFC_CONST::pn532response_firmwarevers, 6))
	{
		return 0;
	}

	// Shift relevant data from response in to uint32
	// while SPI is used though conditional operator select a shift of 6 positions
	int offset = 6;  // Skip a response byte when using I2C to ignore extra data.
	response = _pn532_packetbuffer[offset++];	// position 6 of pn532_packetbuffer into response
	response <<= 8;								// shift response 8 bits left
	response |= _pn532_packetbuffer[offset++];	// or position 7 of pn532_packetbuffer with response
	response <<= 8;								// shift response 8 bits left
	response |= _pn532_packetbuffer[offset++]; 	// or position 8 of pn532_packetbuffer into response
	response <<= 8;								// shift response 8 bits left
	response |= _pn532_packetbuffer[offset++]; 	// or position 9 of pn532_packetbuffer into response

	return response;
}

bool NFC::setPassiveActivationRetries(uint8_t maxRetries)
{
	// compose message to PN532
	_pn532_packetbuffer[0] = NFC_CONST::PN532_COMMAND_RFCONFIGURATION;
	_pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
	_pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
	_pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
	_pn532_packetbuffer[4] = maxRetries;

	// send configuration message to PN532
	if (! sendCommandCheckAck(_pn532_packetbuffer, 5))
	{
		return false;  // because return value is not successful return false
	}
	return true;
}

bool NFC::readPassiveTargetID(const uint8_t cardbaudrate, uint8_t * uid, uint8_t * uidLength, const uint16_t timeout)
{
	// Compose message
	_pn532_packetbuffer[0] = NFC_CONST::PN532_COMMAND_INLISTPASSIVETARGET;
	_pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
	_pn532_packetbuffer[2] = cardbaudrate;

	// send command, command length and timeout.
	if (!sendCommandCheckAck(_pn532_packetbuffer, 3, timeout))
	{
		return false;  // no cards read
	}

	// wait for a card to enter the field (only possible with I2C)
	if (!waitReady(timeout))
	{
		return false;
	}

	// read data packet
	readData(_pn532_packetbuffer, 20);

	/* ISO14443A card response should be in the following format:

	    byte            Description
	    -------------   ------------------------------------------
	    b0..6           Frame header and preamble
	    b7              Tags Found
	    b8              Tag Number (only one used in this example)
	    b9..10          SENS_RES
	    b11             SEL_RES
	    b12             NFCID Length
	    b13..NFCIDLen   NFCID                                      */

	// Test for the number of tags found. Should be 1!
	if (_pn532_packetbuffer[7] != 1)
	{
		return false;	// no tags found, end of read, return false
	}

	/* Card appears to be Mifare Classic */
	*uidLength = _pn532_packetbuffer[12];	// set value of NFCID length to uidLength

	// Move UID to packet buffer
	for (uint8_t i=0; i < _pn532_packetbuffer[12]; i++)
	{
		uid[i] = _pn532_packetbuffer[13+i];
	}

	return true; // return success as card is read.
}

bool NFC::SAMConfig(void)
{
	// Compose message
	_pn532_packetbuffer[0] = NFC_CONST::PN532_COMMAND_SAMCONFIGURATION;
	_pn532_packetbuffer[1] = 0x01; // normal mode;
	_pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
	_pn532_packetbuffer[3] = 0x01; // use IRQ pin!

	// send message
	if (! sendCommandCheckAck(_pn532_packetbuffer, 4))
	{
		return false;
	}
	// read data packet
	readData(_pn532_packetbuffer, 8);

	int offset = 6;
	// check if data at position 5 is equal 0x15 and return outcome
	return (_pn532_packetbuffer[offset] == 0x15);
}
