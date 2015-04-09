#include "stm32f4xx.h"
#include "stdio.h"

#include "blueradio_lib.h"

extern BLR_buff_TypeDef blr_buffers ;		//Bluetooth UART buffers stearing handle 

void HAL_UART_RxCpltCallback( UART_HandleTypeDef * huart ) {
	
	int32_t ind ;
	HAL_StatusTypeDef halret ;
	
	if ( blr_buffers.semaphore != 0 ) {
		// inform bluetooth lib about data ready and reserve next freed buffer
		ind = bl_chngStatBuff( &blr_buffers ) ;
		
		if ( ind >= 0 ) {
			halret = HAL_UART_Receive_IT( huart, (uint8_t*)blr_buffers.buff[ind], BLR_STRUCT_BUFF_SIZE );
			if ( halret == HAL_OK ) {
			}
				
		} // All OK, new buffer is reserwed at index IND
		else if ( ind == -2 ) {
			blr_buffers.int_wait = 1 ;			// main() must call that where unblock semaphore
			blr_buffers.ptr_data = huart ;	// store address of argument that must be passed too to that function
		}	// there is no free buffers
		
	}
	else {
		blr_buffers.int_wait = 1 ;	
		blr_buffers.ptr_data = huart ;	// store address of argument that must be passed too to that function
	} // Inform that is missed one callback - data are waiting
	
}
