/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014 
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "GUI.h"
#include "usart.h"
#include "i2c.h"
#include "main_sys.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile GUI_TIMER_TIME OS_TimeMS;
extern LTDC_HandleTypeDef hltdc;
extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef huart5;

// in main.c
extern struct time_sens_TypeDef sensors_timing;

// in main.c 
extern struct blscan_mode_typedef blscan_mode;

// in main.c - for increment timing
extern BL_Data_TypeDef weather_data;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/


static uint32_t Hm_ms = 0;
static uint32_t Hm_s = 0;

/**
* @brief   This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Update the LocalTime by adding 1 ms each SysTick interrupt */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  OS_TimeMS++;

  // Count interval for sensors reading
  ++sensors_timing.timer_sensors;

  // Count interval for bluetooth scan start task
  ++blscan_mode.timer_scanmode;


  // Keep time alive :D :
  ++Hm_ms; // cound ms
  if (Hm_ms >= 1000) {
	  ++Hm_s;		// count seconds
	  if (Hm_s >= 60) {
		  ++weather_data.min; // count minutes
		  if (weather_data.min >= 60) {
			  ++weather_data.hour; // count hours
			  if (weather_data.hour >= 24) {
				  weather_data.hour = 0;
			  } // check hours
			  weather_data.min = 0;
		  } // check minutes
		  Hm_s = 0;
	  } // check s
	  Hm_ms = 0;
  } // Check ms
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hltdc);
}

void UART5_IRQHandler(void)
{

	//HAL_UART_IRQHandler(&huart5); 
	HAL_UART_IRQHandler_mod(&huart5);

	// Obsluzyc odebrane dane
	// usart_readCharacter(&huart5, huart5.Instance->DR) ;
}




void I2C3_EV_IRQHandler(void) {
	HAL_I2C_EV_IRQHandler(&hi2c3);
}

void I2C3_ER_IRQHandler(void) {
	HAL_I2C_ER_IRQHandler(&hi2c3);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
