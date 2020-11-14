
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include "lcd/lcd5110.h"
#include "logo.h"
#include "game.h"
#include "tone.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
display_5110 display = {
        .hspi = &hspi1,
        .RESET_BASE = LED_RST_GPIO_Port,
        .RESET_PIN = LED_RST_Pin,
        .COM_DAT_BASE = LED_DC_GPIO_Port,
        .COM_DAT_PIN = LED_DC_Pin,
        .SCE_BASE = LED_SCE_GPIO_Port,
        .SCE_PIN = LED_SCE_Pin
};
int snake[SNAKE_MAX_SIZE][2] = { 0 };
int snakeLength = 4;
int snakeDirection = SNAKE_DIRECTION_LEFT;
int food[2] = { 4, 4 };
int snakeSpeed = 100; // ms between movement frames
int mode = MODE_START_SCREEN;
int gameState = GAME_STATE_IN_PROGRESS;

void initializeGame() {
    srand(__HAL_TIM_GET_COUNTER(&htim1));
    snakeLength = 4;
    snakeDirection = SNAKE_DIRECTION_LEFT;
    food[0] = 4;
    food[1] = 4;

    for (int i = 0; i < SNAKE_MAX_SIZE; i++) {
        snake[i][0] = 0;
        snake[i][1] = 0;
    }

    snake[0][0] = 17;
    snake[0][1] = 8;

    snake[1][0] = 16;
    snake[1][1] = 8;

    snake[2][0] = 15;
    snake[2][1] = 8;

    snake[3][0] = 14;
    snake[3][1] = 8;
}

void drawBorders() {
    draw_rect(&display, 0, 0, 84, 48);
}

void drawSnake(int snake[][2], int length) {
    for (int i = 0; i < length; i++) {
        draw_rect(&display, BLOCK_SIZE * snake[i][0], BLOCK_SIZE * snake[i][1], BLOCK_SIZE, BLOCK_SIZE);
    }
}

void drawFood(int food[2]) {
    draw_filled_rect(&display, BLOCK_SIZE * food[0], BLOCK_SIZE * food[1], BLOCK_SIZE, BLOCK_SIZE);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);

  // Setup display
  uint8_t lcd_buffer[528] = { 0x00 };
  memcpy(lcd_buffer, snake_start_Bitmap, LCD_BUFFER_SIZE);
  display.buf = lcd_buffer;
  init_display(&display);
  print_string(&display, "Play: F button", 14, 0, 40);
  refresh_display(&display);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
        if (mode == MODE_START_SCREEN && HAL_GPIO_ReadPin(BTN_CTRL_GPIO_Port, BTN_CTRL_Pin) == GPIO_PIN_RESET) { // step into game
            mode = MODE_GAME_SCREEN;
            gameState = GAME_STATE_IN_PROGRESS;
            initializeGame();

            int doEats = false;
            int doBitesItself = false;
            int doCollidesWithWall = false;

            playNokiaTune(&htim1, SPKR_GPIO_Port, SPKR_Pin);

            while (true) {
                clear_display(&display);
                snakeDirection = determineDirection(snakeDirection);
                doEats = isSnakeEating(snake, snakeLength, food);
                doBitesItself = isSnakeBitesItself(snake, snakeLength);
                doCollidesWithWall = isSnakeCollidesWithWall(snake, snakeLength, snakeDirection);

                if (doEats) {
                    snakeLength++;
                    playFoodCatchSound(&htim1, SPKR_GPIO_Port, SPKR_Pin);
                    moveFood(snake, snakeLength, food);
                }

                if (doBitesItself || doCollidesWithWall) {
                    gameState = GAME_STATE_GAMEOVER;
                    mode = MODE_RESULT_SCREEN;
                    playGameOverSound(&htim1, SPKR_GPIO_Port, SPKR_Pin);
                    break;
                }

                if (snakeLength == SNAKE_MAX_SIZE) {
                    gameState = GAME_STATE_WIN;
                    mode = MODE_RESULT_SCREEN;
                    playWinnerSound(&htim1, SPKR_GPIO_Port, SPKR_Pin);
                    break;
                }

                moveSnake(snake, snakeLength, snakeDirection, doEats);
                drawBorders();
                drawSnake(snake, snakeLength);
                drawFood(food);
                refresh_display(&display);
                HAL_Delay(snakeSpeed);
            }
        }

        if (mode == MODE_RESULT_SCREEN && gameState == GAME_STATE_GAMEOVER) {
            clear_display(&display);
            print_string(&display, "Game Over :(", 12, 6, 0);

            char length[3];
            itoa(snakeLength, length, 10);
            char message[14] = "Length was ";
            strcat(message, length);
            print_string(&display, message, strlen(message), 4, 12);

            print_string(&display, "New game: F", 11, 8, 36);
            refresh_display(&display);
        }

        if (mode == MODE_RESULT_SCREEN && gameState == GAME_STATE_WIN) {
            clear_display(&display);
            print_string(&display, "You won!", 8, 16, 0);

            char length[3];
            itoa(snakeLength, length, 10);
            char message[14] = "Length was ";
            strcat(message, length);
            print_string(&display, message, strlen(message), 4, 12);

            print_string(&display, "New game: F", 11, 8, 36);
            refresh_display(&display);
        }

        if (mode == MODE_RESULT_SCREEN && HAL_GPIO_ReadPin(BTN_CTRL_GPIO_Port, BTN_CTRL_Pin) == GPIO_PIN_RESET) {
            mode = MODE_START_SCREEN;
        }

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

    }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 64-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RST_Pin|LED_DC_Pin|LED_SCE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPKR_GPIO_Port, SPKR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BTN_CTRL_Pin BTN_UP_Pin */
  GPIO_InitStruct.Pin = BTN_CTRL_Pin|BTN_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_RIGHT_Pin */
  GPIO_InitStruct.Pin = BTN_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_RIGHT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_LEFT_Pin BTN_DOWN_Pin */
  GPIO_InitStruct.Pin = BTN_LEFT_Pin|BTN_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RST_Pin LED_DC_Pin LED_SCE_Pin */
  GPIO_InitStruct.Pin = LED_RST_Pin|LED_DC_Pin|LED_SCE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPKR_Pin */
  GPIO_InitStruct.Pin = SPKR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPKR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
