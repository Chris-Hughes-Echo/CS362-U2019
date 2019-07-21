// unittest1.c tests the function baronEffect

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int main() {
	srand(time(0));
	int gameSeed = rand() % 1000;
	int numPlayers = 3;
	struct gameState* state = newGame();
	int* kPile = kingdomCards(adventurer, council_room, feast, gardens, mine,
			remodel, smithy, village, baron, great_hall);

	int initResult = initializeGame(numPlayers, kPile, gameSeed, state);


	return 0;
}
