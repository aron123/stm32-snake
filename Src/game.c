/*
 * game.c
 *
 *  Created on: 2020. nov. 6.
 *      Author: aronk
 */
#include <stdlib.h>
#include "stm32f1xx_hal.h"

int determineDirection(int currentDirection) {
    int upPushed = HAL_GPIO_ReadPin(BTN_UP_GPIO_Port, BTN_UP_Pin) == GPIO_PIN_RESET;
    int rightPushed = HAL_GPIO_ReadPin(BTN_RIGHT_GPIO_Port, BTN_RIGHT_Pin) == GPIO_PIN_RESET;
    int downPushed = HAL_GPIO_ReadPin(BTN_DOWN_GPIO_Port, BTN_DOWN_Pin) == GPIO_PIN_RESET;
    int leftPushed = HAL_GPIO_ReadPin(BTN_LEFT_GPIO_Port, BTN_LEFT_Pin) == GPIO_PIN_RESET;

    if (!upPushed && !rightPushed && !downPushed && !leftPushed) {
        return currentDirection;
    }

    if ((upPushed && currentDirection == SNAKE_DIRECTION_UP) || (rightPushed && currentDirection == SNAKE_DIRECTION_RIGHT)
            || (downPushed && currentDirection == SNAKE_DIRECTION_DOWN)
            || (leftPushed && currentDirection == SNAKE_DIRECTION_LEFT)) {
        return currentDirection;
    }

    if ((upPushed && currentDirection == SNAKE_DIRECTION_DOWN) || (rightPushed && currentDirection == SNAKE_DIRECTION_LEFT)
            || (downPushed && currentDirection == SNAKE_DIRECTION_UP)
            || (leftPushed && currentDirection == SNAKE_DIRECTION_RIGHT)) {
        return currentDirection;
    }

    int pushedCount = 0;
    int buttonPushes[] = { upPushed, rightPushed, downPushed, leftPushed };

    for (int i = 0; i < 4; i++) {
        pushedCount += buttonPushes[i];
    }

    if (pushedCount != 1) {
        return currentDirection;
    }

    if (upPushed) {
        return SNAKE_DIRECTION_UP;
    }
    if (rightPushed) {
        return SNAKE_DIRECTION_RIGHT;
    }
    if (downPushed) {
        return SNAKE_DIRECTION_DOWN;
    }
    if (leftPushed) {
        return SNAKE_DIRECTION_LEFT;
    }

    return -1;
}

int isFoodInsideSnake(int snake[][2], int snakeLength, int foodX, int foodY) {
    for (int i = 0; i < snakeLength; i++) {
        if (snake[i][0] == foodX && snake[i][1] == foodY) {
            return true;
        }
    }

    return false;
}

int isSnakeEating(int snake[][2], int snakeLength, int food[2]) {
    int head = snakeLength - 1;
    return food[0] == snake[head][0] && food[1] == snake[head][1];
}

int isSnakeBitesItself(int snake[][2], int snakeLength) {
    int headX = snake[snakeLength - 1][0];
    int headY = snake[snakeLength - 1][1];

    for (int i = 0; i < snakeLength - 1; i++) {
        if (snake[i][0] == headX && snake[i][1] == headY) {
            return true;
        }
    }

    return false;
}

int isSnakeCollidesWithWall(int snake[][2], int snakeLength, int direction) {
    int headX = snake[snakeLength - 1][0];
    int headY = snake[snakeLength - 1][1];

    return ((headX == 0 && direction == SNAKE_DIRECTION_LEFT)
            || (headY == 0 && direction == SNAKE_DIRECTION_UP)
            || (headX == GAME_WIDTH - 1 && direction == SNAKE_DIRECTION_RIGHT)
            || (headY == GAME_HEIGHT - 1 && direction == SNAKE_DIRECTION_DOWN));
}

void moveFood(int snake[][2], int snakeLength, int food[2]) {
    int width, height;

    do {
        width = 1 + rand() % (GAME_WIDTH - 1);
        height = 1 + rand() % (GAME_HEIGHT - 1);
    } while (isFoodInsideSnake(snake, snakeLength, width, height));

    food[0] = width;
    food[1] = height;
}

void moveSnake(int snake[][2], int length, int direction, int doEats) {
    int previousBlock[2] = { 0, 0 };
    int head = length - 1;

    // if snake eats, only put one block on the head
    if (doEats) {
        int secondBlock[2] = { snake[head - 1][0], snake[head - 1][1] };

        if (direction == SNAKE_DIRECTION_UP) {
            snake[head][0] = secondBlock[0];
            snake[head][1] = secondBlock[1] - 1;
        } else if (direction == SNAKE_DIRECTION_RIGHT) {
            snake[head][0] = secondBlock[0] + 1;
            snake[head][1] = secondBlock[1];
        } else if (direction == SNAKE_DIRECTION_DOWN) {
            snake[head][0] = secondBlock[0];
            snake[head][1] = secondBlock[1] + 1;
        } else if (direction == SNAKE_DIRECTION_LEFT) {
            snake[head][0] = secondBlock[0] - 1;
            snake[head][1] = secondBlock[1];
        }

        return;
    }

    // avoid to overcome borders
    if ((direction == SNAKE_DIRECTION_UP && snake[head][1] == 0)
            || (direction == SNAKE_DIRECTION_RIGHT && snake[head][0] == GAME_WIDTH - 1)
            || (direction == SNAKE_DIRECTION_DOWN && snake[head][1] == GAME_HEIGHT - 1)
            || (direction == SNAKE_DIRECTION_LEFT && snake[head][0] == 0)) {
        return;
    }

    // move head, shift body parts
    for (int i = length - 1; i >= 0; i--) {
        if (i == length - 1) {
            previousBlock[0] = snake[i][0];
            previousBlock[1] = snake[i][1];

            if (direction == SNAKE_DIRECTION_UP) {
                snake[i][1] -= 1;
            } else if (direction == SNAKE_DIRECTION_RIGHT) {
                snake[i][0] += 1;
            } else if (direction == SNAKE_DIRECTION_DOWN) {
                snake[i][1] += 1;
            } else if (direction == SNAKE_DIRECTION_LEFT) {
                snake[i][0] -= 1;
            }
        } else {
            int tmp = snake[i][0];
            snake[i][0] = previousBlock[0];
            previousBlock[0] = tmp;

            tmp = snake[i][1];
            snake[i][1] = previousBlock[1];
            previousBlock[1] = tmp;
        }
    }
}
