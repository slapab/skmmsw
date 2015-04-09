#include "stm32f4xx_hal.h"

#include "mpl3115_lib.h"
#include <math.h>
#include "main_sys.h"

/**	Initializes modelue at start of program. Develoer have to write here
*	proper settings.
* @param pointer to HAL structure of type I2C_HandleTypeDef
* @return value from enum of type HAL_StatusTypeDef
*/
HAL_StatusTypeDef mpl_init_params( I2C_HandleTypeDef * ptr_hi2c ){
	
	//uint8_t data[3] ;		// contain bytes to write at one stream ( using register autoincrement option )
	//uint8_t dev_data ;						// stores register value before changing
	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	// Switch module to STANDBY mode, after power up it is probably not necessary
	/*
	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	if ( ( _hal_func = mpl_StandbyMode( ptr_hi2c )) != HAL_OK ) 
		return _hal_func ;
	*/
	
	// define settings
	
	return _hal_func ;
}

/**	Start 'OneShot' measurement - in STANDBY mode
* @param ptr_hi2c pointer to HAL structure of type I2C_HandleTypeDef
*	@param barOralt if >1 the altimeter will be measured, if 0 then preassure will be measured
* @param overs values 0 to 7, that represent oversampling 2^overs
*/
HAL_StatusTypeDef mpl_start_OneShot( I2C_HandleTypeDef * ptr_hi2c, uint8_t overs, uint8_t barOralt ){
	
	uint8_t dev_data ;						// stores register value before changing
	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	
	// test for proper values
	if ( overs > 7 ) 
		overs = 7 ;
	if ( barOralt > 1 ) 
		barOralt = 1 ;
	
	// Set module to STANDBY mode
	//dev_data &= ~MPL_BIT_SBYB_MASK ;
	dev_data = 0;
	_hal_func = HAL_I2C_Mem_Write( ptr_hi2c, MPL_WRITE_ADDR, MPL_CTRL_REG1,
																I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 50) ;	
	if ( _hal_func != HAL_OK )
		return _hal_func ;
	

	// Start the OneShot measurement
	dev_data = MPL_BIT_OST_MASK |	MPL_BITS_OS_MASK(overs) | (barOralt << 7) ;
	_hal_func = HAL_I2C_Mem_Write( ptr_hi2c, MPL_WRITE_ADDR, MPL_CTRL_REG1,
																I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 50) ;
	
	
	return _hal_func ;
}



/**	That function tries to read data from MPL3115a2 sensor
*		At start checks if data are ready. If module is in Standby mode function
*		reads data if OST = '0', so before call this function and moudle is in
*		Standby mode you need to call ealier OneShote mesurement.
*		
*		@param hI2C pointer to HAL I2C sturcture
*		@param p_storage pointer to 'object' where data will be stored - at this function
*		it is BL_Data_TypeDef structure
*		@param p_buff pointer to uint8_t table, which size must be at least 5 elements
*		@retval 1 if data has been read and stored sucessfully
*		@retval 2 if data are not ready to read
*		@retval 0 if was problem with communication to sensor
*/
uint32_t mpl_ReadStore_Data( 
					I2C_HandleTypeDef * hI2C,
					void * p_storage,
					uint8_t * p_buff 
					) {

	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	uint_fast8_t i = 0 ;
	
	/* First check, that they are ready: 
		1) If OneShot mesurement was start, need to read  CTRL_REG1 (0x26) register and test
			if SBYB == 0 and OST == 0. 
		2) If device is in active mode need to read STATUS (0x00) register and check flags: 
			PTDR PDR TDR
	*/
	
	// check in OneShot - if data are ready
	_hal_func = HAL_I2C_Mem_Read( hI2C, MPL_WRITE_ADDR, MPL_CTRL_REG1, I2C_MEMADD_SIZE_8BIT, &p_buff[0], 1, 5) ;
	if ( _hal_func != HAL_OK ) 
		return 0 ;
	
	if ( !(p_buff[0] & MPL_BIT_SBYB_MASK) ) {
		if ( !(p_buff[0] & MPL_BIT_OST_MASK) ) {
			i = 1;
		} // OneShot mode ended ( if was ealier started )
	} // OneShot mode ( if was ealier started )
	else {
		
		_hal_func = HAL_I2C_Mem_Read( hI2C, MPL_WRITE_ADDR, MPL_STATUS_REG, I2C_MEMADD_SIZE_8BIT, &p_buff[0], 1, 10) ;
		if ( _hal_func != HAL_OK ) 
			return 0 ;
		
		if ( p_buff[0] & (MPL_BIT_TDR_MASK | MPL_BIT_PDR_MASK) ) {
				i = 1 ;
		} // If Pressure and temperature data ready
		
	} // Active mode
	
	
	// Read data when are ready
	if ( i > 0 ) {
		
		_hal_func = HAL_I2C_Mem_Read( hI2C, MPL_WRITE_ADDR, 0x01, I2C_MEMADD_SIZE_8BIT, p_buff, 5 , 30 ) ;
		
		if (  _hal_func != HAL_OK ) 
			return 0;
		
		// Convert and store pressure at sea level:
		((BL_Data_TypeDef *)p_storage)->local_data.press_sea = mpl_get_Psea( p_buff ) ;
		// Convert and store temperature:
		((BL_Data_TypeDef *)p_storage)->local_data.temp_tot = (uint8_t)p_buff[3] ;
		((BL_Data_TypeDef *)p_storage)->local_data.temp_frac = mpl_get_Tempfrac( p_buff[4] );
	
		return 1 ;
	}
	
	return 2 ;
}



