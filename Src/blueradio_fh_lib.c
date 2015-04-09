#include "blueradio_fh_lib.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/*User includes*/
#include "main_sys.h"

// in main.c - bluetooth connection status
extern SYS_CONN_TypeDef conn_stat ;

/**
*		You can (should if it is possible) use these variables in bl_fh_xxxxx handling functions
*		see example function: bl_fh_ERROR()
*		\see bl_fh_ERROR()
*/
static volatile int_fast16_t fh_run_ret ;			/// For store return value from bl_fh_run()
static volatile char out_data[10] ;						/// Can be useas 3'rd value in bl_fh_run()
static volatile size_t i ;										/// Useful for loops



/**
*
*/
void bl_fh_run( void * _hBL , uint_fast16_t index, const char * parstr) {
	
	static void (* const bl_fh_tab[BLR_REPS_CNT])( void *, const char * ) = {
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
* 	@return >0 number of chars read from 'src' before parser met ',' - means that in src
*		are some data to read
*		@return <0 number of chars read form 'src' before parser met '\0'
*		<b>minus 500</b> - means that <b>There is no more data in 'src'!</b>
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
static void bl_fh_OK ( void *_hBL, const char * str ) {
	
	PRINTF("BL_FH_OK", 0);
	
	((BL_Data_TypeDef *)_hBL)->status = BL_RESPONSE_OK ;
	
	return ;
}


// ERROR response handle
static void bl_fh_ERROR ( void *_hBL, const char * str ) {
	
	PRINTF("BL_FH_ERROR", 0);
	
	// Read error code
	fh_run_ret = bl_split_data( &str[0], (char*)&out_data[0], 2 ) ;
	// Test output
	if( fh_run_ret == 0 ) {
		// library's internal error
		((BL_Data_TypeDef *)_hBL)->status = LIB_HF_ERR ;
		return ;
	} 
	else if ( fh_run_ret < 0 ) {
		
		fh_run_ret += 500 ;		// Get number of copied chars
		// Convert ascii to BLUETOOTH AT ERROR values
		fh_run_ret = ( out_data[0] == '1' )?10:0 ;
		// Convert from ascii to decimal
		fh_run_ret += ( (int_fast16_t)out_data[1] - (int_fast16_t)0x30 ) ;			
		// Save error code
		((BL_Data_TypeDef *)_hBL)->error = (hf_err_TypeDef)fh_run_ret ;					
	
	} // means that just read data are the last in 'str'
	
	
	// else {
	// } // means that there is more data in 'str' - not possible at current version of bluetooth module firmware
	
	((BL_Data_TypeDef *)_hBL)->status = BL_RESPONSE_ERR ;
	
	return;
}


/* GENERAL EVENTS */
static void	bl_fh_DONE( void *_hBL, const char * str ) { 	
	// Documentation says that DONE event has two additional data - single digit in ASCII for each
	PRINTF("BL_FH_DONE", (10* (uint_fast8_t)(str[0] - 0x30)) + (uint_fast8_t)(str[2] - 0x30) ) ;
	
	((BL_Data_TypeDef *)_hBL)->status = BL_EV_DONE ;
	
return;
}

static void	bl_fh_CONNECT( void *_hBL, const char * str ) {
	
	// CONNECT,x,0,0,ADDR
	// x - is the connection handler - it can be 1 or more ASCII
	// ADDR is the 6 bytes - this is 12 ASCII characters
	
	((BL_Data_TypeDef *)_hBL)->status = BL_EV_CONNECT;
	((BL_Data_TypeDef *)_hBL)->error = BL_NO_ERRRORS;
	
	
	// Read connection handle
	// Can be < 10 and >= 10 value - so need check it by return value from bl_split_data()
	fh_run_ret = bl_split_data( &str[0], (char*)&out_data[0], 2 ) ;
	if( fh_run_ret == 1 )
		((BL_Data_TypeDef *)_hBL)->conn.conn_handler = (uint_fast8_t)(out_data[0] - 0x30) ;
	else if ( fh_run_ret == 2 )
		((BL_Data_TypeDef *)_hBL)->conn.conn_handler = (uint_fast8_t)((10*(out_data[0] - 0x30)) + (out_data[1] - 0x30) ) ;
	else if ( fh_run_ret == 0 ){
		((BL_Data_TypeDef *)_hBL)->status = LIB_HF_ERR ;
		((BL_Data_TypeDef *)_hBL)->error = BL_CONNECT_HANDLE_ERR ;
		return ;
	} // Error while extraction of handler

	
	// Get the address of connected device
	fh_run_ret = 1 ;
	for ( i = 0 ; fh_run_ret > 0 ;) {
		// reutrn value will be less than 0 when address will be read and that will be last data
		fh_run_ret = bl_split_data( &str[i], (char*)&out_data[0], 12 ) ;	
		i += (fh_run_ret +1) ;	// getting next values - not the same
	}
	
	if ( fh_run_ret < 0 ) {
		// Copy 12 bytes of addres to structure
		memcpy( (char*)((BL_Data_TypeDef *)_hBL)->conn.rem_mac, (char*)&out_data[0], 12 );
	}	// Address has been read properly
	else {
		//((BL_Data_TypeDef *)_hBL)->status = LIB_HF_ERR ;
		((BL_Data_TypeDef *)_hBL)->error = BL_CONNECT_ADDR_ERR ;
	} // Couldn't read address - ERROR
	
	
	// Set status as connected: 
	conn_stat = CONNECTED ;
	
	PRINTF("BL_FH_CONNECT", ((BL_Data_TypeDef *)_hBL)->conn.conn_handler );
	PRINTF(out_data, 0) ;
	
	
	return; 
}
static void	bl_fh_DISCONNECT( void *_hBL, const char * str ) {
	
	// Set status as disconnected: 
	conn_stat = DISCONNECTED ;
	
	return;
}
static void	bl_fh_DISCOVERY( void *_hBL, const char * str ) { return; }
static void	bl_fh_PAIR_REQ( void *_hBL, const char * str ) { return; }
static void	bl_fh_PAIRED( void *_hBL, const char * str ) { return; }
static void	bl_fh_PAIR_FAIL( void *_hBL, const char * str ) { return; }
static void bl_fh_PK_REQ( void *_hBL, const char * str ) { return; }
static void bl_fh_PK_DIS( void *_hBL, const char * str ) { return; }
/* BLUETOOTH LE EVENTS */
static void	bl_fh_SCCPS( void *_hBL, const char * str ) { return; }
static void	bl_fh_CPU( void *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DONE( void *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DPS( void *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DC( void *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_DCD( void *_hBL, const char * str ) { return; }
static void	bl_fh_GATT_VAL( void *_hBL, const char * str ) { return; }
static void	bl_fh_BRSP( void *_hBL, const char * str ) { return; }
/* CLASSIC BLUETOOTH EVENTS */
static void	bl_fh_RN( void *_hBL, const char * str ) { return; }
static void	bl_fh_RS( void *_hBL, const char * str ) { return; }
static void	bl_UC_REQ( void *_hBL, const char * str ) { return; }
static void	bl_fh_PIN_REQ( void *_hBL, const char * str ) { return; }
static void	bl_fh_SPP( void *_hBL, const char * str ) { return; }


