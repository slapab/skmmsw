#ifndef _MPL3115_LIB_H
#define _MPL3115_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f4xx.h"
	
	
#define MPL_WRITE_ADDR 0xC0
#define MPL_READ_ADDR	0xC1
	

// Registers
#define MPL_STATUS_REG 0x00
#define MPL_SYSMOD_REG 0x11
#define MPL_CTRL_REG1 0x26
// Masks	
#define MPL_BIT_SBYB_MASK 0x01

#define MPL_BIT_OST_MASK 0x02
#define MPL_BITS_OS_MASK(_VAL_) (uint8_t)((_VAL_) << 3)
#define MPL_BIT_ALT_MASK 0x80
	
#define MPL_BIT_TDR_MASK 0x02
#define MPL_BIT_PDR_MASK 0x04
#define MPL_BIT_PTDR_MASK 0x8
	
// Functions: 
HAL_StatusTypeDef mpl_StandbyMode( I2C_HandleTypeDef * ptr_hi2c ) ;

HAL_StatusTypeDef mpl_start_OneShot( I2C_HandleTypeDef * ptr_hi2c, uint8_t overs, uint8_t barOralt );

HAL_StatusTypeDef mpl_setBitInReg( 
	I2C_HandleTypeDef * ptr_hi2c,
	uint8_t addr, 
	uint8_t reg,
	uint8_t mask
);




uint32_t mpl_ReadStore_Data( 
					I2C_HandleTypeDef * hI2C,
					void * p_storage,
					uint8_t * p_buff 
					) ;
					
// These functions should be static functions
uint32_t mpl_get_Tempfrac( uint8_t t_frac ) ;
uint32_t mpl_get_Pval( const uint8_t * data ) ;
uint8_t mpl_get_Pfrac( uint8_t fracByte ) ;
uint32_t mpl_get_Psea( uint8_t * data ) ;

#ifdef __cplusplus
}
#endif /* _MPL3115_LIB_H */
	
#endif
