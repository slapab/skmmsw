#include "usart.h"
#include "gpio.h"
#include <stddef.h>
#include <stdio.h>

UART_HandleTypeDef huart5;

/* UART5 init function */
void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart5);

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* Peripheral clock enable */
    __UART5_CLK_ENABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(UART5_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
		
		// Enable the periperial 
		__HAL_UART_ENABLE(huart);
		
		
		
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __UART5_CLK_DISABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(UART5_IRQn);

  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
}




/* USER CODE BEGIN 1 */

void usart_sendString( const char * string, const size_t size ) {
	
	size_t i ;
	for ( i = 0 ; i < size ; i++ ) {
		// Wait for empty tx buffer 
		while( !(huart5.Instance->SR & USART_SR_TXE) ) ;
		
		//Write one character to TX buffer
		//*(huart5.pTxBuffPtr) = string[i] ;
		huart5.Instance->DR = string[i] ;
		
		// check if last char was a 0x0D 
		if( (string[i] != 0x0D) && (i == (size-1)) ) {
			// Send 0x0D 
			while( !(huart5.Instance->SR & USART_SR_TXE) ) {}
		  //*(huart5.pTxBuffPtr) = 0x0D ;
				huart5.Instance->DR = 0x0D ;
		}
			
	}
	
}


void usart_readCharacter( const UART_HandleTypeDef * handler, const char data ) {
	
	// tylko wyswietl na printf 
	//printf( "%c", *(handler->pRxBuffPtr) ) ;
	printf( "%c", data ) ;
	
}


/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
