/*
 * \file NFC.h
 *
 *  Created on: Apr 27, 2016
 *  \author Remko Welling, 541858
 *          remko@rfsee.nl
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *
 *
 */

#ifndef NFC_H_
#define NFC_H_

#include "SPI_NFC.h"
#include "timer.h"

#define PN532_PACKBUFFSIZ 64


namespace NFC_CONST
{
const uint8_t pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
const uint8_t pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

const uint8_t PN532_PREAMBLE                      = 0x00;
const uint8_t PN532_STARTCODE1                    = 0x00;
const uint8_t PN532_STARTCODE2                    = 0xFF;
const uint8_t PN532_POSTAMBLE                     = 0x00;

const uint8_t PN532_HOSTTOPN532                   = 0xD4;
const uint8_t PN532_PN532TOHOST                   = 0xD5;

// PN532 Commands
const uint8_t PN532_COMMAND_DIAGNOSE              = 0x00;
const uint8_t PN532_COMMAND_GETFIRMWAREVERSION    = 0x02;
const uint8_t PN532_COMMAND_GETGENERALSTATUS      = 0x04;
const uint8_t PN532_COMMAND_READREGISTER          = 0x06;
const uint8_t PN532_COMMAND_WRITEREGISTER         = 0x08;
const uint8_t PN532_COMMAND_READGPIO              = 0x0C;
const uint8_t PN532_COMMAND_WRITEGPIO             = 0x0E;
const uint8_t PN532_COMMAND_SETSERIALBAUDRATE     = 0x10;
const uint8_t PN532_COMMAND_SETPARAMETERS         = 0x12;
const uint8_t PN532_COMMAND_SAMCONFIGURATION      = 0x14;
const uint8_t PN532_COMMAND_POWERDOWN             = 0x16;
const uint8_t PN532_COMMAND_RFCONFIGURATION       = 0x32;
const uint8_t PN532_COMMAND_RFREGULATIONTEST      = 0x58;
const uint8_t PN532_COMMAND_INJUMPFORDEP          = 0x56;
const uint8_t PN532_COMMAND_INJUMPFORPSL          = 0x46;
const uint8_t PN532_COMMAND_INLISTPASSIVETARGET   = 0x4A;
const uint8_t PN532_COMMAND_INATR                 = 0x50;
const uint8_t PN532_COMMAND_INPSL                 = 0x4E;
const uint8_t PN532_COMMAND_INDATAEXCHANGE        = 0x40;
const uint8_t PN532_COMMAND_INCOMMUNICATETHRU     = 0x42;
const uint8_t PN532_COMMAND_INDESELECT            = 0x44;
const uint8_t PN532_COMMAND_INRELEASE             = 0x52;
const uint8_t PN532_COMMAND_INSELECT              = 0x54;
const uint8_t PN532_COMMAND_INAUTOPOLL            = 0x60;
const uint8_t PN532_COMMAND_TGINITASTARGET        = 0x8C;
const uint8_t PN532_COMMAND_TGSETGENERALBYTES     = 0x92;
const uint8_t PN532_COMMAND_TGGETDATA             = 0x86;
const uint8_t PN532_COMMAND_TGSETDATA             = 0x8E;
const uint8_t PN532_COMMAND_TGSETMETADATA         = 0x94;
const uint8_t PN532_COMMAND_TGGETINITIATORCOMMAND = 0x88;
const uint8_t PN532_COMMAND_TGRESPONSETOINITIATOR = 0x90;
const uint8_t PN532_COMMAND_TGGETTARGETSTATUS     = 0x8A;

const uint8_t PN532_RESPONSE_INDATAEXCHANGE       = 0x41;
const uint8_t PN532_RESPONSE_INLISTPASSIVETARGET  = 0x4B;

const uint8_t PN532_WAKEUP                        = 0x55;

const uint8_t PN532_SPI_STATREAD                  = 0x02;
const uint8_t PN532_SPI_DATAWRITE                 = 0x01;
const uint8_t PN532_SPI_DATAREAD                  = 0x03;
const uint8_t PN532_SPI_READY                     = 0x01;

const uint8_t PN532_I2C_ADDRESS                   = 0x48 >> 1;
const uint8_t PN532_I2C_READBIT                   = 0x01;
const uint8_t PN532_I2C_BUSY                      = 0x00;
const uint8_t PN532_I2C_READY                     = 0x01;
const uint8_t PN532_I2C_READYTIMEOUT              = 20;

const uint8_t PN532_MIFARE_ISO14443A              = 0x00;

const uint8_t PN532_GPIO_VALIDATIONBIT            = 0x80;
const uint8_t PN532_GPIO_P30                      = 0;
const uint8_t PN532_GPIO_P31                      = 1;
const uint8_t PN532_GPIO_P32                      = 2;
const uint8_t PN532_GPIO_P33                      = 3;
const uint8_t PN532_GPIO_P34                      = 4;
const uint8_t PN532_GPIO_P35                      = 5;

// Mifare Commands
const uint8_t MIFARE_CMD_AUTH_A                   = 0x60;
const uint8_t MIFARE_CMD_AUTH_B                   = 0x61;
const uint8_t MIFARE_CMD_READ                     = 0x30;
const uint8_t MIFARE_CMD_WRITE                    = 0xA0;
const uint8_t MIFARE_CMD_TRANSFER                 = 0xB0;
const uint8_t MIFARE_CMD_DECREMENT                = 0xC0;
const uint8_t MIFARE_CMD_INCREMENT                = 0xC1;
const uint8_t MIFARE_CMD_STORE                    = 0xC2;
const uint8_t MIFARE_ULTRALIGHT_CMD_WRITE         = 0xA2;

// Prefixes for NDEF Records = to identify record type;
const uint8_t NDEF_URIPREFIX_NONE                 = 0x00;
const uint8_t NDEF_URIPREFIX_HTTP_WWWDOT          = 0x01;
const uint8_t NDEF_URIPREFIX_HTTPS_WWWDOT         = 0x02;
const uint8_t NDEF_URIPREFIX_HTTP                 = 0x03;
const uint8_t NDEF_URIPREFIX_HTTPS                = 0x04;
const uint8_t NDEF_URIPREFIX_TEL                  = 0x05;
const uint8_t NDEF_URIPREFIX_MAILTO               = 0x06;
const uint8_t NDEF_URIPREFIX_FTP_ANONAT           = 0x07;
const uint8_t NDEF_URIPREFIX_FTP_FTPDOT           = 0x08;
const uint8_t NDEF_URIPREFIX_FTPS                 = 0x09;
const uint8_t NDEF_URIPREFIX_SFTP                 = 0x0A;
const uint8_t NDEF_URIPREFIX_SMB                  = 0x0B;
const uint8_t NDEF_URIPREFIX_NFS                  = 0x0C;
const uint8_t NDEF_URIPREFIX_FTP                  = 0x0D;
const uint8_t NDEF_URIPREFIX_DAV                  = 0x0E;
const uint8_t NDEF_URIPREFIX_NEWS                 = 0x0F;
const uint8_t NDEF_URIPREFIX_TELNET               = 0x10;
const uint8_t NDEF_URIPREFIX_IMAP                 = 0x11;
const uint8_t NDEF_URIPREFIX_RTSP                 = 0x12;
const uint8_t NDEF_URIPREFIX_URN                  = 0x13;
const uint8_t NDEF_URIPREFIX_POP                  = 0x14;
const uint8_t NDEF_URIPREFIX_SIP                  = 0x15;
const uint8_t NDEF_URIPREFIX_SIPS                 = 0x16;
const uint8_t NDEF_URIPREFIX_TFTP                 = 0x17;
const uint8_t NDEF_URIPREFIX_BTSPP                = 0x18;
const uint8_t NDEF_URIPREFIX_BTL2CAP              = 0x19;
const uint8_t NDEF_URIPREFIX_BTGOEP               = 0x1A;
const uint8_t NDEF_URIPREFIX_TCPOBEX              = 0x1B;
const uint8_t NDEF_URIPREFIX_IRDAOBEX             = 0x1C;
const uint8_t NDEF_URIPREFIX_FILE                 = 0x1D;
const uint8_t NDEF_URIPREFIX_URN_EPC_ID           = 0x1E;
const uint8_t NDEF_URIPREFIX_URN_EPC_TAG          = 0x1F;
const uint8_t NDEF_URIPREFIX_URN_EPC_PAT          = 0x20;
const uint8_t NDEF_URIPREFIX_URN_EPC_RAW          = 0x21;
const uint8_t NDEF_URIPREFIX_URN_EPC              = 0x22;
const uint8_t NDEF_URIPREFIX_URN_NFC              = 0x23;
}

