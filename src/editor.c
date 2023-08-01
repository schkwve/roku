/**
 * @file:		src/editor.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#include <unistd.h>

#include "editor.h"

/**
 * @brief	This routine draws tildes at the beginning of every line
 */
void editor_draw_row_tildes()
{
	// @todo: we should account for different terminal sizes
	for (int y = 0; y < 24; y++) {
		write(STDOUT_FILENO, "~\r\n", 3);
	}

	write(STDOUT_FILENO, "\x1b[H]", 3);
}
