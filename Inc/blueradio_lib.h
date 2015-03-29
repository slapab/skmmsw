#ifndef _BLUERADIO_LIB_H
#define _BLUERADIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include "main_sys.h"
	
	
#define BLR_REP_NAME_LEN 11			// Minimal size of table that contain response/event name with \0 character
#define BLR_REPS_CNT 24					// Says how many responses/events are programmed to be recognized
#define BLR_RESTTXT_SIZE 100		// Size for table which will contain text received form bluetooth module ( without name and a few more chars )
	
/* Function declarations */
int_fast8_t bl_handleResp( BL_Data_TypeDef * _hBL, const char * src ) ;
static int_fast16_t match_rsp( const char * str ) ;
static int_fast8_t get_resp_name ( const char * src, char *name, char *resttxt ) ;

#ifdef __cplusplus
}
#endif

#endif