/// \class NFC NFC.h <NFC.h>
/// \brief Class for all activites using a PN532 NFC reader.
///
/// Supported is:
///	- PN532
///
/// The class communicates with a object of the class type SPI_NFC that will interact with the API interface.
class NFC
{
public:
	NFC();
	virtual ~NFC();

	/// \brief Initialize PN532
	/// \return true if everything executed properly, false for an error
	bool initializePN532(void);

	/// \brief return initialization state
	/// \return true if initialized properly, false for an error
	bool isInitialized(void) { return _initialized;};

	/// \brief Return hard- and software version from PN532
	/// The hard- and software version from PN532 is stored in a 32 bit variable.
	/// MSB byte has no relevant information
	/// Byte 2: chip type: eg. PN5[32] in HEX
	/// Byte 3: version number before decimal point eg. [1].6
	/// Byte 4: version number after decimal point e.g. 1.[6]
	/// \return variable containing hard- and software version from PN532
	uint32_t getVersionData(void) { return _versiondata;};

	/// \brief Read ISO14443A card
	/// Read UID of an ISO14443A card
	/// \param cardbaudrate Card baud rate
	/// \param uid pointer to character array to store UID
	/// \param uidLength pointer to variable to hold UID length (4 or 7)
	/// \param timeout timeout in mS default is 40 mS to allow PN532 to receive answer form card.
	/// \return state of read action. true = success, false is failed.
	bool readPassiveTargetID(const uint8_t cardbaudrate, uint8_t * uid, uint8_t * uidLength, const uint16_t timeout  = 40);

private:
	SPI_NFC		_spiInterface; 		///< SPI interface object to send LED data to.
	Timer		_timer;				///< timer object for delay and timeout.

