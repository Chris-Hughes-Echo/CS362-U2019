// randomtestcard1.c tests function baronEffect
// int baronEffect(struct gameState* state, int choice, int currentPlayer)

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

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

int baronTest(int p, struct gameState* state) {
	int numErrors = 0;
	struct gameState pre;
	memcpy (&pre, state, sizeof(struct gameState));

	int choice = roundRandom(Random());
	int result = baronEffect(state, choice, p);

	numErrors += assertTrue(state->numBuys == (pre.numBuys + 1)); // +1 Buy test

	if (choice) { // Chose to discard Estate
		// If estate was in hand, discard and gain +4 coins
		int validChoice = 0;
		int i;
		for (i = 0; i < pre.handCount[p]; i++) {
			if (pre.hand[p][i] == estate) {
				validChoice = 1;
				break;
			}
		}
		if (validChoice) {
			numErrors += assertTrue(state->coins == (pre.coins + 4));
			numErrors += assertTrue(state->discardCount[p] == (pre.discardCount[p] + 1));
			numErrors += assertTrue(state->handCount[p] == (pre.handCount[p] - 1));
			numErrors += assertTrue(state->discard[p][state->discardCount[p] - 1] == estate);
		}
		else { // Player made invalid choice. Gain Estate instead.
			if (pre.supplyCount[estate] > 0) {
				// Test if player gained estate
				numErrors += assertTrue(state->supplyCount[estate] == (pre.supplyCount[estate] - 1));
				numErrors += assertTrue(state->discard[p][state->discardCount[p] - 1] == estate);
				numErrors += assertTrue(state->discardCount[p] == (pre.discardCount[p] - 1));
			}
			else { // Do nothing
				numErrors += assertTrue(state->supplyCount[estate] == pre.supplyCount[estate]);
			}
		}
	}
	else { // Chose not to discard, gain Estate if in supply
		if (pre.supplyCount[estate] > 0) {
			// Test if player gained estate
			numErrors += assertTrue(state->supplyCount[estate] == (pre.supplyCount[estate] - 1));
			numErrors += assertTrue(state->discard[p][state->discardCount[p] - 1] == estate);
			numErrors += assertTrue(state->discardCount[p] == (pre.discardCount[p] - 1));
		}
		else {
			// Do nothing
			numErrors += assertTrue(state->supplyCount[estate] == pre.supplyCount[estate]);
		}
	}
	return numErrors;
}

int main() {
	SelectStream(2);
	PutSeed(3);

	struct gameState G;
	int n, i, j, p;

	for (n = 0; n < 1000; n++) {
		//memset(&G, '0', sizeof(struct gameState));
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);
		}
		G.numPlayers = roundRandom(Random() * 2) + 2; // numPlayers between 2 and 4
		p = floor(Random() * G.numPlayers);
		G.deckCount[p] = roundRandom(Random() * MAX_DECK);
		G.discardCount[p] = roundRandom(Random() * MAX_DECK);
		G.handCount[p] = roundRandom(Random() * MAX_HAND);
		G.numBuys = roundRandom(Random());
		G.supplyCount[estate] = roundRandom(Random() * 8);

		int hasEstate = roundRandom(Random());
		if (hasEstate) { // Put Estate in player hand
			if (G.handCount[p] == 0) {
				G.handCount[p]++;
			}
			G.hand[p][0] = estate;
		}
		else { // Ensure no Estate in player hand
			for (j = 0; j < G.handCount[p]; j++) {
				if (G.hand[p][j] == estate) {
					G.hand[p][j] = duchy; // Arbitrarily convert estate to duchy
				}
			}
		}

		printf("Case %d: numP=%d, p=%d, deck=%d, discard=%d, hand=%d, buys=%d, supply=%d, hasEstate=%d ... ", 
				n+1, G.numPlayers, p, G.deckCount[p], G.discardCount[p], G.handCount[p], G.numBuys, G.supplyCount[estate], hasEstate);
		fflush(stdout);
		int numErrors = baronTest(p, &G);
		printf("Errors: %d\n", numErrors);
		fflush(stdout);
	}
	return 0;
}
