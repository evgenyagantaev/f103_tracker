#include "main.h"
#include "stm32f1xx_it.h"

#include "usart1_buffer_interface.h"
#include "usart2_buffer_interface.h"

#include "ssd1306.h"
#include "stdio.h"

#include "extern_globals.h"

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("           ", Font_11x18, White);
	ssd1306_SetCursor(0,22);
	ssd1306_WriteString("           ", Font_11x18, White);
	ssd1306_SetCursor(0,22);
	ssd1306_WriteString("HARD FAULT ", Font_11x18, White);
	ssd1306_SetCursor(0,44);
	ssd1306_WriteString("           ", Font_11x18, White);
	ssd1306_UpdateScreen();

  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
	uint32_t isrflags   = USART1->SR;
	uint32_t cr1its     = USART1->CR1;
	uint32_t errorflags;

	uint16_t usart_data;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
	if (errorflags == RESET)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		 if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			usart_data = (uint16_t) USART1->DR;
			usart1_buffer[usart1_write_buffer][usart1_write_index] = (char)usart_data;
			usart1_write_index++;
			if((char)usart_data == '\n') // new full message received
			{
				if(usart1_buffer[usart1_write_buffer][1]=='G' && usart1_buffer[usart1_write_buffer][2]=='N' && usart1_buffer[usart1_write_buffer][3]=='R' && usart1_buffer[usart1_write_buffer][4]=='M' && usart1_buffer[usart1_write_buffer][5]=='C')
				{
					usart1_received_messages++;

					if(usart1_old_message_saved)
					{
						usart1_buffer[usart1_write_buffer][usart1_write_index] = 0;
						usart1_message_length = usart1_write_index;
						usart1_write_index = 0;
						usart1_write_buffer = (usart1_write_buffer + 1) % 2;
						usart1_read_buffer = (usart1_read_buffer + 1) % 2;
						usart1_new_message_ready_flag = 1;
					}
					else
					{
						usart1_write_index = 0;
						usart1_message_lost = 1;
					}
				}
				else
					usart1_write_index = 0;


			}
			else
			{
				if(usart1_write_index >= USART1_BUFFER_LENGTH) // buffer overflow
					usart1_write_index = 0;
			}

		}
	}
	else // some errors
	{
		ssd1306_SetCursor(0,0);
		ssd1306_WriteString("usart1     ", Font_11x18, White);
		ssd1306_SetCursor(0,22);
		ssd1306_WriteString("error      ", Font_11x18, White);
		ssd1306_UpdateScreen();

		// clear error flags
		uint32_t aux;
		aux = USART1->SR;
		aux = USART1->DR;
		UNUSED(aux);
	}
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
	uint32_t isrflags   = USART2->SR;
	uint32_t cr1its     = USART2->CR1;
	uint32_t errorflags;

	uint16_t usart_data;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
	if (errorflags == RESET)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			usart_data = (uint16_t) USART2->DR;
			usart2_buffer[usart2_write_buffer][usart2_write_index] = (char)usart_data;
			usart2_write_index++;
			if((char)usart_data == '\n') // new full message received
			{
				if(usart2_write_index > 1)
				{

					//if(usart2_buffer[usart2_write_buffer][1]=='C' && usart2_buffer[usart2_write_buffer][2]=='M' && usart2_buffer[usart2_write_buffer][3]=='T' && usart2_buffer[usart2_write_buffer][4]==':')
					//if(usart2_buffer[usart2_write_buffer][1]=='C' && usart2_buffer[usart2_write_buffer][2]=='R' && usart2_buffer[usart2_write_buffer][3]=='E' && usart2_buffer[usart2_write_buffer][4]=='G')
					if(1)
					{
						usart2_received_messages++;

						if(usart2_old_message_saved)
						{
							usart2_buffer[usart2_write_buffer][usart2_write_index] = 0;
							usart2_message_length = usart2_write_index;
							usart2_write_index = 0;
							usart2_write_buffer = (usart2_write_buffer + 1) % 2;
							usart2_read_buffer = (usart2_read_buffer + 1) % 2;
							usart2_new_message_ready_flag = 1;
						}
						else
						{
							usart2_write_index = 0;
							usart2_message_lost = 1;
						}
					}
					else
						usart2_write_index = 0;
				}


			}
			else
			{
				if(usart2_write_index >= USART2_BUFFER_LENGTH) // buffer overflow
					usart2_write_index = 0;
			}

		}
	}
	else // some errors
	{
		ssd1306_SetCursor(0,0);
		ssd1306_WriteString("usart2     ", Font_11x18, White);
		ssd1306_SetCursor(0,22);
		ssd1306_WriteString("error      ", Font_11x18, White);
		ssd1306_UpdateScreen();

		// clear error flags
		uint32_t aux;
		aux = USART2->SR;
		aux = USART2->DR;
		UNUSED(aux);
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
