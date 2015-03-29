#include "blueradio_fh_lib.h"
#include "stdbool.h"
#include "stddef.h"
#include "main_sys.h"

int_fast8_t bl_fh_run(BL_Data_TypeDef * _hBL , uint_fast16_t index, const char * parstr) {
	
	static int_fast8_t (* const bl_fh_tab[BLR_REPS_CNT])( BL_Data_TypeDef*, const char * ) = {
		/* handling: common responses */
		bl_fh_OK, bl_fh_ERROR, 
		/* handling: general events */
		bl_fh_DONE, bl_fh_CONNECT, bl_fh_DISCONNECT, bl_fh_DISCOVERY, bl_fh_PAIR_REQ, bl_fh_PAIRED, bl_fh_PAIR_FAIL,
		bl_fh_PK_REQ, bl_fh_PK_DIS, 
		/* handling: bluetooth LE events: */
		bl_fh_SCCPS, bl_fh_CPU, bl_fh_GATT_DONE, bl_fh_GATT_DPS, bl_fh_GATT_DC, bl_fh_GATT_DCD,
		bl_fh_GATT_VAL, bl_fh_BRSP,
		/* handling: classic bluetooth events: */
		bl_fh_RN, bl_fh_RS, bl_UC_REQ, bl_fh_PIN_REQ, bl_fh_SPP
	} ;
	
	
	return bl_fh_tab[index]( _hBL, parstr  ) ;
	
}


int_fast8_t bl_fh_OK ( BL_Data_TypeDef *_hBL, const char * str) {
	return 1 ;
}

int_fast8_t bl_fh_ERROR ( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }

/* general events */
int_fast8_t	bl_fh_DONE( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_CONNECT( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_DISCONNECT( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_DISCOVERY( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_PAIR_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_PAIRED( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_PAIR_FAIL( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t bl_fh_PK_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t bl_fh_PK_DIS( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
/* bluetooth LE events: */
int_fast8_t	bl_fh_SCCPS( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_CPU( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_GATT_DONE( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_GATT_DPS( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_GATT_DC( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_GATT_DCD( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_GATT_VAL( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_BRSP( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
/* Classic bluetooth events: */
int_fast8_t	bl_fh_RN( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_RS( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_UC_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_PIN_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }
int_fast8_t	bl_fh_SPP( BL_Data_TypeDef *_hBL, const char * str ) { return 1; }


