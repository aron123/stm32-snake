/*
 * Original library: https://github.com/embeddedden/lcd5110_driver
 * Tutorial: https://embeddedden.blogspot.com/2017/05/first-steps-with-stm32l152-spi-handling.html
 * ------
 * Extended by Kiss, Aron.
 */

#ifndef LCD5110_H
#define LCD5110_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "stm32f1xx_hal.h"

#define LCD_LINES 6
#define LCD_COLUMNS 84
#define LCD_BUFFER_SIZE 528

typedef struct {
    uint8_t * buf;
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef * RESET_BASE;
    uint16_t RESET_PIN;
    GPIO_TypeDef * COM_DAT_BASE;
    uint16_t COM_DAT_PIN;
    GPIO_TypeDef * SCE_BASE;
    uint16_t SCE_PIN;
} display_5110;

void init_display (display_5110 * display);

void print_string(display_5110 *display, char *buf, int size, int x, int y);

void draw_rect (display_5110 *display, int x, int y, int width, int height);

void draw_filled_rect (display_5110 *display, int x, int y, int width, int height);

void clear_display (display_5110 * display);

void refresh_display (display_5110 * display);

#endif // LCD5110_H
