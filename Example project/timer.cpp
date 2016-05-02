/*
 * \file timer.h
 *
 * Created on: Apr 27, 2016
 * \author Remko Welling, 541858
 *          remko@rfsee.nl
 *
 *
 * \version 0.8	Initial release for the HACOX project team for code review
 *
 *
 */

#include "timer.h"
#include "stm32f4xx.h"

extern __IO uint32_t TimmingDelay;

Timer::Timer()
{
	//Enable the GPIOD Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//Systick Configuration
	SysTick_Config(SystemCoreClock/1000); // set mS
}

void Timer::delay(const uint32_t time)
{
	TimmingDelay = time;
	while(TimmingDelay != 0);
}

void Timer::setTimeOut(const uint32_t timeout)
{
	TimmingDelay = timeout;
}

bool Timer::isTimeOut(void)
{
	if (TimmingDelay == 0)
	{
		return true;
	}
	return false;
}

