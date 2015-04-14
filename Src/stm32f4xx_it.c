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

// in main.c 
extern struct blscan_mode_typedef blscan_mode ;

// in main.c - for increment timing
extern BL_Data_TypeDef weather_data ;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/

// Variable to count ms for increment seconds
static uint32_t Hm_ms = 0;
static uint32_t Hm_s = 0;

void SysTick_Handler(void)
{
  
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  
	// Count interval for sensors reading
	++sensors_timing.timer_sensors ;
	
	// Count interval for bluetooth scan start task
	++blscan_mode.timer_scanmode ;

	
	// Keep time alive :D :
	++Hm_ms; // cound ms
	if ( Hm_ms >= 1000 ) {
		++Hm_s ;		// count seconds
		if ( Hm_s >= 60 ) {
			++weather_data.min ; // count minutes
			if ( weather_data.min >= 60 ) {
				++weather_data.hour ; // count hours
				if ( weather_data.hour >= 24 ) {
					weather_data.hour = 0 ;
				} // check hours
				weather_data.min = 0 ;
			} // check minutes
			Hm_s = 0 ;
		} // check s
		Hm_ms = 0;
	} // Check ms
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
