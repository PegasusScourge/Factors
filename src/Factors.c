/*

Takes a number and generates a factor tree

*/

#define _CRT_SECURE_NO_WARNINGS

#include "PeggyUtil.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define CON_BUFFER_SIZE 100

/* Struct defs */
typedef struct AFactor AFactor;

struct AFactor {
	int number;
	bool hasFactors;
	AFactor* factor1;
	AFactor* factor2;
};

/* Function defs */
// Frees a factor and it's daughter factors
void freeAFactor(AFactor* factor);
// Takes a factor and creates daughter factors until it hits a prime
bool calculateFactors(AFactor* rootFactor);
// Returns if a number is prime
bool isPrime(int n);
// Writes a tree to a file in a nice nested fashion
void writeTreeToFile(const char* path, AFactor* rootFactor);
// Write a factor to a file stream at the nested level and write it's daughters recursively
void writeAFactor(FILE* file, AFactor* factor, int nestLevel);

int main()
{
	char conBuffer[CON_BUFFER_SIZE];

	int gotChars = 0;
	int number = 0;

	while (true) {
		clearScreen();

		printf("Enter a number to generate a tree (type 'quit' to exit):\n> ");
		gotChars = getConsoleLine(conBuffer, CON_BUFFER_SIZE);

		if (gotChars <= 0) {
			printf("Failed to get a valid input\n");
			continue;
		}

		if (strcmp(conBuffer, "quit") == 0) {
			exit(EXIT_SUCCESS);
		}

		number = atoi(conBuffer);

		if (number < 0) {
			printf("This program only accepts positive integers\n");
			continue;
		}

		if (number == 0) {
			// Factors are only 0
		}
		else if (number == 1) {
			// Factors are only 1
		}
		else {
			// Create the root factor
			AFactor* rootFactor = malloc(sizeof(AFactor));

			if (rootFactor == NULL) {
				perror("Failed to allocate memory for root factor!");
				continue;
			}

			rootFactor->number = number;

			bool success = calculateFactors(rootFactor);
			
			if (!success) {
				freeAFactor(rootFactor);
				printf("\n\nFAILED TO FIND FACTORS\n");
				continue;
			}

			printf("1 = %i\n", number);

			printf("\nDo you wish to write the factor tree to file? (y/n)\n> ");
			gotChars = getConsoleLine(conBuffer, CON_BUFFER_SIZE);

			if (strcmp(conBuffer, "y") == 0) {
				printf("Enter the file path:\n> ");
				gotChars = getConsoleLine(conBuffer, CON_BUFFER_SIZE);
				
				// Write to file
				writeTreeToFile(conBuffer, rootFactor);
			}

			// Finally, free the factor
			freeAFactor(rootFactor);
			rootFactor = NULL;
		}
	}
}

bool calculateFactors(AFactor* rootFactor) {
	int factor1Num = 0;
	int factor2Num = 0;

	rootFactor->hasFactors = false;

	if (isPrime(rootFactor->number)) {
		// This factor is prime and thus we should not investigate it any more
		printf("%i x ", rootFactor->number);
		return true;
	}else 
	// Test for 2 division
	if (rootFactor->number % 2 == 0) {
		// Is divisible by two, set the factors and continue down the tree
		rootFactor->hasFactors = true;
		// Factor 1
		factor1Num = 2;
		factor2Num = rootFactor->number / 2;
	}
	else {
		for (int i = 3; i < rootFactor->number / 2; i++) {
			if (rootFactor->number % i == 0) {
				// We found our factors
				rootFactor->hasFactors = true;
				factor1Num = i;
				factor2Num = rootFactor->number / i;
				break;
			}
		}
	}

	// Create the lower factors
	if (rootFactor->hasFactors) {
		//printf(" - Number %i has factors %i and %i\n", rootFactor->number, factor1Num, factor2Num);

		// Create factor 1
		rootFactor->factor1 = malloc(sizeof(AFactor));
		if (rootFactor->factor1 == NULL) {
			// Failed to create factor, crash
			perror("Failed to create factor1 memory");
			return false;
		}
		// Factor is okay, go forth
		rootFactor->factor1->number = factor1Num;
		if(!calculateFactors(rootFactor->factor1))
			return false;

		// Create factor 2
		rootFactor->factor2 = malloc(sizeof(AFactor));
		if (rootFactor->factor2 == NULL) {
			// Failed to create factor, crash
			perror("Failed to create factor2 memory");
			return false;
		}
		// Factor is okay, go forth
		rootFactor->factor2->number = factor2Num;
		if (!calculateFactors(rootFactor->factor2))
			return false;
	}

	// Give a success value
	return true;
}

bool isPrime(int n) {
	if (n == 1) {
		// We'll treat 1 as a prime for this purpose
		return true;
	}
	bool flag = true;
	for (int i = 2; i <= n / 2; i++) {
		// condition for nonprime number
		if (n % i == 0) {
			flag = false;
			break;
		}
	}
	return flag;
}

void writeTreeToFile(const char* path, AFactor* rootFactor) {
	FILE* file;
	file = fopen(path, "w");

	if (file == NULL) {
		perror("Failed to open file");
		return;
	}

	// Write to the file
	writeAFactor(file, rootFactor, 0);

	fclose(file);
}

void writeAFactor(FILE* file, AFactor* factor, int nestLevel) {
	// Write the indentation
	for (int i = 0; i < nestLevel; i++) {
		fprintf(file, "   ");
	}
	fprintf(file, "-> %i\n", factor->number);
	if (factor->hasFactors) {
		writeAFactor(file, factor->factor1, nestLevel + 1);
		writeAFactor(file, factor->factor2, nestLevel + 1);
	}
}

void freeAFactor(AFactor* factor) {
	if (factor->hasFactors) {
		// Free the lower factors accordingly
		freeAFactor(factor->factor1);
		factor->factor1 = NULL;
		freeAFactor(factor->factor2);
		factor->factor2 = NULL;
	}
	// Free this factor from memory
	free(factor);
}
