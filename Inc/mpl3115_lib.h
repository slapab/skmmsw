#ifndef _MPL3115_LIB_H
#define _MPL3115_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f4xx.h"
	
	
#define MPL_WRITE_ADDR 0xC0
#define MPL_READ_ADDR	0xC1
	

// Registers
#define MPL_SYSMOD_REG 0x11
#define MPL_CTRL_REG1 0x26
// Masks	
#define MPL_BIT_SBYB_MASK 0x01

#define MPL_BIT_OST_MASK 0x02
#define MPL_BITS_OS_MASK(_VAL_) (uint8_t)((_VAL_) << 3)
#define MPL_BIT_ALT_MASK 0x80
	
// Functions: 
HAL_StatusTypeDef mpl_StandbyMode( I2C_HandleTypeDef * ptr_hi2c ) ;

HAL_StatusTypeDef mpl_start_OneShot( I2C_HandleTypeDef * ptr_hi2c, uint8_t overs, uint8_t barOralt );

HAL_StatusTypeDef mpl_setBitInReg( 
	I2C_HandleTypeDef * ptr_hi2c,
	uint8_t addr, 
	uint8_t reg,
	uint8_t mask
);

uint16_t mpl_get_Tempfrac( uint8_t t_frac ) ;


#ifdef __cplusplus
}
#endif /* _MPL3115_LIB_H */
	
#endif
