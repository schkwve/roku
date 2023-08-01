/**
 * @file:		src/roku.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "terminal.h"

/**
 * @brief	Entry point.
 */
int main()
{
	terminal_enable_raw();

	// we couldn't exit the program without this
	while (1) {
		char c = '\0';
		read(STDIN_FILENO, &c, 1);
		if (!iscntrl(c)) {
			printf("%c\r\n", c);
		}
		if (c == 'q') break;
	}

	return 0;
}

/**
 * @brief	This function displays an error message and exits with status -1.
 */
void die(char *msg)
{
	perror(msg);
	perror("\r\n");
	exit(1);
}
