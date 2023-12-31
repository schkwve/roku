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

#include "editor.h"
#include "file.h"
#include "input.h"
#include "terminal.h"
#include "find.h"
#include "roku.h"

/**
 * @brief	This routine reads keyboard input and returns it.
 */
int input_get_keypress()
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("read: errno != EAGAIN");
		}
	}

	if (c == '\x1b') {
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return '\x1b';

		if (seq[0] == '[') {
			if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1)
					return '\x1b';

				if (seq[2] == '~') {
					switch (seq[1]) {
					case '1':
						return HOME_KEY;
					case '3':
						return DEL_KEY;
					case '4':
						return END_KEY;
					case '5':
						return PAGE_UP;
					case '6':
						return PAGE_DOWN;
					case '7':
						return HOME_KEY;
					case '8':
						return END_KEY;
					}
				}
			} else {
				switch (seq[1]) {
				case 'A':
					return ARROW_UP;
				case 'B':
					return ARROW_DOWN;
				case 'C':
					return ARROW_RIGHT;
				case 'D':
					return ARROW_LEFT;
				case 'H':
					return HOME_KEY;
				case 'F':
					return END_KEY;
				}
			}
		} else if (seq[0] == 'O') {
			switch (seq[1]) {
			case 'H':
				return HOME_KEY;
			case 'F':
				return END_KEY;
			}
		}

		return '\x1b';
	} else {
		return c;
	}
}

/**
 * @brief	This routine handles keyboard input.
 */
void input_handle_keypress()
{
	static int quit_times = 1;
	int c = input_get_keypress();
	switch (c) {
	/* Special characters */
	case '\r':
	case '\n':
		editor_insert_newline();
		break;
	case BACKSPACE:
	case DEL_KEY:
		if (c == DEL_KEY)
			editor_move_curpos(ARROW_RIGHT);
		editor_remove_char();
		break;
	case CTRL_KEY('l'):
	case '\x1b':
		break;

	/* General keys */
	case CTRL_KEY('q'):
		if (roku_config.file_dirty && quit_times > 0) {
			editor_set_status(
				"File has unsaved changes. Press Ctrl-q again to quit.");
			quit_times--;
			return;
		}
		terminal_clear_screen();
		exit(0);
		break;
	case CTRL_KEY('f'):
		find();
		break;
	case CTRL_KEY('s'):
		file_save();
		break;
	case ARROW_LEFT:
	case ARROW_RIGHT:
	case ARROW_UP:
	case ARROW_DOWN:
		editor_move_curpos(c);
		break;
	case HOME_KEY:
		roku_config.cur_x = 0;
		break;
	case END_KEY:
		if (roku_config.cur_y < roku_config.num_rows) {
			roku_config.cur_x = roku_config.row[roku_config.cur_y].size;
		}
		break;
	case PAGE_UP:
	case PAGE_DOWN: {
		if (c == PAGE_UP) {
			roku_config.cur_y = roku_config.row_off;
		} else if (c == PAGE_DOWN) {
			roku_config.cur_y =
				roku_config.row_off + roku_config.window_size.rows - 1;
			if (roku_config.cur_y > roku_config.num_rows) {
				roku_config.cur_y = roku_config.num_rows;
			}
		}
		int times = roku_config.window_size.rows;
		while (times--) {
			editor_move_curpos(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
		}
	} break;
	default:
		editor_insert_char(c);
		break;
	}

	quit_times = 1;
}
