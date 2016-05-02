/**
 **
 **	 http://electronics-homemade.com/STM32F4-LED-Toggle-Systick.html
 **
 **	 interrupt handler function inside stm32f4xx_it.c shall be modified:
 **
 **	 \verbatim

	__IO uint32_t TimmingDelay;
	void SysTick_Handler(void)
	{
	  if(TimmingDelay !=0)
	  {
		TimmingDelay --;
	   }
	}

 **	 \endverbatim
 **
 **
 *****************************************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"

#include "NFC_test.h"
#include "timer.h"

/* Private macro */

#define UNITTEST

/* Private variables */

//extern __IO uint32_t TimmingDelay;

#ifdef UNITTEST
NFC_test rfidUnitTest;

int counter = 0;

bool buttonMemory = false;
bool button = false;

#endif

/* Private function prototypes */

/* Private functions */

int main(void)
{
	int i = 0;

	// GPIOD Configuration
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// GPIOD LEDs ON
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);

#ifndef UNITTEST

	Timer timer;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;

	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitTypeDefStruct);


#endif

	/* Infinite loop */
	while (1)
	{
		i++;

		/// Normal code for execution
#ifndef UNITTEST

		// insert a delay of 500ms
		timer.delay(1000);

		//Toggle the LED
		STM_EVAL_LEDToggle(LED3);

#endif
		/// Unit test code for execution
#ifdef UNITTEST
		/// Toggle LED3
		if (i == 100000)
		{
			STM_EVAL_LEDToggle(LED3);
			//rfidUnitTest.Cycle();
			i = 0;
		}

		if (STM_EVAL_PBGetState(BUTTON_USER))
		{
			button = true;
		}
		else
		{
			button = false;
		}

		// When BUTTON_USER pressed
		if (button && (button != buttonMemory))
		{
			STM_EVAL_LEDOn(LED4);
			rfidUnitTest.Cycle();

			counter++;
			if (counter == 10)
			{
				counter = 0;
			}
		}
		else
		{
			STM_EVAL_LEDOff(LED4);
		}
		buttonMemory = button;

#endif

	}
}

/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
extern "C" uint32_t sEE_TIMEOUT_UserCallback(void)
{
	/* TODO, implement your code here */
	while (1)
	{
	}
	return 0;
}
