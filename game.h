#ifndef GAME_H
#define GAME_H

#define WIDTH 18
#define HEIGHT 18
#define MAX_ASTEROIDS 5
#define MAX_ENTRIES 10
#define MIN_ASTEROID_DISTANCE 3

#include "types.h"

extern int debrisPosX, debrisPosY;
extern int isGameOver, gameDifficulty, movementCooldown;
extern int totalAsteroids;
extern int asteroidPositionsX[MAX_ASTEROIDS], asteroidPositionsY[MAX_ASTEROIDS];

void readIntroAndDifficulty();
void StartGame();
void GameScreen();
void collectDebris(Player *p);
void GameLogic(Player *p);
void addToLeaderboard(int score, int fuel, int health);
void showLeaderboard();

#endif
