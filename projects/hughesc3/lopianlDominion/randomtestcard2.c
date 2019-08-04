// randomtestcard2.c tests function minionEffect
// int minionEffect(struct gameStatee*, int choice1, int choice2, int currentPlayer, int handPos)
// int playMinion(struct gameState* state, int handPos, int currentPlayer, int choice1, int choice2)

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

int assertTrue(int a) {
	if (a) {
		return 0;
	}
	else {
		return 1;
	}
}

double roundRandom(double randVal) {
	if (randVal - floor(randVal) < .500) { // Round down
		return floor(randVal);
	} else { // Round up
		return ceil(randVal);
	}
}

int minionTest(int p, int choice1, int choice2, struct gameState* state) {
	int numErrors = 0;
	struct gameState pre;
	memcpy (&pre, state, sizeof(struct gameState));

	//minionEffect(state, choice1, choice2, p, 0);
	playMinion(state, 0, p, choice1, choice2);

	numErrors += assertTrue(state->numActions == (pre.numActions + 1));
	//pre.numActions++;
	if (choice1) { // +2 coins
		numErrors += assertTrue(state->coins == (pre.coins + 2));
		//pre.coins += 2;
	}
	else if (choice2) { // Discard hand, +4 cards, and all other players with at least 5 in hand do the same
		int i;
		// Discard player hand, +4 cards
		numErrors += assertTrue(state->discardCount[p] == (pre.discardCount[p] + pre.handCount[p]));
		numErrors += assertTrue(state->handCount[p] == 4);
		// Discard other hands
		for (i = 0; i < state->numPlayers; i++) {
			if (i != p) {
				if (pre.handCount[i] >= 5) {
					numErrors += assertTrue(state->discardCount[i] == (pre.discardCount[i] + pre.handCount[i]));
					numErrors += assertTrue(state->handCount[i] == 4);
				}
				else {
					//Do nothing
					numErrors += assertTrue(state->discardCount[i] == pre.discardCount[i]);
					numErrors += assertTrue(state->handCount[i] == pre.handCount[i]);
				}
			}
		}
	}
	return numErrors;
}

int main() {
	srand(time(0));
	int stream = (rand() % 5) + 1;
	SelectStream(stream);
	int seed = (rand() % 1000) + 1;
	PutSeed(seed);

	struct gameState G;
	int n, i, p, c1, c2;

	for (n = 0; n < 10; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);
		}
		G.numPlayers = roundRandom(Random() * 2) + 2; // numPlayers between 2 and 4
		p = floor(Random() * G.numPlayers);

		// Give hands and decks to all players
		for (i = 0; i < G.numPlayers; i++) {
			G.deckCount[i] = roundRandom(Random() * MAX_DECK);
			G.handCount[i] = roundRandom(Random() * MAX_HAND);
			G.discardCount[i] = roundRandom(Random() * MAX_DECK);
		}
		G.numActions = roundRandom(Random());

		// Generate random player choice
		int choiceGen = roundRandom(Random());
		if (choiceGen == 0) {
			c1 = 1;
			c2 = 0;
		}
		else if (choiceGen == 1) {
			c1 = 0;
			c2 = 1;
		}

		printf("Case %d: numP=%d, p=%d, actions=%d, choice1=%d, choice2=%d ... ", 
				n+1, G.numPlayers, p, G.numActions, c1, c2);
		fflush(stdout);

		int numErrors = minionTest(p, c1, c2, &G);
		//int numErrors = minionTest(p, 0, 1, &G);

		printf("Errors: %d\n", numErrors);
		//fflush(stdout);
	}
	return 0;
}
