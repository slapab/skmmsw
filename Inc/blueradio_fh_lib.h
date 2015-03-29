#ifndef _BLUERADIO_FH_LIB_H
#define _BLUERADIO_FH_LIB_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include "blueradio_lib.h"
#include "main_sys.h"
	
	
// You must run that function to call proper handling function:
int_fast8_t bl_fh_run( BL_Data_TypeDef * _hBL, uint_fast16_t index, const char * parstr ) ;
	
// Functios to handle each possible response/event from bluetooth module
// Each function must have the same returned type and input argument, it is: 
// int_fast8_t funcccc( const char * )

/* common responses */
int_fast8_t bl_fh_OK( BL_Data_TypeDef *, const char * ) ;
int_fast8_t bl_fh_ERROR( BL_Data_TypeDef *, const char * ) ;
/* general events */
int_fast8_t	bl_fh_DONE( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_CONNECT( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_DISCONNECT( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_DISCOVERY( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_PAIR_REQ( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_PAIRED( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_PAIR_FAIL( BL_Data_TypeDef *, const char * ) ;
int_fast8_t bl_fh_PK_REQ( BL_Data_TypeDef *, const char * ) ;
int_fast8_t bl_fh_PK_DIS( BL_Data_TypeDef *, const char * ) ;
/* bluetooth LE events: */
int_fast8_t	bl_fh_SCCPS( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_CPU( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_GATT_DONE( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_GATT_DPS( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_GATT_DC( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_GATT_DCD( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_GATT_VAL( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_BRSP( BL_Data_TypeDef *, const char * ) ;
/* Classic bluetooth events: */
int_fast8_t	bl_fh_RN( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_RS( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_UC_REQ( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_PIN_REQ( BL_Data_TypeDef *, const char * ) ;
int_fast8_t	bl_fh_SPP( BL_Data_TypeDef *, const char * ) ;
	
#ifdef __cplusplus
}
#endif

#endif
