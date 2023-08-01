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
#include "editor.h"
#include "input.h"
#include "roku.h"

roku_config_t roku_config;

/**
 * @brief	Entry point.
 */
int main()
{
	terminal_enable_raw();
	editor_init();

	// we couldn't exit the program without this
	while (1) {
		terminal_clear_screen();
		editor_draw_row_tildes();

		input_handle_keypress();

		write(STDOUT_FILENO, "\x1b[H]", 3);
	}

	return 0;
}

/**
 * @brief	This function displays an error message and exits with status -1.
 */
void die(char *msg)
{
	terminal_clear_screen();

	perror(msg);
	perror("\r\n");
	exit(1);
}
