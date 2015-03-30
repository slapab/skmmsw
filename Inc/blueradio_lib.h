#ifndef _BLUERADIO_LIB_H
#define _BLUERADIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include <stddef.h>
#include "main_sys.h"
	
	
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
	volatile uint_fast8_t ix_buff ;						/// in the bits is hidden information which buffer is full, e.g 
																						/// if at bit 0 is 1 that means buff[0] char table is full
	volatile uint_fast8_t semafor ;						/// semafor for protecting writing data to that structure
} BLR_buff_TypeDef ;	
	
	
/* Function declarations */
void bl_init_buffers( char * buff_tab, uint_fast8_t no, size_t size ) ;
int_fast8_t bl_handleResp( BL_Data_TypeDef * _hBL, const char * src ) ;
static int_fast16_t match_rsp( const char * str ) ;
static int_fast8_t get_resp_name ( const char * src, char *name, char *resttxt ) ;

#ifdef __cplusplus
}
#endif

#endif
