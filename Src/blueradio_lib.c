#include "blueradio_lib.h"
#include "stddef.h"
#include "string.h"

#include "blueradio_fh_lib.h"

	 // For debugging only
	 #define _DEBUG_DEF_
	 #include <stdio.h>
	 #define PRINTF(_txt_, _d_) printf("\n%s : %d\n", (_txt_), (_d_))


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



/**	Init bluetooth's UART buffer structure 
*
*		@param buff pointer to 2-dimensional char array which those will contain AT response/event data
*		@param no number of buffers should be the same as @ref BLR_STRUCT_BUFF_NO macro 
*		@param size how many elements are in each buffer - should be the same as @ref BLR_STRUCT_BUFF_SIZE macro
*/
void bl_buffers_init( BLR_buff_TypeDef * hBL, char * buff_tab, uint_fast8_t no, size_t size ) {
	
	size_t i ;
	for ( i = 0 ; i < no ; ++ i ) {
		hBL->buff[i] = &buff_tab[i * size] ;	// Copy buffers pointers
		hBL->ix_buff[i] = 200 ;								// Set all buffers as free to use
	}
	
	hBL->int_wait = 0 ;
	//Init semafor as free - ready to down
	hBL->semaphore = 1 ;
}



/**	That function resets all bluetooth UART information in
*		@ref BLR_buff_TypeDef structure. 
*	
*		@param hBL pointer to @ref BLR_buff_TypeDef structure
*		@return it return the same values as @ref bl_buffers_reserve() does.
*
*/
int32_t bl_buffer_clearStatus( BLR_buff_TypeDef * hBL ) {
	
	// Reset data in structure: 
	hBL->semaphore = 1 ;
	hBL->int_wait = 0 ;
	hBL->ptr_data = NULL ;
	
	// Set all UART buffers as free to use
	bl_buffers_release() ;
	
	
	// reserve one buffer for next incoming data
	return bl_buffers_reserve( hBL ) ;
	
}

/**	Sets all bluetooth UART buffers as free to use
*
*/
void bl_buffers_release( void ) {
	size_t i ;
	for ( i = 0 ; i < BLR_STRUCT_BUFF_NO ; ++i ) 
		blr_buffers.ix_buff[i] = 200 ;
}


/** Tries to reserve buffer if it is free ( not full and not using ). 
*
*	 @warning	That function is adapted only to @ref BLR_buff_TypeDef structure and
*  function that are using that structure.
*
*	 @param hBL pointer to @ref BLR_buff_TypeDef structure
*	 @retval -1 if in tab: @ref BLR_buff_TypeDef.buff is no at least one free buffer
*	 @retval >=0 index of reserved buffer
*
*/
int32_t bl_buffers_reserve( BLR_buff_TypeDef * hBL ) {
	
	int32_t ix = -1 ;
	size_t i ;
	
	for ( i = 0 ; i < BLR_STRUCT_BUFF_NO ; ++i ) {
		if ( hBL->ix_buff[i] == 200 ) {
			ix = i ;
			hBL->ix_buff[i] += (1 + i) ;
			break ;
		}
	}
	
	return ix ;
}


