/*
 * touchdisplay.h
 *
 *  Created on: Apr 13, 2016
 *      Author: Remko
 */


// LCD_PIXEL_WIDTH 240
// LCD_PIXEL_HEIGHT 320


#ifndef TOUCHDISPLAY_H_
#define TOUCHDISPLAY_H_

#define FONT_SIZE 8
#define FONT_8_LINE_SIZE 31
#define FONT_8_LINE_COUNT 39

#include <stdint.h>

class touchdisplay
{
public:
	touchdisplay();
	virtual ~touchdisplay();
	static touchdisplay& Get();
	void PrintfLn(const char* information,...);
	void Printf(const char* information,...);

	void clr(void);

private:

	uint8_t	_line;
	uint8_t _char;
	const uint8_t _totalLines;
	const uint8_t _totalChar;

	bool _displayStatus;

	uint8_t newLine(void);
	uint8_t newChar(void);
};

#endif /* TOUCHDISPLAY_H_ */
