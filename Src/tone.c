/*
 * tone.c
 *
 *  Created on: 2020. nov. 6.
 *      Author: aronk
 */

#include <stdint.h>
#include "stm32f1xx_hal.h"

void delay_microseconds(TIM_HandleTypeDef* timer, uint16_t us) {
    __HAL_TIM_SET_COUNTER(timer, 0);
    while (__HAL_TIM_GET_COUNTER(timer) < us)
        ;
}

void tone(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin, int frequency, int duration) {
    unsigned long startTime = HAL_GetTick();
    unsigned long halfPeriod = 1000000L / frequency / 2;

    while (HAL_GetTick() - startTime < duration) {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        delay_microseconds(timer, halfPeriod);
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        delay_microseconds(timer, halfPeriod);
    }
}



/* Public API */

void playNokiaTune(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin) {
    tone(timer, port, pin, 1318, 112);
    tone(timer, port, pin, 1174, 120);
    tone(timer, port, pin, 739, 219);
    tone(timer, port, pin, 830, 222);
    tone(timer, port, pin, 1108, 103);
    tone(timer, port, pin, 987, 126);
    tone(timer, port, pin, 587, 248);
    tone(timer, port, pin, 659, 265);
    tone(timer, port, pin, 987, 123);
    tone(timer, port, pin, 880, 151);
    tone(timer, port, pin, 554, 164);
    tone(timer, port, pin, 554, 164);
    tone(timer, port, pin, 659, 164);
    tone(timer, port, pin, 759, 164);
    tone(timer, port, pin, 880, 460);
}

void playFoodCatchSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin) {
    tone(timer, port, pin, 400, 30);
    tone(timer, port, pin, 500, 30);
}

void playGameOverSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin) {
    tone(timer, port, pin, 369, 75);
    tone(timer, port, pin, 349, 75);
    tone(timer, port, pin, 311, 75);
    tone(timer, port, pin, 277, 75);
    tone(timer, port, pin, 261, 75);
    tone(timer, port, pin, 233, 75);
    tone(timer, port, pin, 220, 450);
}

void playWinnerSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin) {
    tone(timer, port, pin, 220, 78);
    tone(timer, port, pin, 220, 78);
    tone(timer, port, pin, 220, 78);
    tone(timer, port, pin, 220, 78);
    tone(timer, port, pin, 246, 78);
    tone(timer, port, pin, 246, 78);
    tone(timer, port, pin, 277, 78);
    tone(timer, port, pin, 220, 78);
    tone(timer, port, pin, 246, 236);
}
