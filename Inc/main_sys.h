#ifndef _MAIN_SYS_H
#define _MAIN_SYS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <signal.h>

	// For debugging only
	 #define _DEBUG_PRINTF_ 1
	 #include <stdio.h>
	 #define PRINTF(_txt_, _d_) printf( "\n%s : %d\n", (_txt_), (_d_) )
	 #define PRINTFSTR( _str_ ) printf( "%s\n", (_str_) )



/// in [ms] time between next two readings
#define SENSORS_CHECK_TIME 5000
	
/// in [ms] time after which scanning mode will be turn on
/// But should not be the same or less than scanning duration
#define BLUETOOTH_SCAN_ITNERVAL 8000
	 

#define BL_UUID_SIZE 32 /// how many chars( 4 or 32 ) UUID has
#define BL_CHAR_LEN (BL_UUID_SIZE+1) /// 32-chars for characteristic UUID + \0 character
#define BL_CHAR_NO 3	/// number of characteristics to read
	 

/** That structure is using to reading data from sensors
*		repetitively but with breaks
*/
struct time_sens_TypeDef {
	uint32_t timer_sensors ;
	sig_atomic_t is_converting ;
};


/** That structure is using to turning on scanning (LE mode) in 
*		bluetooth module repetitively but with breaks
*		
*		block variable is using for stoping counting when scanning
*		is turned on. That variable shoud be reset when scanning stops.
*/
struct blscan_mode_typedef {
	uint32_t timer_scanmode ;
	sig_atomic_t block ;
} ;


	 
typedef struct sens_param_typedef {
	volatile uint8_t temp_tot ;						/// Total part of the temperature
	volatile uint32_t temp_frac ; 				/// Fractional part of the temperature
	
	volatile uint32_t press_sea ;					/// Pressure calculated at sea level
	//volatile uint_fast16_t press_frac ;		///

	volatile uint_fast8_t humidity ;				/// in %
	
} sensors_TypeDef ;


typedef struct remote_weather_typedef {
	
	volatile char date[11] ; 							/// date for that weather data, e.g. 23.07.2015 - null terminated
	volatile uint8_t descr_id;						/// wather description - index for look-up table
	
	volatile char rssi[5] ;								/// [string] rssi value -127 to +20 - \0 terminated
	
	
	//volatile uint_fast8_t temp ;					/// only total part of value
	//volatile uint_fast16_t pressure ;			/// atmospheric pressure in [hPa]
	//volatile uint_fast8_t humidity ; 			/// in %
	//volatile uint_fast8_t ch_rain ;				/// chence of rain in %
} weather_TypeDef ;


typedef struct bluetooth_connection_typedef {
	
	volatile char rem_mac[12] ;						/// connected remote mac address
	volatile char conn_handler[3] ;				/// connection handler - \0 terminated
	volatile uint_fast16_t srv_atr_h ; 		/// remote service attribute handle 
	volatile uint_fast16_t srv_last_h ; 	/// attribute handle of the last attribute in the service
	volatile uint_fast16_t srv_uuid ;			/// service UUID
	
	volatile uint16_t char_attr_h[10][6] ; 	/// characteristic value attribute handle must be string ( \0 terminate )
	volatile const char char_uuid[BL_CHAR_NO][BL_CHAR_LEN] ;			/// uuids of characteristic
	
	// Descirptors of characteristic:
	volatile 	uint_fast16_t char_descr_h[10] ;	 /// characteristic descriptor attribute handle
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
	
	BL_NO_ERRRORS,
	
	BL_CONNECT_HANDLE_ERR,			/// Error when tried to read connection handle
	BL_CONNECT_ADDR_ERR,				/// Error when tried to read address of connected device
	/*  to do */
	GATT_DONE_ERR
} hf_err_TypeDef ;


typedef enum bl_hf_status { 
	BL_NOACTION, 
	BL_RESPONSE_OK,
	BL_RESPONSE_ERR,
	
	BL_EV_CONNECT,				/// If set means that other device has been connected to module
	BL_EV_DISCONNECT,			/// Indicates that received disconnect event
	BL_EV_DONE,
	BL_EV_GATT_DC,				/// When ch-stic was found
	BL_EV_GATT_DONE,			/// Send after GATT task in module was ended

	LIB_HF_ERR,						/// This is library internal error - is set if it occur in one of bl_hf_xxxx() functions
	BL_TIMEOUT, 					/// If not receive response durning window time
	BL_VAL_CHANGED,
	BL_VAL_READ
	/* to do */
} hf_stat_TypeDef ;


typedef struct bluetooth_data_typedef {
	
	sensors_TypeDef local_data ;
	weather_TypeDef remote_data ;
	
	volatile sig_atomic_t hour ;				/// Current hour - valid only when was bluetooth connection established
	volatile sig_atomic_t min ;					/// Current minutes - valid only when was bluetooth connectio established
	
	blueConn_TypeDef conn ;					/// Bluetooth connection parameters
	
	volatile hf_stat_TypeDef status ;			/// The value is different than BL_NOACT if something was changed
	/* to do, more info about status changed, e.g on which ch-stic handle are data changed */
	volatile hf_err_TypeDef error ;				/// Error code/source
	
	volatile uint8_t ev_gatt_doneNO ;			/// Stores 'function' ended id ( e.g.  ATGDC )
	volatile uint8_t ev_gatt_doneERR; 		/// Stores error returned by GATT_DONE event
	
} BL_Data_TypeDef;


typedef enum sys_conn_TypeDef {
	DISCONNECTED, 
	CONNECTED,
	/// Indicates when module is connected and characteristic
	/// from device was identified succesfully
	CONNECTED_WITH_DATA 
	
} SYS_CONN_TypeDef ;

#ifdef __cplusplus
}
#endif
#endif
