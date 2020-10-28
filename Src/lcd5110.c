/*
 * Original library: https://github.com/embeddedden/lcd5110_driver
 * Tutorial: https://embeddedden.blogspot.com/2017/05/first-steps-with-stm32l152-spi-handling.html
 * ------
 * Extended by Kiss, Aron.
 */

#include "lcd5110.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"
#include "ascii_table.h"
#include <math.h>

/* Private functions */

static void Soft_Delay(volatile uint32_t number) {
    while (number--)
        ;
}

static void switch_to_command_mode(display_5110 *display) {
    HAL_GPIO_WritePin(display->COM_DAT_BASE, display->COM_DAT_PIN, GPIO_PIN_RESET);
}

static void switch_to_data_mode(display_5110 *display) {
    HAL_GPIO_WritePin(display->COM_DAT_BASE, display->COM_DAT_PIN, GPIO_PIN_SET);
}

int is_bit_set(uint8_t num, uint8_t nth) {
    return 1 == ((num >> nth) & 1);
}

int set_pixel(display_5110 *display, int x, int y) {
    if (x > 83 || y > 47) {
        return -1;
    }

    int buff_idx = y * 11 + (int) floor(x / 8);
    int pos = 7 - (x % 8);

    int byte = display->buf[buff_idx];
    byte = (1 << pos) | byte;
    display->buf[buff_idx] = byte;
    return 0;
}

int set_position(display_5110 *display, uint8_t x, uint8_t y) {
    if (!display)
        return -1;
    if (x > 83 || y > 5)
        return -1;
    switch_to_command_mode(display);
    uint8_t byteY = 0x40 + y;
    uint8_t byteX = 0x80 + x;
    //set y address
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(display->hspi, &byteY, 1, 100);
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_SET);
    //set X address
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(display->hspi, &byteX, 1, 100);
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_SET);
    switch_to_data_mode(display);
    return 0;
}

/* Public API */

void init_display(display_5110 *display) {
    uint8_t byte = 0;
    HAL_GPIO_WritePin(display->RESET_BASE, display->RESET_PIN, GPIO_PIN_SET); //reset disable

    Soft_Delay(0xFF);
    HAL_GPIO_WritePin(display->RESET_BASE, display->RESET_PIN, GPIO_PIN_RESET); //reset enable
    Soft_Delay(0xFFFF);
    HAL_GPIO_WritePin(display->RESET_BASE, display->RESET_PIN, GPIO_PIN_SET); //reset disable

    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET); //chip enable
    Soft_Delay(0xFFFFF);
    HAL_GPIO_WritePin(display->COM_DAT_BASE, display->COM_DAT_PIN, GPIO_PIN_RESET); //commands

    Soft_Delay(0xFFFF);
    //Command set in addition mode
    byte = 0x21;
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);

    Soft_Delay(0xFFFF);

    //Bias System
    byte = 0x15;
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);

    Soft_Delay(0xFFFF);

    //temp coef
    byte = 0x07;
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);

    Soft_Delay(0xFFFF);

    //Vop
    byte = 0xB8;
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);

    Soft_Delay(0xFFFF);

    byte = 0x20;
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_SET);

    Soft_Delay(0xFFFF);

    byte = 0x0C;
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(display->hspi, &byte, 1, 100);
    HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_SET);

    Soft_Delay(0xFFFF);

    HAL_GPIO_WritePin(display->COM_DAT_BASE, display->COM_DAT_PIN, GPIO_PIN_SET); //data
}

void print_string(display_5110 *display, char *buf, int size, int x, int y) {
    for (int i = 0; i < size; i++) { // iterate over string chars
        char chr = buf[i];
        for (int j = 0; j < 6; j++) { // iterate over a char's bytes
            uint8_t byte = small_ascii_font[((uint8_t) chr - 32) * 6 + j];
            for (int k = 0; k < 8; k++) { // iterate over a char's byte's bits
                int bit_set = is_bit_set(byte, k);
                if (bit_set) {
                    set_pixel(display, x + j + i * 6, y + k);
                }
            }
        }
    }
}

void draw_rect(display_5110 *display, int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (j == 0 || j == height - 1 || i == 0 || i == width - 1) {
                set_pixel(display, x + i, y + j);
            }
        }
    }
}

void draw_filled_rect(display_5110 *display, int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            set_pixel(display, x + i, y + j);
        }
    }
}

void clear_display(display_5110 *display) {
    for (int i = 0; i < LCD_BUFFER_SIZE; i++) {
        display->buf[i] = 0x00;
    }
}

void refresh_display(display_5110 *display) {
    switch_to_data_mode(display);
    for (int i = 0; i < LCD_LINES; i++) {
        set_position(display, 0, i);
        for (int j = 0; j < LCD_COLUMNS; i++) {
            HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_RESET);

            uint8_t byte = 0x00;
            for (int k = 0; k < 8; k++) {
                // i: position in buffer (line: 0-5)
                // j: position in buffer (column: 0-83)
                // k: position in byte (current column to write)

                int buff_idx = (i * 11 * 8) + k * 11 + (int) floor(j / 8);
                int pos = 7 - j % 8;

                uint8_t element = display->buf[buff_idx];
                int bit_set = is_bit_set(element, pos);
                if (bit_set) { // set k-th bit of byte
                    byte = (1 << k) | byte;
                }
            }

            return;

            HAL_SPI_Transmit(display->hspi, &byte, 1, 10);
            HAL_GPIO_WritePin(display->SCE_BASE, display->SCE_PIN, GPIO_PIN_SET);
        }
    }
}

