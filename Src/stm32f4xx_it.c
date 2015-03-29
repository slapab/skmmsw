/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "usart.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c3;

extern UART_HandleTypeDef huart5;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/**
* @brief This function handles UART5 global interrupt.
*/
void UART5_IRQHandler(void)
{
	
  HAL_UART_IRQHandler(&huart5); 
	// Obsluzyc odebrane dane
	usart_readCharacter(&huart5, huart5.Instance->DR) ;
}




void I2C3_EV_IRQHandler( void ) {
	HAL_I2C_EV_IRQHandler( &hi2c3 ) ;
}

void I2C3_ER_IRQHandler( void ) {
	HAL_I2C_ER_IRQHandler( &hi2c3 ) ;	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
