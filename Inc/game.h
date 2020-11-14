/*
 * game.h
 *
 *  Created on: 2020. nov. 6.
 *      Author: aronk
 */

#ifndef GAME_H_
#define GAME_H_

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

int determineDirection(int currentDirection);
int isFoodInsideSnake(int snake[][2], int snakeLength, int foodX, int foodY);
int isSnakeEating(int snake[][2], int snakeLength, int food[2]);
int isSnakeBitesItself(int snake[][2], int snakeLength);
int isSnakeCollidesWithWall(int snake[][2], int snakeLength, int direction);
void moveFood(int snake[][2], int snakeLength, int food[2]);
void moveSnake(int snake[][2], int length, int direction, int doEats);

#endif /* GAME_H_ */
