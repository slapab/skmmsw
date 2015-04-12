/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "usart.h"
#include "i2c.h"
#include "main_sys.h"

extern I2C_HandleTypeDef hi2c3;

extern UART_HandleTypeDef huart5;

// in main.c
extern struct time_sens_TypeDef sensors_timing ;
// rozwiazanie poprzez advertising z dwoch stron: 
extern struct sys_task_TypeDef sys_tasks ;


/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  
	//Count interval for sensors reading
	++sensors_timing.timer_sensors ;
	
	//ozwiazanie poprzez advertising z dwoch stron: 
	++sys_tasks.tim ;
}

/**
* @brief This function handles UART5 global interrupt.
*/
void UART5_IRQHandler(void)
{
	
  //HAL_UART_IRQHandler(&huart5); 
	HAL_UART_IRQHandler_mod(&huart5); 
	
	// Obsluzyc odebrane dane
	// usart_readCharacter(&huart5, huart5.Instance->DR) ;
}




void I2C3_EV_IRQHandler( void ) {
	HAL_I2C_EV_IRQHandler( &hi2c3 ) ;
}

void I2C3_ER_IRQHandler( void ) {
	HAL_I2C_ER_IRQHandler( &hi2c3 ) ;	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
