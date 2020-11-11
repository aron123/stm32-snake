/*
 * game.h
 *
 *  Created on: 2020. nov. 6.
 *      Author: aronk
 */

#ifndef GAME_H_
#define GAME_H_

int determineDirection(int currentDirection);
int isFoodInsideSnake(int snake[][2], int snakeLength, int foodX, int foodY);
int isSnakeEating(int snake[][2], int snakeLength, int food[2]);
int isSnakeBitesItself(int snake[][2], int snakeLength);
int isSnakeCollidesWithWall(int snake[][2], int snakeLength, int direction);
void moveFood(int snake[][2], int snakeLength, int food[2]);
void moveSnake(int snake[][2], int length, int direction, int doEats);

#endif /* GAME_H_ */
