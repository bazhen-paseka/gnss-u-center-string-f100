/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	char DataChar[0xff];
	sprintf(DataChar,"\r\n\r\n\tDIEGO - dot clock for bedroom");
	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	#define DATE_as_int_str 	(__DATE__)
	#define TIME_as_int_str 	(__TIME__)
	sprintf(DataChar,"\r\n\tBuild: %s. Time: %s." , DATE_as_int_str , TIME_as_int_str ) ;
	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	sprintf(DataChar,"\r\n\tFor debug: UART1-115200/8-N-1\r\n" ) ;
	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	RTC_TimeTypeDef TimeSt = { 0 } ;
	TimeSt.Hours 	= 13 ;
	TimeSt.Minutes 	= 35 ;
	TimeSt.Seconds 	=  0 ;
	HAL_RTC_SetTime(&hrtc, &TimeSt, RTC_FORMAT_BIN);
	uint32_t x_u32 = 75000;
	uint32_t y_u32 = 53000;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	HAL_GPIO_TogglePin(LD4_GPIO_Port,LD4_Pin);

//	sprintf(DataChar,"$GNGGA,093442.00,5029.75956,N,03046.53359,E,1,12,0.64,112.2,M,25.3,M,,*41\r\n" ) ;
//	HAL_UART_Transmit( &huart3, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	//uint8_t gnss_lenght_u8 = strlen(DataChar);
	uint8_t gnss_lenght_u8 = 75;
//	sprintf(DataChar, "GNSS length: %d\r\n", gnss_lenght_u8);
//	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

//	#define CS_SIZE	4
//	char cs_glue_string[CS_SIZE];
	//	cs glue:
//	memset( cs_glue_string, 0, CS_SIZE );
//	memcpy( cs_glue_string, &DataChar[gnss_lenght_u8 - 4], 2 );
//	uint8_t cs_glue_u8 = strtol(cs_glue_string, NULL, 16 );
//
	uint8_t cs_calc_u8 = 0;
//	cs_calc_u8 = DataChar[1];
//	for (int i=2; i<(gnss_lenght_u8 - 5); i++) {
//		cs_calc_u8 ^= DataChar[i];
//	}
//
//	if (cs_calc_u8 == cs_glue_u8) {
//		sprintf(DataChar, "chSum Ok\r\n");
//		HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;
//	}


	x_u32 = x_u32 + 10;
	y_u32 = y_u32 + 10;
	HAL_RTC_GetTime(&hrtc, &TimeSt, RTC_FORMAT_BIN);
	sprintf(DataChar,"RTC Time: %02d:%02d:%02d\r\n",TimeSt.Hours, TimeSt.Minutes, TimeSt.Seconds );
	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	sprintf(DataChar,"$GNGGA,%02d%02d%02d.00,5029.%05lu,N,03046.%05lu,E,1,12,0.64,112.2,M,25.3,M,,*41\r\n",
			TimeSt.Hours, TimeSt.Minutes, TimeSt.Seconds, x_u32, y_u32 ) ;
	//HAL_UART_Transmit( &huart3, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	cs_calc_u8 = DataChar[1];
	for (int i=2; i<(gnss_lenght_u8 - 5); i++) {
		cs_calc_u8 ^= DataChar[i];
	}

//	sprintf(DataChar, "New chSum %d\r\n", cs_calc_u8 );
//	HAL_UART_Transmit( &huart1, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

	sprintf(DataChar,"$GNGGA,%02d%02d%02d.00,5029.%05lu,N,03046.%05lu,E,1,12,0.64,112.2,M,25.3,M,,*%02x\r\n",
			TimeSt.Hours, TimeSt.Minutes, TimeSt.Seconds, x_u32, y_u32, cs_calc_u8) ;
	HAL_UART_Transmit( &huart3, (uint8_t *)DataChar , strlen(DataChar) , 100 ) ;

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
