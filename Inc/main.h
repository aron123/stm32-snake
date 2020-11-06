/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BTN_CTRL_Pin GPIO_PIN_10
#define BTN_CTRL_GPIO_Port GPIOB
#define BTN_RIGHT_Pin GPIO_PIN_7
#define BTN_RIGHT_GPIO_Port GPIOC
#define BTN_LEFT_Pin GPIO_PIN_8
#define BTN_LEFT_GPIO_Port GPIOA
#define BTN_DOWN_Pin GPIO_PIN_9
#define BTN_DOWN_GPIO_Port GPIOA
#define LED_RST_Pin GPIO_PIN_10
#define LED_RST_GPIO_Port GPIOA
#define LED_DC_Pin GPIO_PIN_11
#define LED_DC_GPIO_Port GPIOA
#define LED_SCE_Pin GPIO_PIN_12
#define LED_SCE_GPIO_Port GPIOA
#define SPKR_Pin GPIO_PIN_4
#define SPKR_GPIO_Port GPIOB
#define BTN_UP_Pin GPIO_PIN_6
#define BTN_UP_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

#define true 1
#define false 0

#define BLOCK_SIZE 3
#define GAME_WIDTH 84/BLOCK_SIZE
#define GAME_HEIGHT 48/BLOCK_SIZE
#define SNAKE_MAX_SIZE 64

#define SNAKE_DIRECTION_UP 1
#define SNAKE_DIRECTION_RIGHT 2
#define SNAKE_DIRECTION_DOWN 3
#define SNAKE_DIRECTION_LEFT 4

#define MODE_START_SCREEN 1
#define MODE_GAME_SCREEN 2
#define MODE_RESULT_SCREEN 3

#define GAME_STATE_IN_PROGRESS 1
#define GAME_STATE_WIN 2
#define GAME_STATE_GAMEOVER 3

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