/**	That function must be called on each place where program goes to
*		'infinite' loop for long time.
*		Checks if there is any of bluetooth UART buffer which contain data to handle.
*		It's parsing data and is trying to get proper action depending on received
*		event / response string. After that it frees buffer and trying reserve new
*		one - but not the same - the buffer which index is as close to 0 as possible.
*		
*		@param hBL pointer to @ref BLR_buff_TypeDef structure
*		@param fh_param pointer to data which will be passed to event/response handle 
*		functions - check for functions in blueradio_lib.c/.h files and @ref bl_handleResp()
*		
*	 	@warning macro: BLR_STRUCT_BUFF_NO must have the same value as number
*		of used buffers for storing UART data
*/
void bl_checkEvents( BLR_buff_TypeDef * hBL, void * fh_param ) {
	size_t i, j;
	int32_t an = -1 ;


	
	if ( hBL->semaphore != 0 ) {
	// Semaphore down
	//sem_dec(&hBL->semaphore) ;
		hBL->semaphore = 0 ;
		
		
		for ( j = 0 ; j < 2 ; ++j ) {
			for ( i = 0 ; i < (BLR_STRUCT_BUFF_NO-1) ; ++i ) {

				/* OLD
				// ix_buff[i] is the current
				// ix_buff[i+1] is the next
				// possibly anomaly is detected when 'current' is >=200, and 'next' < 200. Must be found
				// any values <200 after that anomaly.
				*/
				if ( j == 0 ) {
					if ( ( hBL->ix_buff[i] >= 200 ) && (hBL->ix_buff[i+1] < 200) ) {
						an = i+1 ;
						continue ;
					} // find the last position of anomaly
				}	// if j == 0
				else {

					if ( an != -1 ) {
						break ;
					}	// If anomaly was detected
					else if ( hBL->ix_buff[i] < 200 ){
						an = i ;
						break ;
					} // No anomaly. Find the first full buffer
				}	// if j == 1
			}
		}
		
		// semaphore up!
		// sem_inc( &hBL->semaphore );
		hBL->semaphore = 1 ;
		
		// Check if UART RX callbackcplt is waiting ( was blocket by semaphre )
		if ( hBL->int_wait != 0 ) {
			hBL->int_wait = 0 ;
			// call that function with his argument
			HAL_UART_RxCpltCallback( hBL->ptr_data ) ;
		}
		
		if ( an != -1 ) {
			// 'an' is the index of full buffer which must be handled
			// Call parser:
			bl_handleResp( fh_param, (char *)hBL->buff[an] ) ;
			
			hBL->ix_buff[an] = 200 ; // free that buffer

		} // Handle response/event - buffer pointed by index: ix_buff[an]
	
	}
}


/**	Must be called after each UART receive (RX) complete.
*		That function tries to get index from UART buffers table - because there
*		are data to parsing - and tries to reserve next free buffer - for next
*		incoming data
*	
*		@param hBL pointer to @ref BLR_buff_TypeDef structure
*		@retval -1 that is fatal error - mising data - couldn't get index of previous reserved buffer
*		@retval -2 there is no free buffers in UART buffers table - can not reserve empty buffer for next data
*		@retval >=0 the index of just reserved buffer - for incoming data
*		
*		@warning That function use @ref BLR_STRUCT_BUFF_NO macro. You shoud set that macro
*		with number of created UART buffers.
*/
int32_t bl_chngStatBuff( BLR_buff_TypeDef * hBL ) {
	
	size_t i ;
	int32_t ix = -1;
	
	// Semaphore down
	hBL->semaphore = 0;
	
	// Find reserved buffer -> it has had just received data
	for( i = 0 ; i < BLR_STRUCT_BUFF_NO ; ++i ) {
		if ( hBL->ix_buff[i] > 200 ) {
			ix = i ;
			break ;
		}
	}
	
	// that is fatal error! Missing data!
	if ( ix == -1 ) {
		hBL->semaphore = 1;
		return -1 ;
	}
	
	// Set information that here are new data
	hBL->ix_buff[ix] -= 201 ; 				
	
	
	// Reserve free buffer for next incomming data
	ix = bl_buffers_reserve( hBL ) ;
		
	// Semaphore up
	hBL->semaphore = 1;
	
	// There is no free buffer!
	if ( ix == -1 ) {
		return -2 ;
	}
	
	return ix ;
}



