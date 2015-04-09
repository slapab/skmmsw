#ifndef _BLUERADIO_FH_LIB_H
#define _BLUERADIO_FH_LIB_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include "blueradio_lib.h"
#include "main_sys.h"
	
	
// You must run that function to call proper response/event handle function
void bl_fh_run( void * _hBL, uint_fast16_t index, const char * parstr ) ;
	

// That function should be useful in response/event hadnle functions
int_fast16_t bl_split_data( const char * src, char * out, const uint_fast16_t out_size ) ;	

	
// Functios to handle each possible response/event from bluetooth module
// Each function must have the same returned type and input argument, it is: 
// int_fast8_t funcccc( const char * )
// void * - as that param. can be any type of data, usefull can be: struct BL_Data_TypeDef

/* common responses */
static void bl_fh_OK( void *, const char * ) ;
static void bl_fh_ERROR( void *, const char * ) ;
/* general events */
static void	bl_fh_DONE( void *, const char * ) ;
static void	bl_fh_CONNECT( void *, const char * ) ;
static void	bl_fh_DISCONNECT( void *, const char * ) ;
static void	bl_fh_DISCOVERY( void *, const char * ) ;
static void	bl_fh_PAIR_REQ( void *, const char * ) ;
static void	bl_fh_PAIRED( void *, const char * ) ;
static void	bl_fh_PAIR_FAIL( void *, const char * ) ;
static void bl_fh_PK_REQ( void *, const char * ) ;
static void bl_fh_PK_DIS( void *, const char * ) ;
/* bluetooth LE events: */
static void	bl_fh_SCCPS( void *, const char * ) ;
static void	bl_fh_CPU( void *, const char * ) ;
static void	bl_fh_GATT_DONE( void *, const char * ) ;
static void	bl_fh_GATT_DPS( void *, const char * ) ;
static void	bl_fh_GATT_DC( void *, const char * ) ;
static void	bl_fh_GATT_DCD( void *, const char * ) ;
static void	bl_fh_GATT_VAL( void *, const char * ) ;
static void	bl_fh_BRSP( void *, const char * ) ;
/* Classic bluetooth events: */
static void	bl_fh_RN( void *, const char * ) ;
static void	bl_fh_RS( void *, const char * ) ;
static void	bl_UC_REQ( void *, const char * ) ;
static void	bl_fh_PIN_REQ( void *, const char * ) ;
static void	bl_fh_SPP( void *, const char * ) ;
	
#ifdef __cplusplus
}
#endif

#endif
