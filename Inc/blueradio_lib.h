#ifndef _BLUERADIO_LIB_H
#define _BLUERADIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include <stddef.h>
#include <signal.h>
#include <stdbool.h>
#include "main_sys.h"
	
// That are assembly functions for semaphore - defined in semaphore_src.s file
extern void sem_inc( volatile void * _sem ) ;
extern void sem_dec( volatile void * _sem ) ;

	
	
#define BLR_REP_NAME_LEN 11			// Minimal size of table that contain response/event name with \0 character
#define BLR_REPS_CNT 24					// Says how many responses/events are programmed to be recognized
#define BLR_RESTTXT_SIZE 100		// Size for table which will contain text received form bluetooth module ( without name and a few more chars )
	
/**	Contain information about buffers which is used to 
*		store data - AT respnses / events received by USART
*
*/
#define BLR_STRUCT_BUFF_NO 3
#define BLR_STRUCT_BUFF_SIZE 120
typedef struct blr_buffer_typedef {
	volatile char * volatile buff[BLR_STRUCT_BUFF_NO] ;	/// contain buffers pointers
	
	volatile sig_atomic_t ix_buff[BLR_STRUCT_BUFF_NO] ;	/// provides atomic operation
	// rezerwacja : 200 + 1 + bufer index - potem wpisanie indeksu zapelnionego to po porostu -201
	// 200 - wolny
	// < 200 - zawiera indeks buforu zapelnionego
	
	volatile sig_atomic_t semaphore ;
	
	volatile sig_atomic_t int_wait ;					// If it is != 0 then UART RX cplt callback was blocket by semaphore
	void * volatile ptr_data ;								// Stores address of data that must be send to UART RX callback when
																						// that callback was suspended. It always must be set if int_wait is set
	
} BLR_buff_TypeDef ;	
	
	
/* Function declarations */
void bl_buffers_init(  BLR_buff_TypeDef * hBL, char * buff_tab, uint_fast8_t no, size_t size ) ;
int32_t bl_buffers_reserve( BLR_buff_TypeDef * hBL ) ;
void bl_buffers_release( void ) ;
int32_t bl_buffer_clearStatus( BLR_buff_TypeDef * hBL );

int32_t bl_chngStatBuff( BLR_buff_TypeDef * hBL ) ; 		// Must be called in UART RX callbacks after when buffer was read
void bl_checkEvents( BLR_buff_TypeDef * hBL, void * fh_param ) ;

uint32_t bl_advertUpdate( BL_Data_TypeDef * hBL, UART_HandleTypeDef * hUART ) ;
hf_stat_TypeDef bl_advertisingON(
			BL_Data_TypeDef * hBL,
			UART_HandleTypeDef * hUART,
			char * buff
		) ;

void ble_stopallcmd( BL_Data_TypeDef * hBL, UART_HandleTypeDef * hUART, char * buff );

size_t bl_AdvertConfigCMD(
						char * buff, 
						uint32_t w_list,
						uint32_t ad_type,
						uint32_t chn  );

int_fast8_t bl_handleResp( void * _hBL, const char * src ) ;
static int_fast16_t match_rsp( const char * str ) ;
static int_fast8_t get_resp_name ( const char * src, char *name, char *resttxt ) ;

#ifdef __cplusplus
}
#endif

#endif
