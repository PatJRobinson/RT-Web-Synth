/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// including httpd.h [- HTTPd #1 -]
#include "lwip/apps/httpd.h"

// we include String.h for the strcmp() function [= CGI #1 =]
#include <String.h>

// we include this library to be able to use boolean variables for SSI
#include <Stdbool.h>

bool LD1ON = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD2ON = false; // this variable will indicate if our LD2 LED on the board is ON or not

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

bool onOff[15];

// just declaring the function for the compiler [= CGI #2 =]
const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[],
char *pcValue[]);

// in our SHTML file <form method="get" action="/leds.cgi"> [= CGI #3 =]
const tCGI LedCGI = { "/leds.cgi", LedCGIhandler };

// [= CGI #4 =]
tCGI theCGItable[1];

// just declaring SSI handler function [* SSI #1 *]
u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen);


// [= SSI #2 =]
#define numSSItags 15

// [= SSI #1 =]
const char *theSSItags[numSSItags] = { "tag1", "tag2", "tag3", "tag4", "tag5", "tag6", "tag7", "tag8","tag9", "tag10", "tag11", "tag12", "tag13", "tag14" ,"tag15" };

// strings to be returned by SSI handler, depending on whether variable is "checked" or "unchecked"
char * myStr[2][numSSItags] = {{
		"<input value = \"1\" name=\"led\" class = \"wshape\" type = \"checkbox\" onclick = \"handleClick(this)\">",
		"<input value = \"2\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"3\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"4\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"5\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"6\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"7\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"8\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"9\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"10\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"11\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"12\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"13\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"14\" name=\"led\" class = \"audioFX\" onclick = \"handleClick(this)\" type = \"checkbox\">",
		"<input value = \"15\" name=\"led\" class = \"audioFX\" onclick = \"handleClick(this)\" type = \"checkbox\">"},

		{"<input value = \"1\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"2\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"3\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"4\" name=\"led\" class = \"wshape\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"5\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"6\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"7\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"8\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"9\" name=\"led\" class = \"octave\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"10\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"11\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"12\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"13\" name=\"led\" class = \"filt\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"14\" name=\"led\" class = \"audioFX\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>",
		"<input value = \"15\" name=\"led\" class = \"audioFX\" onclick = \"handleClick(this)\" type = \"checkbox\"checked>"

}};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// the actual function for handling CGI [= CGI #5 =]
const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[],
char *pcValue[]) {

	uint32_t i = 0;
	const int numChannels = 3;
	uint8_t ctrData[numChannels];

	if (iIndex == 0) {

		//DEBUG -- turning the LED lights off
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

		// set perams to "unchecked"
		for (int i = 0; i < numSSItags; i++)
		{
			onOff[i] = false;
		}
	 }
	for (i = 0; i < iNumParams; i++) {

		// cycle through perameters
		for (int x = 0; x < 15; x++) {

			char num;
			sprintf(num, "%d", x + 1);

			// find "checked" perameters
			if (strcmp(pcValue[i], num) == 0) {

				//DEBUG
				if (x == 0)
				{
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				} else if (x == 1)
				{
					HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				} else if (x == 2)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
				}

				onOff[x] = true;


				// select appropriate txData
				if (x < 4)		// wshape
				{
					ctrData[0] = x;
					uint8_t txData[3] = {0x90, 0, ctrData[0]};
					HAL_UART_Transmit(&huart5, txData, 3, 10);


				} else if (x < 9)		//octave
				{
					ctrData[1] = x - 4;
					uint8_t txData[3] = {0x90, 0, ctrData[0]};
					HAL_UART_Transmit(&huart5, txData, 3, 10);


				} else if (x < 13)		//filter
				{
					ctrData[2] = x - 9;
					uint8_t txData[3] = {0x90, 0, ctrData[0]};
					HAL_UART_Transmit(&huart5, txData, 3, 10);


				}
				else if (x < 15)		//delayTog
				{
					ctrData[3] = x - 13;
					uint8_t txData[3] = {0x90, 0, ctrData[0]};
					HAL_UART_Transmit(&huart5, txData, 3, 10);
				}
			}
		}
	}

	// the extension .shtml for SSI to work
	return "/index.shtml";

} // END [= CGI #5 =]

                    // function to initialize CGI [= CGI #6 =]
void myCGIinit(void) {
//add LED control CGI to the table
theCGItable[0] = LedCGI;
//give the table to the HTTP server
http_set_cgi_handlers(theCGItable, 1);
} // END [= CGI #6 =]

// the actual function for SSI [* SSI #4 *]
u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen) {

	char * s;

	if (onOff[iIndex] == false)
	{
		s = myStr[0][iIndex];

	} else if (onOff[iIndex] == true)

	{
		s = myStr[1][iIndex];
	}

	strcpy(pcInsert, s);

	return strlen(s);

}

// function to initialize SSI [* SSI #5 *]
void mySSIinit(void) {

	http_set_ssi_handler(mySSIHandler, (char const**) theSSItags,
	numSSItags);
 }

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_LWIP_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

	// initializing the HTTPd [-HTTPd #2-]
	httpd_init();

	// initializing CGI  [= CGI #7 =]
	myCGIinit();

	// initializing SSI [* SSI #6 *]
	mySSIinit();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
                        while (1) {

                            // starting the LWIP process [-HTTPd #3-]
                            MX_LWIP_Process();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
                        }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 31250;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD1_Pin */
  GPIO_InitStruct.Pin = LD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
