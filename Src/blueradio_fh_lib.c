#include "blueradio_fh_lib.h"
#include "stdbool.h"
#include "stddef.h"
#include "main_sys.h"


/**
*		You can (should if it is possible) use these variables in bl_fh_xxxxx handling functions
*		see example function: bl_fh_ERROR()
*		\see bl_fh_ERROR()
*/
static int_fast16_t fh_run_ret ;			/// For store return value from bl_fh_run()
static char out_data[10] ;						/// Can be useas 3'rd value in bl_fh_run()




/**
*
*/
void bl_fh_run(BL_Data_TypeDef * _hBL , uint_fast16_t index, const char * parstr) {
	
	static void (* const bl_fh_tab[BLR_REPS_CNT])( BL_Data_TypeDef*, const char * ) = {
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
	
	// Call proper handle function
	bl_fh_tab[index]( _hBL, parstr  ) ;
	return ; 
	
}


/**	@brief Store in 'out' chars that were read from 'src' until parser met ',' or '\0'
*	
*		@param src input string to scanning - must be terminated with symbol: '\0' 
* 				( function get_resp_name() should be used on src string before calling that function )
*		@param out table where will be placed data - extracted from 'src'
*		@param out_size is the size of 'out' table
*		@return 0 if was a problem with extracting data from string
* 	@return >0 number of chars read from 'src' before parser met ',' - means that in src are some data to read
*		@return <0 number of chars read form 'src' before parser met '\0' <b>minus 500</b> - means that <b>There is no more data in 'src'!</b>
*		
*		@warning In 'src' at the begining can not be 0x0D followed by 0x0A chars! (\see get_resp_name())
*		@warning There is no '\0' in the 'out' table
*		@warning 'src' string must be '\0' terminated
*/
int_fast16_t bl_split_data( const char * src, char * out, const uint_fast16_t out_size ) {
	
	uint_fast16_t i, j;
	for( j = 0, i = 0; (j < out_size) ; ++i ) {
			if ( src[i] == ',' ) {
				// j contain number of chars that was read untli ',' met
				return j;
			} else if ( src[i] == '\0' ) {
				return j - 500 ;
			}
	 	 	else {
				out[j] = src[i] ;
				++j ;
			}
	}

	// if read and valid data have the same size as out table
	if ( (src[j] == ',') || ( src[j] == '\0'))
		return j;
	
	return 0 ; 	// If loop was ended itself - means that data cannot be extract properly
}


/* COMMAND STATUS RESPONSES */

// OK response handle
static void bl_fh_OK ( BL_Data_TypeDef *_hBL, const char * str ) {
	
	PRINTF("BL_FH_OK", 0);
	
	_hBL->status = BL_RESPONSE_OK ;
	
	return ;
}


// ERROR response handle
static void bl_fh_ERROR ( BL_Data_TypeDef *_hBL, const char * str ) {
	
	PRINTF("BL_FH_ERROR", 0);
	
	// Read error code
	fh_run_ret = bl_split_data( &str[0], &out_data[0], 2 ) ;
	// Test output
	if( fh_run_ret == 0 ) {
		// library's internal error
		_hBL->status = LIB_HF_ERR ;
		return ;
	} 
	else if ( fh_run_ret < 0 ) {
		// means that just read data are the last in 'str'
		fh_run_ret += 500 ;
		// Convert ascii to BLUETOOTH AT ERROR values
		fh_run_ret = ( out_data[0] == '1' )?10:0 ;
		fh_run_ret += ( (int_fast16_t)out_data[1] - (int_fast16_t)0x30 ) ;			// Convert from ascii to decimal
		_hBL->error = fh_run_ret ;																							// Save error code
	} 
	// means that there is more data in 'str' - not possible at current version of bluetooth module firmware
	// else {}
	
	_hBL->status = BL_RESPONSE_ERR ;
	
	return;
}


/* GENERAL EVENTS */
static void	bl_fh_DONE( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_CONNECT( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_DISCONNECT( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_DISCOVERY( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_PAIR_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_PAIRED( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_PAIR_FAIL( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void bl_fh_PK_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void bl_fh_PK_DIS( BL_Data_TypeDef *_hBL, const char * str ) { return; }
/* BLUETOOTH LE EVENTS */
static void	bl_fh_SCCPS( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_CPU( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DONE( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DPS( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DC( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DCD( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_VAL( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_BRSP( BL_Data_TypeDef *_hBL, const char * str ) { return; }
/* CLASSIC BLUETOOTH EVENTS */
static void	bl_fh_RN( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_RS( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_UC_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_PIN_REQ( BL_Data_TypeDef *_hBL, const char * str ) { return; }
static void	bl_fh_SPP( BL_Data_TypeDef *_hBL, const char * str ) { return; }