/**	Use this function to analyze AT format ascii bytes which has been
*		just received from bluetooth. 
*		@warning This function only parsing that bytes for matching event/response
*		to proper handle function. The Developer must implement that functions - see
*		bluetooth_fh_lib.c/.h files.
*		
*		@param _hBL pointer to data which will be passed to proper event/response
*		handle function
*		@param src pointer to bytes table (AT format) that have been received from bluetooth
*		@retval 0 when event was handled properly
*		@rerval other check functions: @ref get_resp_name(), @ref match_rsp()
*/
int_fast8_t bl_handleResp( void * _hBL, const char * src ) {
	
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


/**	Tries to set advertisement data to bluetooth module - payload in 
*		advertising mode.
*
*		Data such as: temperature, pressure, humidity, gas concentration
*		stored in @ref BL_Data_TypeDef.local_data are converted to string and
*	 	with proper AT command are passing to bluetooth module.
*
*		@param hBL pointer to @ref BL_Data_TypeDef structure
*		@param hUART pointer to (HAL) UART_HandleTypeDef structure
*		@retval 0 if data cannot be set properly
*		@retval 1 if data was set properly
*
*		@warning when temperature will have negative value but variable is unsigned type
*						 so developer need to check MSB bit of that value and if it is 1 then
*						 that value is negative!
*/
uint32_t bl_advertUpdate( BL_Data_TypeDef * hBL, UART_HandleTypeDef * hUART ) {
	
	// Create string to set advertisment data
	// For BlueRadio module, data must contain at start: 
	// xxFF8500FFyy, where xx is: how many bytes after that will send by UART
	// yy is: those are user bytes, the amound of data: FF8500FF and yy must
	// be the same as value xx says.
	
	// we used 10 bytes of aur data + FF 85 00 FF = 14 bytes of user data
	char advert_cmd[50] ;
	char advert_data[29] ;
	uint32_t tick_ms ;				// For timeout
	
	// xx = 0E - 14 bytes of data will be send to module
	// OUR UUID  = 0AB3
	memcpy( (char *)&advert_cmd[0], (const char*)"ATSDSDLE,0,0EFF8500FF0AB3", 25 ) ;
	
	// Convert data to string
	snprintf(advert_data, 17, "%02X%02X%02X%02X%02X%02X%02X%02X",
					(uint8_t)hBL->local_data.temp_tot, (uint32_t)(hBL->local_data.temp_frac>>8), 
					(hBL->local_data.temp_frac & 0x000000FF), 
					(uint32_t)(hBL->local_data.press_sea >> 8 ),
					(hBL->local_data.press_sea & 0x000000FF),
					hBL->local_data.humidity, 
					0x00,  // for CO2 ? 
					0x00 	// for CO2 ?
	) ; //17 = 16 chars + \0
	
	// Copy data to string
	memcpy((char*)&advert_cmd[25], (const char*)&advert_data[0], 16 ) ; 
	// and add CR symbol at the and
	advert_cmd[41] = 0x0D ; 
	
	
	// Reset status in BL structure
	hBL->status = BL_NOACTION ;
	
	// Send command to module 
	HAL_UART_Transmit( hUART, (uint8_t *)&advert_cmd[0], 42 , 25 ) ;
	
	tick_ms = HAL_GetTick() ;
	// Wait for response
	do {
		bl_checkEvents( &blr_buffers, hBL ) ;
	} while( (hBL->status == BL_NOACTION) && ((HAL_GetTick() - tick_ms) <= 500) ) ;
	
	if ( hBL->status == BL_RESPONSE_OK ) {
		#ifdef _DEBUG_DEF_
			PRINTF( "\nAdvertising data set\n", 0 );
		#endif
	}
	else if ( hBL->status == BL_RESPONSE_ERR ) {
		#ifdef _DEBUG_DEF_
			PRINTF( "\nAdvertising data NOT set\n", 0 );
		#endif
		return 0 ;
	} 
	else {
		return 0 ;
	}
		
	
	// **** Sets the same data to 'scan response' payload:
	
	// Reset status in BL structure
	hBL->status = BL_NOACTION ;
	// Send command to module 
	HAL_UART_Transmit( hUART, (uint8_t *)&advert_cmd[0], 42 , 25 ) ;
	
	tick_ms = HAL_GetTick();
	// Wait for response
	do {
		bl_checkEvents( &blr_buffers, hBL ) ;
	} while( (hBL->status == BL_NOACTION) && ((HAL_GetTick() - tick_ms) <= 500) ) ;
	
	if ( hBL->status == BL_RESPONSE_OK ) {
		#ifdef _DEBUG_DEF_
			PRINTF( "\nScanResponse data set\n", 0 );
		#endif
	}
	else if ( hBL->status == BL_RESPONSE_ERR ) {
		#ifdef _DEBUG_DEF_
			PRINTF( "\nScanResponse data NOT set\n", 0 );
		#endif
		return 0 ;
	} 
	else {
		return 0 ;
	}
	
	// Reset status in BL structure
	hBL->status = BL_NOACTION ;
	
	return 1;
}


/**	Tries to turn on advertising mode in the bluetooth module
*		
*		@param hBL pointer to @ref BL_Data_TypeDef - to read status of response
*		@param hUART pointer to (HAL) UART_HandleTypeDef structure - for UART transmision
*		@param buff pointer to 8bit table with minimum 7 elements
*		@return value of enum @ref hf_stat_TypeDef type
*		@retval @ref BL_RESPONSE_OK if advertisment mode was turned on properly
*		@retval @ref BL_RESPONSE_ERR if advertisment mode wasn't turn on properly
*		@retval other value of @ref hf_stat_TypeDef type - but it means that
*		bluetooth send to MCU other message than response for latest command.
*		( if you get more error code check enum @ref hf_err_TypeDef )
*		
*		@see hf_err_TypeDef, hf_stat_TypeDef
*/
hf_stat_TypeDef bl_advertisingON(
			BL_Data_TypeDef * hBL,
			UART_HandleTypeDef * hUART,
			char * buff
		) {
	
	buff[0] = 'A' ; buff[1] = 'T' ; buff[2] = 'D' ;
	buff[3] = 'S' ; buff[4] = 'L' ; buff[5] = 'E' ;
	buff[6] = 0x0D ;
	
	// Reset status in BL structure
	hBL->status = BL_NOACTION ;
	// Send command to module 
	HAL_UART_Transmit( hUART, (uint8_t *)&buff[0], 7 , 25 ) ;
	
	// Wait for response from bluetooth module
	do {
		bl_checkEvents( &blr_buffers, hBL ) ;
	} while( hBL->status == BL_NOACTION ) ;
	
	
	if ( hBL->status == BL_RESPONSE_OK ) {
		return BL_RESPONSE_OK ;
	}	// If Advertising was turned on properly 
	else if ( hBL->status == BL_RESPONSE_ERR ) {
		return BL_RESPONSE_ERR ;
	}	// If Advertising wasn't turn on properly
	
	return hBL->status ;		// That couldn't happen
}



/** Disables ALL running commands in bluetooth module
*	
*		Should be used if you try to change some settings in module
*		
*		@param hBL pointer to @ref BLR_buff_TypeDef structure
*		@param hUART pointer to (HAL) UART_HandleTypeDef structure
*		@param buff pinter to buffer where will be AT command store - must have at least 5 elements!
*
*		@note The table poited by *buff argument must have at least 5 elements!
*/
void ble_stopallcmd( BL_Data_TypeDef * hBL, UART_HandleTypeDef * hUART, char * buff ) {
	
	uint32_t tick = HAL_GetTick() ;
	int32_t ix ;
	
	buff[0] = 'A' ; 
	buff[1] = 'T' ; 
	buff[2] = 'D' ; 
	buff[3] = 'C' ; 
	buff[4] = 0x0D ; // CR symbol
	
	// Send command to module 
	HAL_UART_Transmit( hUART, (uint8_t *)&buff[0], 5 , 20 ) ;
	
	// Discard all responses/events that should came in a few ms
	while ( (HAL_GetTick() - tick) <= 250 ) {;}
	
		
	// Reset status in BL structure
	hBL->status = BL_NOACTION ;
	
	// Clear current bluetooth UART buffers status - and get index of reserved buffer
	ix = bl_buffer_clearStatus( &blr_buffers );
	// Restart UART RX IT function:
	HAL_UART_Receive_IT(hUART, (uint8_t*)blr_buffers.buff[ix], BLR_STRUCT_BUFF_SIZE ); // start waiting for data
	
}





/**	@brief Tries to match string 'str' to standard responses / events BlueRadio module
*
*		@param *str pointer to string (must be '\0' terminated)
* 	@return -2 if str points to NULL
*		@return -1 if not matched to any of standard responses / events
* 	@return >=0 it is the index which shall be used to call proper function from the table pointers to functions
*/
static int_fast16_t match_rsp( const char * str ) {
	
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
