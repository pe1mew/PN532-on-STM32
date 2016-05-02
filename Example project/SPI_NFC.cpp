/*
 * \file API_NFC.cpp
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

#include <stdint.h>
#include <string.h>

#include "stm32f4xx_spi.h"

#include <SPI_NFC.h>

SPI_NFC::SPI_NFC()
{
	initSpi4();
}

SPI_NFC::~SPI_NFC()
{
	// TODO Auto-generated destructor stub
}

void SPI_NFC::initSpi4(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	SPI_InitTypeDef SPI_InitTypeDefStruct;

	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Half duplex mode not possible so full duplex
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High; // clock is Mode 0: SCK is active high.
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge; // Data is sampled on the first clock edge of SCK
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //64; // Maximum 5 MHz set to 2,8 MHz
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_LSB; // The data order used is LSB first.

	SPI_Init(SPI4, &SPI_InitTypeDefStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF , ENABLE);

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;
	//                                SCK          MOSI         MISO
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);

	//                         		  SEL
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitTypeDefStruct);

	// 							      IRQ
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitTypeDefStruct);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);

	SPI_Cmd(SPI4, ENABLE);
}

uint8_t SPI_NFC::getByte(void)
{
	while(!SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI4, 0x00);   //Dummy byte to generate clock
	while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_BSY));  	//transmission ready?
	while(!SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE)); //data received?

	return SPI_I2S_ReceiveData(SPI4); //return received data
}

void SPI_NFC::sendByte(uint8_t byte)
{
	while(!SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI4, byte); // send 8 bits of data
	while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_BSY));  	//transmission ready?
	SPI_I2S_ReceiveData(SPI4);      //Clear RXNE bit
}

void SPI_NFC::setSelect(const bool state)
{
	if (state)
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
	}
	else
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_6);
	}
}

bool SPI_NFC::isIRQ(void)
{
	// IRQ is low when data is ready to be received
	if (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8))
	{
		return false;
	}
	return true;
}
