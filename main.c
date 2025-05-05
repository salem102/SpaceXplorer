#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "utilis.h"
#include "types.h"
#include "utilis.c"
#define MIN_ASTEROID_DISTANCE 3

Player player;

int debrisPosX, debrisPosY;
int totalAsteroids;
int asteroidPositionsX[MAX_ASTEROIDS], asteroidPositionsY[MAX_ASTEROIDS];
int isGameOver = 0, gameDifficulty, movementCooldown;



void readIntroAndDifficulty() {
    FILE *file = fopen("settings.txt", "r");
    if (file == NULL) {
        printf("settings.txt not found!\n");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) printf("%s", line);
    fclose(file);

    int validInput = 0;
    while (!validInput) {
        printf("\nChoose difficulty: 1 (Easy), 2 (Medium), 3 (Hard): ");
        if (scanf("%d", &gameDifficulty) != 1) {
            while (getchar() != '\n'); 
            printf("Invalid input! Returning to main menu...\n");
            Sleep(1500);
            return;
        }

        switch (gameDifficulty) {
            case 1:
                player.fuel = 100;
                movementCooldown = 150;
                totalAsteroids = 1;
                validInput = 1;
                break;
            case 2:
                player.fuel = 75;
                movementCooldown = 100;
                totalAsteroids = 3;
                validInput = 1;
                break;
            case 3:
                player.fuel = 50;
                movementCooldown = 75;
                totalAsteroids = 5;
                validInput = 1;
                break;
            default:
                printf("Invalid choice! Returning to main menu...\n");
                Sleep(1500);
                return;
        }
    }
}


void StartGame() {  
	
	player.x = WIDTH / 2;
	player.y = HEIGHT / 2;
	player.health = 100;
	player.fuel = 60;
	player.score = 0;
	
    srand(time(0));  
    
    player.x = WIDTH / 2;
    player.y = HEIGHT / 2;
    
    debrisPosX = rand() % WIDTH;
    debrisPosY = rand() % HEIGHT;
    
    for (int i = 0; i < totalAsteroids; i++) {
        do {
            asteroidPositionsX[i] = rand() % WIDTH;
            asteroidPositionsY[i] = rand() % HEIGHT;
        } while (
            abs(asteroidPositionsX[i] - player.x) < MIN_ASTEROID_DISTANCE &&
            abs(asteroidPositionsY[i] - player.x) < MIN_ASTEROID_DISTANCE
        );
    }
}

void GameScreen() {
    system("cls");
    
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (row == player.y && col == player.x) {
                printf("A");
            }
            else if (row == debrisPosY && col == debrisPosX) {
                printf("#");
            }
            else {
                int isAsteroid = 0;  
                for (int asteroidIdx = 0; asteroidIdx < totalAsteroids; asteroidIdx++) {
                    if (row == asteroidPositionsY[asteroidIdx] && 
                        col == asteroidPositionsX[asteroidIdx]) {
                        printf("@");
                        isAsteroid = 1;  
                        break;
                    }
                }
                if (!isAsteroid) {
                    printf(".");
                }
            }
        }
        printf("\n");
    }

    printf("Fuel: %d | Health: %d | Junk Collected: %d\n", player.fuel, player.health, player.score);
    printf("Press H to check status. X to exit.\n");
}


void collectDebris(Player *p) {
    if (p->health < 100) p->health += 10;
    if (p->fuel < 100) p->fuel += 5;
    p->score++;
}



void GameLogic(Player *p) {
    if (_kbhit()) {
        char input = tolower(_getch());
        switch (input) {
            case 'w': if (p->y > 0) p->y--; break;
            case 's': if (p->y < HEIGHT - 1) p->y++; break;
            case 'a': if (p->x > 0) p->x--; break;
            case 'd': if (p->x < WIDTH - 1) p->x++; break;
            case 'h': 
                printf(">>> Health: %d, Fuel: %d\n", p->health, p->fuel); 
                Sleep(1000); 
                break;
            case 'x': isGameOver = 1; break;
        }
        p->fuel--;
    }

    for (int i = 0; i < totalAsteroids; i++) {
        int direction = rand() % 4;
        switch (direction) {
        	
            case 0: if (asteroidPositionsX[i] > 0) asteroidPositionsX[i]--; break;
            case 1: if (asteroidPositionsX[i] < WIDTH - 1) asteroidPositionsX[i]++; break;
            case 2: if (asteroidPositionsY[i] > 0) asteroidPositionsY[i]--; break;
            case 3: if (asteroidPositionsY[i] < HEIGHT - 1) asteroidPositionsY[i]++; break;
            
        }

        if (player.x == debrisPosX && player.y == debrisPosY) {
    		collectDebris(&player);
    		debrisPosX = rand() % WIDTH;
    		debrisPosY = rand() % HEIGHT;
		}
		if (player.x == asteroidPositionsX[i] && player.y == asteroidPositionsY[i]) {
    		player.health -= 20;
    		if (player.health <= 0) {
        		isGameOver = 1;
        		return;
    	}
	}
    }
    

    



    if (player.fuel <= 0 || player.health <= 0) isGameOver = 1;
    if (player.score >= 5) isGameOver = 2;
    
}



int main() {
	
    int menuSelection;

    while (1) {
    	
        system("cls");
        printf("=== SPACE SURVIVAL ===\n");
        printf("1. Start Game\n2. Show Leaderboard\n3. Exit\nChoose: ");
        scanf("%d", &menuSelection);

        if (menuSelection == 1) {
            readIntroAndDifficulty();

            if (gameDifficulty == 1 || gameDifficulty == 2 || gameDifficulty == 3) {
                StartGame();
                
                while (!isGameOver) {
                    GameScreen();
                    GameLogic(&player);
                    Sleep(movementCooldown);
                }

                GameScreen();
                if (isGameOver == 2) {
                    printf("You win! You cleaned up enough debris!\n");
                }
                else {
                    printf("Game Over. Crashed or ran out of fuel/health.\n");
                }

                addToLeaderboard(player.score, player.fuel, player.health);
                showLeaderboard();
                
                isGameOver = 0;
                player.score = 0;
                player.health = 100;
                
                printf("Press any key to return to menu...");
                _getch();
            }
        }
        else if (menuSelection == 2) {
        	
            showLeaderboard();
            printf("Press any key to return to menu...");
            _getch();
            
        }
        else if (menuSelection == 3) {
            break;
        }
    }

    return 0;
}

