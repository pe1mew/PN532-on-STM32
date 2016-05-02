/*
 * touchdisplay.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: Remko
 */

#include <touchdisplay.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

//const uint8_t fontSize = {8};

/* Touch Panel configuration */
touchdisplay::touchdisplay()
:
_line(FONT_8_LINE_COUNT),
_char(FONT_8_LINE_SIZE),
_totalLines(FONT_8_LINE_COUNT),
_totalChar(FONT_8_LINE_SIZE),
_displayStatus(true)
{
	LCD_Init();							/* LCD initialization */
	LCD_LayerInit();					/* LCD Layer initialization */
	LTDC_Cmd(ENABLE);					/* Enable the LTDC */
	LCD_SetLayer(LCD_FOREGROUND_LAYER); /* Set LCD foreground layer */
	LCD_Clear(LCD_COLOR_WHITE);			/* Clear the LCD */

	/* Configure the IO Expander */
	if (IOE_Config() == IOE_OK)
	{
		LCD_SetFont(&Font8x8);
	}
	else
	{
		LCD_Clear(LCD_COLOR_RED);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"   IOE NOT OK     ");
		LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"Reset the board   ");
		LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"and try again     ");
	}
	//LCD_DisplayStringLine(LINE(FONT_8_LINE_COUNT), (uint8_t*)"----");
}

touchdisplay::~touchdisplay()
{
	// TODO Auto-generated destructor stub
}

touchdisplay& touchdisplay::Get()
{
	static touchdisplay Instance;
	return Instance;
}

void touchdisplay::PrintfLn(const char* information, ...)
{
	char buffer[FONT_8_LINE_SIZE];
	memset(buffer, ' ', FONT_8_LINE_SIZE);
	va_list Args;
	va_start (Args, information);
	vsnprintf (buffer, FONT_8_LINE_SIZE, information, Args);
	va_end (Args);
	LCD_DisplayStringLine(LINE(newLine()), (uint8_t*)buffer);
	_char = _totalChar; // reset character pointer to initial value
}

void touchdisplay::Printf(const char* information,...)
{
	int i = sizeof(information);

	char buffer[256];
	memset(buffer, ' ', 256);

	char printBuffer[FONT_8_LINE_SIZE];
	memset(printBuffer, '0', FONT_8_LINE_SIZE);

	va_list Args;
	va_start (Args, information);
	vsnprintf (buffer, 256, information, Args);
	va_end (Args);




	LCD_DisplayStringLine(LINE(newLine()), (uint8_t*)buffer);
	_char = _totalChar; // reset character pointer to initial value

}

void touchdisplay::clr(void)
{
	LCD_Clear(LCD_COLOR_WHITE);			/* Clear the LCD */
	_line = _totalLines;
}

uint8_t touchdisplay::newLine(void)
{
	_line++;
	if( _totalLines < _line)
	{
		_line -= (_totalLines + 1);
	}
	return _line;
}

uint8_t touchdisplay::newChar(void)
{
	_char++;
	if( _totalChar < _char)
	{
		_char -= (_totalChar + 1);
	}
	return _char;
}
