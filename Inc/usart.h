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
