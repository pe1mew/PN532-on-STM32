/*
 * \file NFC_test.cpp
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

#include <NFC_test.h>

NFC_test::NFC_test()
:
_success(false),
_uid(),
_uidLength(0)
{
	// Print messages to display
	display.PrintfLn("RF-ID Unit-tests.");
	display.PrintfLn("=================");

	// get initialization state from PN532 to test if initialization is success
	if (rfid_test.isInitialized())
	{
		printVersionData(); 	// print initialization data
	}
	else
	{
		display.PrintfLn("Initialization failed!");	// announce initialization failure
	}

}

NFC_test::~NFC_test()
{
	// TODO Auto-generated destructor stub
}

void NFC_test::Step(void)
{

}

void NFC_test::Cycle(void)
{
	readCard();
}

void NFC_test::printVersionData(void)
{
	uint32_t versiondata = rfid_test.getVersionData();
	if (versiondata != 0)
	{
		/*
			 bit: 76543210765432107654321076543210
			      [      ]                         << not relevant
			              [      ]                 << chip type: eg. PN5[32] right shift 16 bits
			                      [      ]         << version number before decimal point eg. [1].6 right shift 8
			                              [      ] << version number after decimal point e.g. 1.[6]
		 */
		display.PrintfLn("Found chip PN5%x", (versiondata>>16) & 0xFF);
		display.PrintfLn("Firmware ver. %i.%i", (versiondata>>8) & 0xFF, (versiondata) & 0xFF);
		display.PrintfLn("");
		display.PrintfLn("Waiting for an ISO14443A card");
		display.PrintfLn("-----------------------------");
	}
	else
	{
		display.PrintfLn("Chip not found.");
	}
}

void NFC_test::readCard(void)
{
	// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
	// 'uid' will be populated with the UID, and uidLength will indicate
	// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	_success = rfid_test.readPassiveTargetID(NFC_CONST::PN532_MIFARE_ISO14443A, &_uid[0], &_uidLength);	// request reading of card. provide addresses of variables (no time-out)

	if (_success)
	{
		// Print messages to display
		display.PrintfLn("CARD! %i bytes UID:", _uidLength);
		if ( _uidLength == 4)
		{
			display.PrintfLn("%02x:%02x:%02x:%02x ", _uid[0], _uid[1], _uid[2], _uid[3] );
			display.PrintfLn("------------");
		}
		else
		{
			display.PrintfLn("%02x:%02x:%02x:%02x:%02x:%02x:%02x ", _uid[0], _uid[1], _uid[2], _uid[3], _uid[4], _uid[5], _uid[6] );
			display.PrintfLn("---------------------");
		}
	}
	else
	{
		// PN532 probably timed out waiting for a card
		display.PrintfLn("Timed out waiting for card");
		display.PrintfLn("--------------------------");
	}
}
