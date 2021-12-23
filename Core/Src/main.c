#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

#include "ssd1306.h"
#include "stdio.h"
#include "string.h"

#include "usart1_buffer_interface.h"
#include "usart2_buffer_interface.h"
#include "gps_interface.h"

void SystemClock_Config(void);



char usart1_buffer[2][USART1_BUFFER_LENGTH];
int usart1_write_buffer = 0;
int usart1_read_buffer = 1;
int usart1_write_index = 0;
int usart1_new_message_ready_flag = 0;
int usart1_message_length = 0;
int usart1_old_message_saved = 1;
int usart1_message_lost = 0;
int usart1_received_messages = 0;
int usart1_processed_messages = 0;

char usart2_buffer[2][USART2_BUFFER_LENGTH];
int usart2_write_buffer = 0;
int usart2_read_buffer = 1;
int usart2_write_index = 0;
int usart2_new_message_ready_flag = 0;
int usart2_message_length = 0;
int usart2_old_message_saved = 1;
int usart2_message_lost = 0;
int usart2_received_messages = 0;
int usart2_processed_messages = 0;

char gps_coordinates[64];

int gsm_condition = 0;
// 0 - ozhidanie creg 5
// 1 - ozhidanie eha AT+CMGF=1;&W
// 2 - ozhidanie eha AT+CMGS="+79219666439"
// 3 - ozhidanie eha > test sms
// 4 - ozhidanie +CMGS: 2

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();


	ssd1306_set_i2c_port(&hi2c1, 1);
	ssd1306_Init();
	HAL_Delay(100);

	ssd1306_Fill(White);
	ssd1306_UpdateScreen();
	HAL_Delay(100);
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();
	HAL_Delay(100);

	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("TRACKER", Font_16x26, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);

	gps_object_init();

	//uint32_t counter = 0;
	//char message[64];

	ssd1306_Fill(Black);
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("GPS-GLONASS", Font_11x18, White);
	ssd1306_SetCursor(0,0);
	ssd1306_UpdateScreen();

	int odd_even = 0;
	UNUSED(odd_even);

	// DEEP DEBUG ***********************************
	//***********************************************
	/*
	while (1)
	{
		if(HAL_GetTick()%1000 == 0)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *)"Hello\r\n", 7, 500);
			HAL_GPIO_TogglePin(onboard_led_GPIO_Port, onboard_led_Pin);
			//HAL_Delay(5);

			if(usart2_new_message_ready_flag == 0)
			{
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_UpdateScreen();
			}
		}

		if(usart2_new_message_ready_flag)
		{
			usart2_new_message_ready_flag = 0;
			ssd1306_SetCursor(0,44);
			ssd1306_WriteString("SMS        ", Font_11x18, White);
			ssd1306_UpdateScreen();
		}
	}
	//*/
	//***********************************************
	// DEEP DEBUG ***********************************


	int step1 = 0;
	UNUSED(step1);
	int step2 = 0;

    while (1)
	{
    	/*
    	if(HAL_GetTick()%1000 == 0)
    	{
			counter++;
			sprintf(message, "%u", (unsigned int)counter);
			ssd1306_SetCursor(0,30);
			ssd1306_WriteString("           ", Font_11x18, White);
			ssd1306_SetCursor(0,30);
			ssd1306_WriteString(message, Font_11x18, White);
			ssd1306_UpdateScreen();
    	}
		//*/

    	if(HAL_GetTick()%500 == 0)
    	{
    		/*
    		ssd1306_SetCursor(110,0);
    		if(odd_even)
    			ssd1306_WriteString("+", Font_11x18, White);
    		else
    			ssd1306_WriteString("x", Font_11x18, White);
    		ssd1306_UpdateScreen();
    		odd_even = (odd_even+1)%2;
    		//*/

    		HAL_GPIO_TogglePin(onboard_led_GPIO_Port, onboard_led_Pin);
    		HAL_Delay(5);

    		/*
    		if(usart2_new_message_ready_flag == 0)
			{
				ssd1306_SetCursor(0,44);
				ssd1306_WriteString("           ", Font_11x18, White);
				ssd1306_UpdateScreen();
			}
    		//*/
    	}


    	//usart1_buffer_action();
    	gps_action();

    	if(usart2_new_message_ready_flag)
		{
    		char gsm_message[128];

    		usart2_new_message_ready_flag = 0;
			usart2_processed_messages++;

			// copy message
			usart2_old_message_saved = 0;
			int i;
			for(i=0; i<=usart2_message_length; i++)
				gsm_message[i] = usart2_buffer[usart2_read_buffer][i];
			usart2_old_message_saved = 1;


			// debug
			gsm_message[usart2_message_length-2] = 0;
			gsm_message[11] = 0;

			ssd1306_SetCursor(0,44);
			ssd1306_WriteString("           ", Font_11x18, White);
			ssd1306_SetCursor(0,44);
			ssd1306_WriteString(gsm_message, Font_11x18, White);
			ssd1306_UpdateScreen();

			if(gsm_message[1] == 'C' && gsm_message[2] == 'R' && gsm_message[3] == 'E' && gsm_message[7] == '1')
			{
				HAL_Delay(3000);
				sprintf(gsm_message, "AT+CMGF=1;&W\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)gsm_message, strlen(gsm_message), 500);
			}

			if(gsm_message[0] == 'l' && gsm_message[1] == 'o' && gsm_message[2] == 'c')
			{
				HAL_Delay(3000);
				sprintf(gsm_message, "AT+CMGS=\"+79219666439\"\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)gsm_message, strlen(gsm_message), 500);
				step2 = 1;
			}
			else if(step2 )
			{
				HAL_Delay(3000);
				sprintf(gsm_message, "%s%c", gps_coordinates, 0x1a);
				HAL_UART_Transmit(&huart2, (uint8_t *)gsm_message, strlen(gsm_message), 500);
				//HAL_UART_Transmit(&huart2, (uint8_t *)"AT\r\n", 2, 500);

				//ssd1306_SetCursor(0,44);
				//ssd1306_WriteString("sms sent   ", Font_11x18, White);
				//ssd1306_UpdateScreen();

				step2 = 0;
			}
				//*/

		}


	}

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
