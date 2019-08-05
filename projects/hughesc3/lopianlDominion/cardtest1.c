// cardtest1.c tests initializeGame()
// int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed, struct gameState* state)

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define NEG_CHOICE 0
#define POS_CHOICE 1
#define NO_HAND_ESTATE 0
#define YES_HAND_ESTATE 1
#define NO_SUPPLY_ESTATE 0
#define YES_SUPPLY_ESTATE 1
#define PLAYER_COUNT 2
#define CURRENT_PLAYER 0

int assertTrue(int a) {
	if (a) {
		printf("SUCCESS!\n");
	}
	else {
		printf("FAILED!\n");
	}
	return a;
}

void baronTest(struct gameState* state, struct gameState* preState, int* kPile, 
			  int choseDiscard, int estateInHand, int estateInSupply) {
	int gameSeed = (rand() % 1000) + 1;
	memset(state, '0', sizeof(struct gameState));
	memset(preState, '0', sizeof(struct gameState));
	initializeGame(PLAYER_COUNT, kPile, gameSeed, state);
	int i;

	// Modify game state for test cases
	if (estateInHand) { // Modify player hand to contain Estate
		gainCard(estate, state, 2, CURRENT_PLAYER);
	}
	else { // Ensure hand does not contain Estate
		for (i = 0; i < state->handCount[CURRENT_PLAYER]; i++) {
			if (state->hand[CURRENT_PLAYER][i] == estate) {
				state->hand[CURRENT_PLAYER][i] = duchy; // Arbitrarily change Estate to Duchy
			}
		}
	}
	if (estateInSupply) { // Default scenario after initialization is true, do nothing
	}
	else { // Set Estate supply to 0
		state->supplyCount[estate] = 0;
	}

	// Copy preState and call function
	*preState = *state; // Copy state to preState
	playBaron(state, choseDiscard, CURRENT_PLAYER);

	// +1 Buy, and other players unaffected should be consistent across all test cases
	printf("Testing +1 Buy: ");
	assertTrue(state->numBuys == (preState->numBuys + 1));
	printf("Testing other player same state: ");
	assertTrue((state->handCount[CURRENT_PLAYER + 1] == preState->handCount[CURRENT_PLAYER + 1])
			&& (state->discardCount[CURRENT_PLAYER + 1] == preState->discardCount[CURRENT_PLAYER + 1]));

	// Test branches
	if (choseDiscard) { // Player chose to discard an Estate to gain 4 gold
		if (estateInHand) { // Discard Estate for +4 coins
			printf("Testing +4 coins: ");
			assertTrue(state->coins == (preState->coins + 4));
			printf("Testing Estate discarded: ");
			assertTrue((state->discardCount[CURRENT_PLAYER] == (preState->discardCount[CURRENT_PLAYER] + 1))
					&& (state->handCount[CURRENT_PLAYER] == (preState->handCount[CURRENT_PLAYER] - 1))
					&& (state->discard[CURRENT_PLAYER][state->discardCount[CURRENT_PLAYER] - 1] == estate));
		}
		else { // Player made incorrect choice. Should gain Estate instead.
			if (estateInSupply) { // Player should gain Estate from supply
				printf("Testing 1 Estate removed from supply: ");
				assertTrue(state->supplyCount[estate] == (preState->supplyCount[estate] - 1));
				printf("Testing Estate added to player discard pile: ");
				assertTrue((state->discard[CURRENT_PLAYER][state->discardCount[CURRENT_PLAYER] - 1] == estate)
						&& (state->discardCount[CURRENT_PLAYER] == (preState->discardCount[CURRENT_PLAYER] + 1)));
			}
			else { // Cannot gain Estate, check equal supply
				printf("Testing NO Estates gained: ");
				assertTrue(state->supplyCount[estate] == preState->supplyCount[estate]);
			}
		}
	}
	else { // Player chose not to discard an Estate in order to gain an Estate
		if (estateInSupply) { // Player should gain Estate from supply
			printf("Testing 1 Estate removed from supply: ");
			assertTrue(state->supplyCount[estate] == (preState->supplyCount[estate] - 1));
			printf("Testing Estate added to player discard pile: ");
			assertTrue((state->discard[CURRENT_PLAYER][state->discardCount[CURRENT_PLAYER] - 1] == estate)
					&& (state->discardCount[CURRENT_PLAYER] == (preState->discardCount[CURRENT_PLAYER] + 1)));
		}
		else { // Cannot gain Estate, check equal supply
			printf("Testing NO Estates gained: ");
			assertTrue(state->supplyCount[estate] == preState->supplyCount[estate]);
		}
	}
}

int main() {
	srand(time(0));
	int seed = rand() % 1000);
	struct gameState* state = newGame();
	struct gameState* preState = newGame();
	int* kPile = kingdomCards(adventurer, council_room, feast, gardens, mine,
			remodel, smithy, village, baron, great_hall);

	// Player chose to discard and has estate in hand
	printf("\nPOS, YES_HAND, YES_SUPPLY\n");
	baronTest(state, preState, kPile, POS_CHOICE, YES_HAND_ESTATE, YES_SUPPLY_ESTATE);

	// Player chose to discard, does not have estate in hand, but there is one in supply
	printf("\nPOS, NO_HAND, YES_SUPPLY\n");
	baronTest(state, preState, kPile, POS_CHOICE, NO_HAND_ESTATE, YES_SUPPLY_ESTATE);

	// Player chose to discard, does not have estate in hand, and there are none in supply
	printf("\nPOS, NO_HAND, NO_SUPPLY\n");
	baronTest(state, preState, kPile, POS_CHOICE, NO_HAND_ESTATE, NO_SUPPLY_ESTATE);

	// Player choose not to discard, and supply contains an estate
	printf("\nNEG, NO_HAND, YES_SUPPLY\n");
	baronTest(state, preState, kPile, NEG_CHOICE, NO_HAND_ESTATE, YES_SUPPLY_ESTATE);

	// Player chose not to discard, and there are none in supply
	printf("\nNEG, NO_HAND, NO_SUPPLY\n");
	baronTest(state, preState, kPile, NEG_CHOICE, NO_HAND_ESTATE, NO_SUPPLY_ESTATE);

	return 0;
}
