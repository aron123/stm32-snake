/*
 * tone.h
 *
 *  Created on: 2020. nov. 6.
 *      Author: aronk
 */

#ifndef TONE_H_
#define TONE_H_

void playNokiaTune(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin);
void playFoodCatchSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin);
void playGameOverSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin);
void playWinnerSound(TIM_HandleTypeDef* timer, GPIO_TypeDef* port, uint16_t pin);

#endif /* TONE_H_ */
