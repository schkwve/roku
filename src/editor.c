/**
 * @file:		src/editor.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#include <unistd.h>

#include "editor.h"
#include "roku.h"

/**
 * @brief	This routine draws tildes at the beginning of every line
 */
void editor_draw_row_tildes()
{
	for (int y = 0; y < roku_config.window_size.rows; y++) {
		write(STDOUT_FILENO, "~\r\n", 3);
	}

	write(STDOUT_FILENO, "\x1b[H]", 3);
}

/**
 * @brief	This routine initializes the editor interface
 */
void editor_init()
{
	if (terminal_get_window_size(&roku_config.window_size.rows, &roku_config.window_size.cols) == -1) {
		die("terminal_get_window_size: couldn't get window size");
	}
}
