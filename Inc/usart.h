/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stddef.h>

extern UART_HandleTypeDef huart5;

void MX_UART5_Init(void);
	 
void usart_sendString( const char * string, const size_t size ) ;
void usart_readCharacter( const UART_HandleTypeDef * handler, const char data ) ;
	 
//Modyfications of some HAL functions
void HAL_UART_IRQHandler_mod(UART_HandleTypeDef *huart) ;
static HAL_StatusTypeDef UART_Receive_IT_mod(UART_HandleTypeDef *huart) ;
HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart); // Defined in stm32f4xx_hal_uart.c ! Rmoved static keyword!	 

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