HAL_StatusTypeDef mpl_setBitInReg( 
	I2C_HandleTypeDef * ptr_hi2c,
	uint8_t addr, 
	uint8_t reg,
	uint8_t mask
) {
	
	uint8_t dev_data ;						// stores register value before changing
	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	
	
	// Get current settings 
	_hal_func = HAL_I2C_Mem_Read( ptr_hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 5) ;
	if ( _hal_func != HAL_OK ) 
		return _hal_func ;
	

	dev_data |= mask ;
	// write register value
	_hal_func = HAL_I2C_Mem_Write( ptr_hi2c, addr, reg,I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 5) ;
	
	return _hal_func ;
}


/**	Set module to Standby mode - only in that mode you can change settings
*	@param ptr_hi2c pointer to HAL structure of type I2C_HandleTypeDef
* @return value from enum of type HAL_StatusTypeDef
*/
HAL_StatusTypeDef mpl_StandbyMode( I2C_HandleTypeDef * ptr_hi2c ) {
	
	uint8_t dev_data ;						// stores register value before changing
	HAL_StatusTypeDef _hal_func ;	// stores value from HAL_I2C_xxxx functions
	
	// Get current settings 
	_hal_func = HAL_I2C_Mem_Read( ptr_hi2c, MPL_WRITE_ADDR, MPL_CTRL_REG1, 
																I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 5) ;
	if ( _hal_func != HAL_OK ) 
		return _hal_func ;
	
	// Clear SBYB bit in CTRL_REG1 register
	dev_data &= ~MPL_BIT_SBYB_MASK ;
	
	// Set module to STANDBY mode
	_hal_func = HAL_I2C_Mem_Write( ptr_hi2c, MPL_WRITE_ADDR, MPL_CTRL_REG1,
																I2C_MEMADD_SIZE_8BIT, &dev_data, 1, 5) ;
	
	return _hal_func ;
}



// writing for test only !
uint32_t mpl_get_Tempfrac( uint8_t t_frac ) {
	
	uint32_t frac = 0 ;
	if ( t_frac & ( 1 << 7 ) )
		frac += 500 ;
	if ( t_frac & ( 1 << 6 ) )
		frac += 250 ;
	if ( t_frac & ( 1 << 5 ) ) 
		frac += 125 ;
	if ( t_frac & ( 1 << 4 ) ) 
		frac += 62; 	// not exacly proper value!
	
	return frac;
}


/**	Convert Barometer value in bytes to one unsigned value - that is only total value
*
*	@param data pointer to (table) first (MSB) byte with barometer value, 
*	second must be the CSB byte and third must be the LSB byte
*	@return 18bit unsigned value that represent preassure in [Pa] 
*	( that is only total value, without fractinal part). To get fractional part use
*	@ref mpl_get_Pfrac()
*/
uint32_t mpl_get_Pval( const uint8_t * data ) {
	
	uint32_t Pval = 0u;
	uint32_t tmp = 0u;
	
	// Convert 18bit unsigned component:
	// data[0] = P_MSB, data[1] = P_CSB, data[2] = P_LSB
	
	Pval = (uint32_t)data[0] ;
	Pval <<= 10 ;
	
	tmp = ((uint32_t)data[1]);
	tmp <<= 2;
	tmp |= ((((uint32_t)data[2]) & 0x000000C0u) >> 6) ;
	
	Pval |= tmp ;
	
	return Pval ;
}


/**	Get fractional value from P-LSB byte
*		
*		Fractinal value is stored at 5 and 4 bit in P-LSB byte.
*		If bit 5 is '1' means frac += 0.25 [Pa]
*		If bit 4 is '1' means frac += 0.5 [Pa]
*		
*		That function returns unsigned 8bit integer in that way:
*		bit 5 = '1' frac += 25
*		bit 4 = '1' frac += 50
*/
uint8_t mpl_get_Pfrac( uint8_t fracByte ) {
	
	uint8_t frac = 0u ;
	
	if ( fracByte & ( 1 << 5 ) ) 
		frac += 25u ;
	
	if ( fracByte & ( 1 << 4 ) ) 
		frac += 50u ;
	
	return frac ;
}

/**	That function calculate Pressure at sea level. It uses float variables
*		inside. 
*		
*		Agrumet must be pointer to table which contain information just read
*		from MPL3115a2 sensor - the byte order must be the same as registers
*		in that sensor. ([0] - P_MSB, [1] - P_CSB, [2] - P_LSB, [3] T_MSB, 
*		[4] T_LSB;
*		
*		@param *data pointer to table which contains 5 bytes of data that has
*		been read from sensor
*		@return Pressure at sea leve in format: unsinged integer (without fractinal part)
*/
uint32_t mpl_get_Psea( uint8_t * data ) {
	
	float PtABS ;
	float Psea ;
	float temp ;
	
	PtABS = (float)mpl_get_Pval( data );
	PtABS += (float)mpl_get_Pfrac( data[2] ) / 100.0f ;
	
	PtABS /= 100 ; // In [hPa]
	
	temp = (float)data[3] ;
	temp += (float)mpl_get_Tempfrac( data[4] ) / 1000.0f ;
	
	// Krakow altitude is 242 m
	// 0.0065 * 242 = 1.573
	// 273.15 + 1.573 = 274.72
	Psea = PtABS * pow( (1 - (1.573f/(274.72f+temp) )), -5.257f );
	
	return (uint32_t)Psea ;
}
