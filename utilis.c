#include "utilis.h"
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

void addToLeaderboard(int score, int fuel, int health) {
	
    FILE *file = fopen("leaderboard.txt", "a"); 
    
    if (file == NULL) {
        fprintf(stderr, "Error opening leaderboard file!\n");
        return;
    }
    
    fprintf(file, "Score: %d, Fuel: %d, Health: %d\n", score, fuel, health);
    fclose(file);
}

void showLeaderboard() {

    FILE *file = fopen("leaderboard.txt", "r");
    
    if (file == NULL) {
        printf("Leaderboard is empty!\n");
        return;
    }

    char line[100];
    printf("=== LEADERBOARD ===\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}