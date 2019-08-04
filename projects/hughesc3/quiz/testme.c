/*************************
 * Christopher Hughes
 * Random Testing Quiz
**************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MIN_ASCII 32
#define MAX_ASCII 126
#define STRING_LENGTH 5
#define NUM_VOWELS 5
#define NUM_CONSONANTS 21
#define MIN_ASCII_LOWER 97
#define MAX_ASCII_LOWER 122

//const char* lowerVowels = "aeiou";
//const char* lowerConsonants = "bcdfghjklmnpqrstvwxyz";

char inputChar()
{
    // TODO: rewrite this function
	// Generate random int from MIN_ASCII to MAX_ASCII inclusive and cast to char
	// This will yield a character between ' ' and '~' inclusive
	return (char)((rand() % (MAX_ASCII - MIN_ASCII + 1)) + MIN_ASCII);
    //return ' ';
}

char *inputString()
{
    // TODO: rewrite this function
	int i;
	char* outString = malloc((STRING_LENGTH + 1) * sizeof(char));
	
	/*
	 * Tighter restriction based on vowel-consonant expectations in exit condition
	for (i = 0; i < STRING_LENGTH; i++) {
		if (i == 0 || i == 2 || i == 4) { // Generate lower-case consonant
			outString[i] = lowerConsonants[rand() % NUM_CONSONANTS];
		}
		else { // Generate lower-case vowel
			outString[i] = lowerVowels[rand() % NUM_VOWELS];
		}
	}
	*/

	// Fills string with random lowercase letters
	for (i = 0; i < STRING_LENGTH; i++) {
		outString[i] = (char)((rand() % (MAX_ASCII_LOWER - MIN_ASCII_LOWER + 1)) + MIN_ASCII_LOWER);
	}
	outString[STRING_LENGTH] = '\0';

	return outString;
    //return "";
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
