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
#include "file.h"
#include "roku.h"

roku_config_t roku_config;

/**
 * @brief	Entry point.
 */
int main(int argc, char *argv[])
{
	terminal_enable_raw();
	editor_init();
	if (argc >= 2) {
		file_open(argv[1]);
	}

	// we couldn't exit the program without this
	while (1) {
		editor_refresh_screen();
		input_handle_keypress();
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
