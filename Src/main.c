#include "stm32f4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"

#include <stdio.h>
#include <string.h>

#include "mpl3115_lib.h"
#include "main_sys.h"
#include "blueradio_lib.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int32_t bl_read_cmd( char * string, const size_t max_size ) ;
void delay_ms( const uint32_t delay );



extern UART_HandleTypeDef huart5;
extern I2C_HandleTypeDef hi2c3;
extern BLR_buff_TypeDef blr_buffers ;		// Bluetooth UART buffers stearing handle 

// buffers for Bluetooth UART data
char bl_buff[BLR_STRUCT_BUFF_NO][BLR_STRUCT_BUFF_SIZE] ;	

// SENSORS AND BLUETOOTH DATA
BL_Data_TypeDef weather_data ;

// Time counter - for sensors checking
struct time_sens_TypeDef sensors_timing = { .timer_sensors = 0,
																						.is_converting = 0 } ;


	int a = 0 ;
	
	size_t size_string = 3 ;
	char send_string[10] ;
	// Get ID from I2C barometer sensor: 
	uint8_t i2c_rcv_data[10] ;

int main(void)
{

	uint32_t utmp ;
	int32_t itmp ;

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

	
	// Init bluetooth UART buffers structure
	bl_buffers_init( &blr_buffers, bl_buff[0], BLR_STRUCT_BUFF_NO, BLR_STRUCT_BUFF_SIZE) ;
	
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART5_Init();
	MX_I2C_Init();

	// Let know UART callbackcplt function which buffer was reserved at start
	itmp = bl_buffers_reserve( &blr_buffers ) ;
	HAL_UART_Receive_IT(&huart5, (uint8_t*)blr_buffers.buff[itmp], BLR_STRUCT_BUFF_SIZE ) ;
	
	
	// Send char over 'semihosting' - initialization( without it, sometimes first character is missing )
	printf( " START \n\n" ) ;

	
	const size_t user_cmd_size = 80 ;
	char user_cmd[user_cmd_size] ;
	size_t rcv_cmd_size ;
	
	
	//delay_ms(500) ;
	
	// Set advertising payload to iBeacon format: 
	// iBeacon prefix is: 02 01 06 1A FF 4C 00 02 15
	// Set UUID from nRF iBeacon app ( it suggest that for other iBeacon ): 
	// 01 12 23 34 - 45 56 - 67 78 - 89 9A - AB BC CD DE EF F0
	// set major: 00 0F
	// set minor: 00 03
	// set proximity, at start set: C5
	/*
	const char *src_advr_data = "ATSDSDLE,0,1AFF4C0002150112233445566778899AABBCCDDEEFF0000F0003C5" ;
	strcpy( &user_cmd[0], &src_advr_data[0]) ;
	user_cmd[65] = 0x0D ;
	// set advertising data:
	size_string = 66 ;
	usart_sendString( user_cmd, size_string ) ;
	
	delay_ms(500) ;
	
	// set 'device state' typying for iBeacon: 
	strcpy( &user_cmd[0], (const char*)"ATSDSLE,0,3,7") ;
	user_cmd[13] = 0x0D ;
	size_string = 14 ;
	usart_sendString( user_cmd, size_string ) ;
	
	*/
	// try nRF UART service ( nordic CID: 0x0059 )
	// not working: 
	/*
	const char *src_advr_data = "ATSDSDLE,0,14FF5900FF6E400001B5A3F393E0A9E50E24DCCA9E" ;
	strcpy( &user_cmd[0], &src_advr_data[0]) ;
	user_cmd[53] = 0x0D ;
	// set advertising data:
	size_string = 54 ;
	usart_sendString( user_cmd, size_string ) ;
	*/
	/*
	delay_ms(500) ;
	
	// enable advertising: 
	strcpy( &user_cmd[0], (const char*)"ATDSLE") ;
	user_cmd[6] = 0x0D ;
	// set advertising data:
	size_string = 7 ;
	usart_sendString( user_cmd, size_string ) ;
	
	
	
	*/



	// stop all running commands in bluetooth module
	ble_stopallcmd( &weather_data, &huart5, &send_string[0] ) ;
	
	// Get first data - pressure and temperature 
	// next try to set advertising data in module
	if ( mpl_start_OneShot(&hi2c3, 5, 0) == HAL_OK ) {
		delay_ms(600) ;
		if ( 1 == mpl_ReadStore_Data(&hi2c3, &weather_data, &i2c_rcv_data[0] ) ) {
			
			printf( "\nT = %d.%i [C]\n", weather_data.local_data.temp_tot, weather_data.local_data.temp_frac  ) ;
			printf("\nPreasure = %i [hPa]\n",  weather_data.local_data.press_sea ) ;
			
			// Start advertising:
			bl_advertUpdate( &weather_data, &huart5 ) ;
			
			// Try to start advertising mode
			if ( BL_RESPONSE_OK == bl_advertisingON( &weather_data, &huart5, &send_string[0] ) ) {
				#ifdef _DEBUG_PRINTF_
				printf("\nAdvertising MODE has been turned on.\n" );
				#endif
			} 
			else {
				#ifdef _DEBUG_PRINTF_
				printf("\nAdvertising MODE has NOT been turned on.\n" );
				#endif
				
			}
			
			
		} else {
			#ifdef _DEBUG_PRINTF_
				printf("\nProblem z odczytem\n" );
			#endif
		}
	}
	
	
	
  while (1)
  {
		
		// It has to be called becouse it is part of UART IT reading system
		bl_checkEvents( &blr_buffers, &weather_data ) ;
		
		// start reading value from sensors
		if ( sensors_timing.timer_sensors >= SENSORS_CHECK_TIME ) {
			
			// Start OneShot mesurement
			if ( sensors_timing.is_converting == 0 ) {
				sensors_timing.is_converting = 1 ;
				
				// Start reading temp and pressure
				if ( HAL_OK != mpl_start_OneShot(&hi2c3, 5, 0) ) {
					sensors_timing.timer_sensors = 0 ;
					sensors_timing.is_converting = 0 ;
				} // Error while connecting with MPL3115A2 sensor
				
			}
			
			
			// Need to wait while data are prepared in sensors
			
			// At the end of that if statement the LCD can be updated
			if ( (sensors_timing.timer_sensors >= (SENSORS_CHECK_TIME + 600)) && 
					 (sensors_timing.is_converting == 1 ) 
				 ) {	 
				sensors_timing.timer_sensors = 0 ;
				sensors_timing.is_converting = 0 ;
				
				// Read data from MPL3115A2 sensor
				if ( 1 == mpl_ReadStore_Data(&hi2c3, &weather_data, &i2c_rcv_data[0] ) ) {
				
				#ifdef _DEBUG_PRINTF_
					printf( "\nT = %d.%i [C]\n", weather_data.local_data.temp_tot, weather_data.local_data.temp_frac  ) ;
					printf("\nPreasure = %i [hPa]\n",  weather_data.local_data.press_sea ) ;	
				#endif
					
					// Change values in advertisig payload: 
					bl_advertUpdate( &weather_data, &huart5 ) ;
					
					
					/// ***** HERE CAN BE CODE TO UPDATE LCD ***** ///
					/// ***** data are stored in (BL_Data_TypeDef) weather_data vairable
					
					
				} // If read and store in structure has been done properly
			
				
				
			}	// After waiting for sensors
			
		} // IF - start reading value
		
		
		/* !!!!!!! OD TEGO MIEJSCA NA RAZIE NIE RUSZAC
							 DO DEBUGOWNIA POTRZEBNE
			 !!!!!!!
		*/
		
		if ( a == 1) {
			a = 0;
			
			if ( (rcv_cmd_size = bl_read_cmd( &user_cmd[0], user_cmd_size ) ) > 0 ) {
				//printf("\n %s \n ", user_cmd ) ;
				usart_sendString( user_cmd, rcv_cmd_size ) ;
			} else  {
				printf( "\n Bad command \n " ) ;
			}
		} else if ( a == 2 ) {
			// Try read prassure and temperature
			
			// Start 'OneShot' measurement
			if ( mpl_start_OneShot(&hi2c3, 5, 0) == HAL_OK ) {
					
					delay_ms(1000) ; // about 512 ms when oversampling is set to 128
					i2c_rcv_data[6] = i2c_rcv_data[7] = 0xFF;
					if ( HAL_I2C_Mem_Read(&hi2c3, MPL_WRITE_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &i2c_rcv_data[0], 8 , 20 ) 
						== HAL_OK )  {
						printf( "\nData read successfully.\n") ;
						printf( "\nT = %d.%i [C]\n", i2c_rcv_data[4], mpl_get_Tempfrac(i2c_rcv_data[5]) ) ;
						// print preassure
						printf("\nPreasure = %i [hPa]\n", mpl_get_Psea( &i2c_rcv_data[1] )) ;

							
						} else 
						printf( "\nError while tried to read data from IC\n");
				
			} else {
				printf( "\nError while tried to start 'OneShot' measurement \n");
			}
			
		}
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}

/* USER CODE BEGIN 4 */


int32_t bl_read_cmd( char * string, const size_t max_size ) {
	
	// User should defined here 'end-of-command' characeter
	const char _end_char = 0x0D ;
	size_t i = 0 ;
	size_t j ;
	
	// Flush buffer before prompt user to get command
	while (ITM_CheckChar() == 1) 
		ITM_ReceiveChar() ;
	
	printf("\nEnter the command, at the end press Enter\n" );
	
	while ( 1 ) {
		
		if ( ITM_CheckChar() == 1  ) {
				string[i] = (char)ITM_ReceiveChar() ;
			
				// Check if it is user-defined end of command character
				if ( _end_char == string[i] ) {
					// whole command received, flush rest of chars in buffer
					while (ITM_CheckChar() == 1) 
						 ITM_ReceiveChar() ;
					
					// Return size of received characters ( in buffer: 'string' there is no \0 )
					return i+1 ;
				}
				else if ( (++i) >= (max_size - 1) ) {
					// received maximum chars and it means that table: 'string' not contain whole 'command'
					
					printf("\n Command to long. Exiting... \n " ) ;
					
					// Flush the characters from buffer
					 while (ITM_CheckChar() == 1) 
						 ITM_ReceiveChar() ;
					
					return -1 ; 			
				} else {
					
					// Inform user what he entered so far
					ITM_SendChar( (uint32_t)'\n') ;
					for ( j = 0 ; j < i ; j++ )					
						ITM_SendChar( (uint32_t)string[j] ) ;
				}
		}
		
	}
	// Timeout 
	return -2 ;
}



void delay_ms( const uint32_t delay ) {
	uint32_t start = HAL_GetTick() ;
	
	while( (uint32_t)(HAL_GetTick() - start) <= delay ) ;
	
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
