/**
 * @file:		src/input.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "input.h"
#include "terminal.h"
#include "roku.h"

/**
 * @brief	This routine reads keyboard input and returns it.
 */
char input_get_keypress()
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("read: errno != EAGAIN");
		}
	}
	return c;
}

/**
 * @brief	This routine handles keyboard input.
 */
void input_handle_keypress()
{
	char c = input_get_keypress();
	switch (c) {
		case CTRL_KEY('q'):
			terminal_clear_screen();
			exit(0);
			break;
		default:
			break;
	}
}
