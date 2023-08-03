/**
 * @file:		src/find.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to search queries
 */

#include <stdlib.h>
#include <string.h>

#include "editor.h"
#include "input.h"
#include "roku.h"
#include "find.h"

/**
 * @brief	This routine searches for a query and shows it if found.
 */
void find()
{
	int saved_cur_x = roku_config.cur_x;
	int saved_cur_y = roku_config.cur_y;
	int saved_col_off = roku_config.col_off;
	int saved_row_off = roku_config.row_off;

	char *query =
		editor_display_prompt("Search: %s (ESC to cancel)", find_callback);

	if (query) {
		free(query);
	} else {
		roku_config.cur_x = saved_cur_x;
		roku_config.cur_y = saved_cur_y;
		roku_config.col_off = saved_col_off;
		roku_config.row_off = saved_row_off;
	}
}

/** 
 * @brief	This routine is a callback to the find() function
 */
void *find_callback(char *query, int key)
{
	static int last_match = -1;
	static int direction = -1;

	if (key == '\r' || key == '\n' || key == '\x1b') {
		last_match = -1;
		last_match = 1;
		return NULL;
	} else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
		direction = 1;
	} else if (key == ARROW_LEFT || key == ARROW_UP) {
		direction = -1;
	} else {
		last_match = -1;
		direction = 1;
	}

	if (last_match == -1) {
		direction = 1;
	}

	int current = last_match;
	for (int i = 0; i < roku_config.num_rows; i++) {
		current += direction;
		if (current == -1) {
			current = roku_config.num_rows - 1;
		} else if (current == roku_config.num_rows) {
			current = 0;
		}

		editor_row_t *row = &roku_config.row[current];
		char *match = strstr(row->render, query);
		if (match) {
			last_match = current;
			roku_config.cur_y = current;
			roku_config.cur_x =
				editor_row_rx_to_cur_x(row, match - row->render);
			roku_config.cur_x = match - row->render;
			roku_config.row_off = roku_config.num_rows;
			break;
		}
	}
	return NULL;
}
