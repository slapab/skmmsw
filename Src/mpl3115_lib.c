#include "stm32f4xx_hal.h"

#include "mpl3115_lib.h"


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
uint16_t mpl_get_Tempfrac( uint8_t t_frac ) {
	
	uint16_t frac = 0 ;
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

