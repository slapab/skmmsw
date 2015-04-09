#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f429i_discovery.h"

extern uint8_t i2c_rcv_data[5] ;
/*
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
*/

/*
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	
}
*/

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
		printf("\nbar sens id = %02x \n", i2c_rcv_data[1] ) ; 
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	printf( "\nI2C error \n ") ;
}

