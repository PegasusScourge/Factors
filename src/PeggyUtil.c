#include "PeggyUtil.h"

#include <stdio.h>

int getConsoleLine(char* buf, int bufLen) {
	char c;
	int i = 0;
	while (i < bufLen - 1) {
		c = getchar();
		if (c == EOF || c == '\n' || c == '\r') {
			// if we get an EOF or a new line character, break
			break;
		}

		buf[i] = c;
		i++;
	}
	buf[i] = 0; // Set the next position to a null character

	return i; // Return the length of the string got (0th char can techincally be the null character we just appended)
}

void clearScreen() {
	system("cls");
}