	uint8_t 	_uid[7];       		///< array to store ISO14443A uid
	uint8_t 	_uidLen;       		///< uid len
	uint8_t 	_pn532_packetbuffer[PN532_PACKBUFFSIZ]; ///< buffer to compose command.
	uint32_t 	_versiondata;		///< version data see getVersionData function for description.
	bool		_initialized;		///< state of PN532

	/// \brief Retrieve firmware version form PN532
	/// return Firmware version of PN532 see getVersionData function for description.
	uint32_t getFirmwareVersion(void);

	/// \brief Set retries of PN532
	/// Sets the MxRtyPassiveActivation byte of the RFConfiguration register
	/// \return maxRetries true if everything executed properly, false for an error
	bool setPassiveActivationRetries(uint8_t maxRetries);

	/// \brief Configures the SAM (Secure Access Module)
	/// \return true if everything executed properly, false for an error
	bool SAMConfig(void);

	/// \brief send command and wait for acknowledge form PN532
	/// \param cmd pointer to variable tha holds composed command.
	/// \param cmdlen Length of command (number of bytes)
	/// \param timeout Timeout in mS before acknowledge is expected. Default is 1 mS
	/// \return state of command. true = accepted by PN532
	bool sendCommandCheckAck(uint8_t *cmd, const uint8_t cmdlen, const uint16_t timeout = 1);

	/// \brief Write command to PN532 using SPI
	/// \param cmd pointer to character array that contains complete command packet
	/// \param cmdlen length of command packet
	void writeCommand(uint8_t* cmd, uint8_t cmdlen);

	/// \brief Waits until the PN532 is ready
	/// \param timeout Timeout before giving up
	bool waitReady(const uint16_t timeout);

	/// \brief wait for PN532 until ready
	/// \return true if the PN532 is ready with a response.
	bool isReady(void);

	/// \brief read acknowledge form PN532
	/// \return true when acknowledge is received.
	bool readAck(void);

	/// \brief Reads n bytes of data from the PN532
	/// \param  buff Pointer to the buffer where data will be written
	/// \param  n Number of bytes to be read
	void readData(uint8_t* buff, uint8_t n);
};

#endif /* NFC_H_ */
