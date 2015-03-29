#include "i2c.h"


I2C_HandleTypeDef hi2c3 ;


void MX_I2C_Init( void ) {
	
	hi2c3.Instance = I2C3 ;
	hi2c3.Init.ClockSpeed = 100000 ;
	hi2c3.Init.DutyCycle =   I2C_DUTYCYCLE_2  ;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT ;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED ;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED ;
	hi2c3.Init.NoStretchMode =  I2C_NOSTRETCH_DISABLED ;
	hi2c3.Init.OwnAddress1 = 0x000CBAF0 ;
	hi2c3.Init.OwnAddress1 = 0x000CBAF0 ;
	
	HAL_I2C_Init( &hi2c3 );
}


void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if ( hi2c->Instance == I2C3 ) {
		
		//I2C3 peripheral clock enable 
		__I2C3_CLK_ENABLE() ;

		// Used gpio configuration: 
		// SPI3 SCL - PA8
		// SPI3 SDA - PC9
		
		// For SCL
		GPIO_InitStruct.Pin = GPIO_PIN_8 ;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// For SDA
		GPIO_InitStruct.Pin = GPIO_PIN_9 ;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		// Set NVIC for that interrupt
		
		/* Peripheral interrupt init*/
    HAL_NVIC_SetPriority( I2C3_EV_IRQn, 0, 4) ;
    HAL_NVIC_EnableIRQ( I2C3_EV_IRQn );
		
		// Enable the periperial:
		__HAL_I2C_ENABLE(hi2c);
	}
	
}
