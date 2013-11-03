/*
 * main.c
 *
 *  Created on: 26/ott/2013
 *      Author: cristina
 */

#include <serial/serial.h>
#include <stdio.h>

int main() {
	serial_configuration_t ttyO1 = {
						"/dev/ttyO1",
						-1,
						0,
						9600
				};
	char ch;
	printf("Start serial read on port ttyO1 \n");

	while(1) {
		serial_read(&ch, 1, &ttyO1);
		printf("%c", ch);
		fflush(stdout);
	}
}
