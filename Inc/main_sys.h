#ifndef _MAIN_SYS_H
#define _MAIN_SYS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

	 
typedef struct sens_param_typedef {
	uint_fast8_t temp_tot ;				/// Total part of the temperature
	uint_fast16_t temp_frac ; 		/// Fractional part of the temperature
	
	uint_fast16_t press_tot ;			/// 
	uint_fast16_t press_frac ;		///

	uint_fast8_t humidity ;				/// in %
	
} sensors_TypeDef ;


typedef struct remote_weather_typedef {
	
	char date[11] ; 							/// date for that weather data, e.g. 23.07.2015 - null terminated
	char time[6] ;								/// time for that weather data, e.g. 13:00
	
	uint_fast8_t temp ;						/// only total part of value
	uint_fast16_t pressure ;			/// atmospheric pressure in [hPa]
	uint_fast8_t humidity ; 			/// in %
	uint_fast8_t ch_rain ;				/// chece of rain in %
	
} weather_TypeDef ;


typedef struct bluetooth_connection_typedef {
	
	char rem_mac[13] ;						/// connected remote mac address
	uint_fast8_t conn_handler ;		/// connection handler
	uint_fast16_t srv_atr_h ; 		/// remote service attribute handle 
	uint_fast16_t srv_last_h ; 		/// attribute handle of the last attribute in the service
	uint_fast16_t srv_uuid ;			/// service UUID
	
	uint_fast16_t char_attr_h[10] ; 	/// characteristic value attribute handle
	uint_fast16_t char_uuid[10] ;			/// uuid of the characteristic
	
	// Descirptors of characteristic: 
	uint_fast16_t char_descr_h[10] ;		/// characteristic descriptor attribute handle
	uint_fast16_t char_descr_uudi[10] ; /// uuid of the characteristic descriptor
		
} blueConn_TypeDef ;


typedef enum bl_hf_error_code { GATT_DONE_ERR /* to do */ } hf_err_TypeDef ;
typedef enum bl_hf_status { 
	BL_NOACT, 
	BL_ERR, 
	BL_VAL_CHANGED,
	BL_VAL_READ
	/* to do */
} hf_stat_TypeDef ;


typedef struct bluetooth_data_typedef {
	
	sensors_TypeDef local_data ;
	weather_TypeDef remote_data ;
	
	uint_fast8_t hours ;					/// Current hour - valid only when was bluetooth connection established
	uint_fast8_t min ;						/// Current minutes - valid only when was bluetooth connectio established
	
	blueConn_TypeDef conn ;				/// Bluetooth connection parameters
	
	hf_stat_TypeDef status ;			/// The value is different than BL_NOACT if something was changed
	/* to do, more info about status changed, e.g on which ch-stic handle are data changed */
	hf_err_TypeDef error ;				/// Error code/source
	
} BL_Data_TypeDef;


#ifdef __cplusplus
}
#endif
#endif
