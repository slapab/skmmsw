#include "blueradio_lib.h"
#include "stdbool.h"
#include "stddef.h"

#include "blueradio_fh_lib.h"

BLR_buff_TypeDef blr_buffers ;


/** Contain possible response or event strings
*		Index  in this table is the same as index in the table pointers to functions,
*		e.g. "OK" have index 0, so in the table pointers to functions at index 0 is the
* 	pointer which points to "ok_handle" function.
*/

static const char blrsp_tab[BLR_REPS_CNT][BLR_REP_NAME_LEN] = {
	/* Common responses: */
	"OK", "ERROR", 
	/* General events: */
	"DONE", "CONNECT", "DISCONNECT", "DISCOVERY" , "PAIR_REQ", "PAIRED", "PAIR_FAIL",
	"PK_REQ", "PK_DIS", 
	/* Bluetooth Low Energy events: */
	"SCCPS" , "CPU", "GATT_DONE", "GATT_DPS", "GATT_DC", "GATT_DCD", "GATT_VAL", "BRSP",
	/* Classic Bluetooth events: */
	"RN" , "RS", "UC_REQ" , "PIN_REQ", "SPP"
} ;



/**	Init bluetooth buffer structure 
*
*		@param buff pointer to 2-dimensional char array which will contain AT response/event data
*		@param no number of buffers should be the same as BLR_STRUCT_BUFF_NO macro 
*		@param size how many elements are in each buffer - should be the same as BLR_STRUCT_BUFF_SIZE macro 
*/
void bl_init_buffers( char * buff_tab, uint_fast8_t no, size_t size ) {
	
	size_t i ;
	for ( i = 0 ; i < no ; ++ i ) 
		blr_buffers.buff[i] = &buff_tab[i * size] ;	// Copy buffers pointers
	
	blr_buffers.ix_buff = 0 ;
	//init semafor:
	blr_buffers.semafor = 1 ;
}



/**
*
*/
int_fast8_t bl_handleResp( BL_Data_TypeDef * _hBL, const char * src ) {
	
	static char _name[BLR_REP_NAME_LEN] ;
	static char _resttxt[BLR_RESTTXT_SIZE] ;
	
	int_fast8_t ret ;
	
	// Get response/event name
	ret = get_resp_name( src, &_name[0], &_resttxt[0]) ;
	if( ret < 0 ) 
		return ret ;			// Error, could not be read proper name
	
	// Try to get index of function which will be called to handle that response/event
	ret = match_rsp( &_name[0] ) ;
	if( ret < 0 )
		return ret ;			// Error, could not match passed string to available handle functions
	
	
	// Got index, so call proper handle functions for that response/event from bluetooth
	bl_fh_run( _hBL, ret, &_resttxt[0] ) ;
	
	
	/* to do error handler */
	
	return 0 ;
}





/**	@brief That function trying to extract response/event name (e.g OK, ERROR) from 
*		string received from BlueRadio bluetooth module trought UART link.
*
*		@param src pointer to char table which contains RAW string that has been just received from BlueRadio
*		@param name pointer to char table which will contain response/event name, '\0' terminated. 
*		Size of that table musnt not be smaller than BLR_REP_NAME_LEN macro says
*		@param resttxt pointer to char table which will contain rest of RAW string that has been just received from BL
*		without at the beginign: 0x0D 0x0A response/event name and ',' character, and at the end instead of 0x0D and 0x0A
*		will be placed '\0' character. 
*		@return -2 Error. At least one of passed pointer points to NULL
*		@return >=1 size of valid data that were copied to 'name' - includes \0 char
*
*		@note *resttxt is not using and modified for 'OK' response
*		@warning *resttxt must have a big size to contain whole text recived from blueooth module. Use BLR_RESTTXT_SIZE macro.
*		@warning *name size must not be smaller than BLR_REP_NAME_LEN macro says
*/
static int_fast8_t get_resp_name ( const char * src, char *name, char *resttxt ) {
	
	size_t i = 0, j = 0, k = 0;
	
	if ( (src == NULL) || (name == NULL ) || (resttxt == NULL ) ) 
		return -2 ;
	
	// start at 3'rd character, because the first two are standard: 0x0D followed by 0x0A
	for( j = 0, i = 2 ; (src[i] != 0x0D) && 
							 (src[i+1] != 0x0A) ; ++j, ++i ) {
		
		// Stop copying response/event name and copy rest of string to resttxt table
		// and exit if found ',' character
		if ( src[i] == ',' ) {
			
			// Copy the rest of string to resttxt buffer (without 0x0D followed by 0x0A)
			for (k = 0, i=i+1 ; (src[i] != 0x0D) && (src[i+1] != 0x0A) ; ++k, ++i ) {
				resttxt[k] = src[i] ;
			} 
			resttxt[k] = '\0' ; // Add '\0' character at the end instead of 0x0D followed by 0x0A
			
			break ;
		}
		
		name[j] = src[i] ;			// copy character to place pointed by 'name'
	}

	if ( j != 0 ) {
		name[j] = '\0' ;			// terminate name string with '\0' character
		++j ;									// add one to return number of bytes that are valid in the 'name' table
	}
	return j ;
}








/**	@brief Tries to match string 'str' to standard responses / events BlueRadio module
*
*		@param *str pointer to string (must be '\0' terminated)
* 	@return -2 if str points to NULL
*		@return -1 if not matched to any of standard responses / events
* 	@return >=0 it is the index which shall be used to call proper function from the table pointers to functions
*/
int_fast16_t match_rsp( const char * str ) {
	
	static const uint_fast8_t rsp_cnt = BLR_REPS_CNT ;
	static const uint_fast8_t rsp_len = BLR_REP_NAME_LEN ;
	
	int_fast8_t i, j ;
	

	if ( str == NULL ) 
		return -2 ;
	
	// Find index of response or event - need to call proper function
	for ( i = 0 ; i < rsp_cnt ; ++i ) {
		for( j = 0 ; j < rsp_len ; ++j ) {
			
			if ( str[j] == blrsp_tab[i][j] ) {
				
				if ( str[j] == '\0' )
					return i ;					// match found - retrun index which function need to call
				else
					continue ;					// not matched yet
			}
			else {
				break ;								// mismatch - try next string
			}
			
		}
		
	}
	
	return -1 ;									// not matched at all
}
