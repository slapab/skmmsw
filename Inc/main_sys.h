#ifndef _MAIN_SYS_H
#define _MAIN_SYS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

// For debugging only
	 #include <stdio.h>
	 #define PRINTF(_txt_, _d_) printf("\n%s : %d\n", (_txt_), (_d_))

	 
	 
typedef struct sens_param_typedef {
	volatile uint_fast8_t temp_tot ;				/// Total part of the temperature
	volatile uint_fast16_t temp_frac ; 		/// Fractional part of the temperature
	
	volatile uint_fast16_t press_tot ;			/// 
	volatile uint_fast16_t press_frac ;		///

	volatile uint_fast8_t humidity ;				/// in %
	
} sensors_TypeDef ;


typedef struct remote_weather_typedef {
	
	volatile char date[11] ; 							/// date for that weather data, e.g. 23.07.2015 - null terminated
	volatile char time[6] ;								/// time for that weather data, e.g. 13:00
	
	volatile uint_fast8_t temp ;						/// only total part of value
	volatile uint_fast16_t pressure ;			/// atmospheric pressure in [hPa]
	volatile uint_fast8_t humidity ; 			/// in %
	volatile uint_fast8_t ch_rain ;				/// chece of rain in %
	
} weather_TypeDef ;


typedef struct bluetooth_connection_typedef {
	
	volatile char rem_mac[12] ;						/// connected remote mac address
	volatile uint_fast8_t conn_handler ;		/// connection handler
	volatile uint_fast16_t srv_atr_h ; 		/// remote service attribute handle 
	volatile uint_fast16_t srv_last_h ; 		/// attribute handle of the last attribute in the service
	volatile uint_fast16_t srv_uuid ;			/// service UUID
	
	volatile uint_fast16_t char_attr_h[10] ; 	/// characteristic value attribute handle
	volatile uint_fast16_t char_uuid[10] ;			/// uuid of the characteristic
	
	// Descirptors of characteristic: 
	volatile 	uint_fast16_t char_descr_h[10] ;		/// characteristic descriptor attribute handle
	volatile uint_fast16_t char_descr_uudi[10] ; /// uuid of the characteristic descriptor
		
} blueConn_TypeDef ;


typedef enum bl_hf_error_code { 
	/* START BLUETOOTH AT - ERROR CODE
		 these enums values must start at 1 and end at 12 - as in the documentation */
	BLR_INVALID_PARAMS=1,
	BLR_INVALID_ROLE=2,
	BLR_INVALID_STATE=3,
	BLR_INVALID_PASSWD=4,
	BLR_INVALID_CONN_HANDLE=5,
	BLR_CONF_LOCKED=6,
	BLR_LIST_ERROR=7,
	BLR_HARDWARE_ERROR=8,
	BLR_NO_ADDR_STORED=9,
	BLR_BLUETOOTH_ERROR=10,
	BLR_MEM_ALLOC_ERROR=11,
	BLR_GATT_REQ_PENDING=12,
	/* STOP BLUETOOTH AT - ERROR CODE */
	
	BL_CONNECT_HANDLE_ERR,			/// Error when tried to read connection handle
	BL_CONNECT_ADDR_ERR,				/// Error when tried to read address of connected device
	/*  to do */
	GATT_DONE_ERR
} hf_err_TypeDef ;


typedef enum bl_hf_status { 
	BL_NOACTION, 
	BL_RESPONSE_OK,
	BL_RESPONSE_ERR,
	
	BL_EV_CONNECT,
	
	LIB_HF_ERR,						// this is library internal error - is set if it occur in one of bl_hf_xxxx() functions
	BL_VAL_CHANGED,
	BL_VAL_READ
	/* to do */
} hf_stat_TypeDef ;


typedef struct bluetooth_data_typedef {
	
	sensors_TypeDef local_data ;
	weather_TypeDef remote_data ;
	
	volatile uint_fast8_t hours ;		/// Current hour - valid only when was bluetooth connection established
	volatile uint_fast8_t min ;			/// Current minutes - valid only when was bluetooth connectio established
	
	blueConn_TypeDef conn ;				/// Bluetooth connection parameters
	
	hf_stat_TypeDef status ;			/// The value is different than BL_NOACT if something was changed
	/* to do, more info about status changed, e.g on which ch-stic handle are data changed */
	hf_err_TypeDef error ;				/// Error code/source
	
} BL_Data_TypeDef;


#ifdef __cplusplus
}
#endif
#endif
