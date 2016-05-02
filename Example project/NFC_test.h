/*
 * \file NFC_TEST.h
 *
 *  Created on: Apr 27, 2016
 *  \author Remko Welling, 541858
 *          remko@rfsee.nl
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *
 *
 *
 *
 */

#ifndef NFC_TEST_H_
#define NFC_TEST_H_

#include "NFC.h"
#include "touchdisplay.h"

/// \class NFC_test NFC_test.h <NFC_test.h>
/// \brief Class for testing PN532 NFC reader.
///
///
class NFC_test
{
public:
	NFC_test();
	virtual ~NFC_test();

	/// \brief step function for unit testing
	/// function shall be called to step to next test.
	void Step(void);

	/// \brief cycle function for unit testing
	/// Function shall be called periodically to do activities
	void Cycle(void);

private:
	NFC 			rfid_test;		///< NFC reader object.
	touchdisplay 	display;		///< Display object

	bool _success;					///< variable to store activity result
	uint8_t _uid[7];				///< Buffer to store the returned UID
	uint8_t _uidLength;				///< Length of the UID (4 or 7 bytes depending on ISO14443A card type)

	/// \brief print PN532 version data on display
	void printVersionData(void);

	/// \brief read card and print UID
	void readCard(void);
};

#endif /* NFC_TEST_H_ */
