/**
 * @file:		src/editor.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "input.h"
#include "editor.h"
#include "roku.h"

/**
 * @brief	This routine draws every row on the screen.
 * 			If a row hasn't been specified to be drawn,
 * 			the first character of it is replaced by a tilde (~).
 */
void editor_draw_row(struct append_buf *buf)
{
	for (int y = 0; y < roku_config.window_size.rows; y++) {
		int file_row = y + roku_config.row_off;
		if (file_row >= roku_config.num_rows) {
			if (roku_config.num_rows == 0 &&
				y == roku_config.window_size.rows / 3) {
				char welcome_msg[80];
				int welcome_msg_len =
					snprintf(welcome_msg, sizeof(welcome_msg),
							 ROKU_WELCOME_MESSAGE, ROKU_VERSION);
				if (welcome_msg_len > roku_config.window_size.cols) {
					welcome_msg_len = roku_config.window_size.cols;
				}

				int padding =
					(roku_config.window_size.cols - welcome_msg_len) / 2;
				if (padding) {
					editor_buffer_append(buf, "~", 1);
					padding--;
				}

				while (padding--) {
					editor_buffer_append(buf, " ", 1);
				}

				editor_buffer_append(buf, welcome_msg, welcome_msg_len);
			} else {
				editor_buffer_append(buf, "~", 1);
			}
		} else {
			int len = roku_config.row[file_row].size - roku_config.col_off;
			if (len < 0) {
				len = 0;
			}

			if (len > roku_config.window_size.cols) {
				len = roku_config.window_size.cols;
			}

			editor_buffer_append(
				buf, &roku_config.row[file_row].buf[roku_config.col_off], len);
		}

		editor_buffer_append(buf, "\x1b[K", 3);
		if (y < roku_config.window_size.rows - 1) {
			editor_buffer_append(buf, "\r\n", 2);
		}
	}

	write(STDOUT_FILENO, "\x1b[H]", 3);
}

/**
 * @brief	This routine informs the editor_refresh_screen()
 * 			function to update the cursor position
 */
void editor_move_curpos(int key)
{
	switch (key) {
	case ARROW_LEFT:
		if (roku_config.cx != 0) {
			roku_config.cx--;
		}
		break;
	case ARROW_RIGHT:
		roku_config.cx++;
		break;
	case ARROW_UP:
		if (roku_config.cy != 0) {
			roku_config.cy--;
		}
		break;
	case ARROW_DOWN:
		if (roku_config.cy < roku_config.num_rows) {
			roku_config.cy++;
		}
		break;
	}
}

/**
 * @brief	This routine initializes the editor interface
 */
void editor_init()
{
	roku_config.cx = 0;
	roku_config.cy = 0;
	roku_config.row_off = 0;
	roku_config.col_off = 0;
	roku_config.num_rows = 0;
	roku_config.row = NULL;

	if (terminal_get_window_size(&roku_config.window_size.rows,
								 &roku_config.window_size.cols) == -1) {
		die("terminal_get_window_size: couldn't get window size");
	}
}

/**
 * @brief	This routine is called upon screen refresh.
 */
void editor_refresh_screen()
{
	struct append_buf buf = APPEND_BUF_INIT;

	editor_handle_scrolling();

	editor_buffer_append(&buf, "\x1b[?25l", 6);
	editor_buffer_append(&buf, "\x1b[H", 3);

	editor_draw_row(&buf);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "\x1b[%d;%dH",
			 (roku_config.cy - roku_config.row_off) + 1,
			 (roku_config.cx - roku_config.col_off) + 1);
	editor_buffer_append(&buf, buffer, strlen(buffer));

	editor_buffer_append(&buf, "\x1b[?25h", 6);

	write(STDOUT_FILENO, buf.buffer, buf.size);
	editor_buffer_free(&buf);
}

/**
 * @brief	This routine handles buffer scrolling
 * 			if requested.
 */
void editor_handle_scrolling()
{
	if (roku_config.cy < roku_config.row_off) {
		roku_config.row_off = roku_config.cy;
	}
	if (roku_config.cy >= roku_config.row_off + roku_config.window_size.rows) {
		roku_config.row_off = roku_config.cy - roku_config.window_size.rows + 1;
	}
	if (roku_config.cx < roku_config.col_off) {
		roku_config.col_off = roku_config.cx;
	}
	if (roku_config.cx >= roku_config.col_off + roku_config.window_size.cols) {
		roku_config.col_off = roku_config.cx - roku_config.window_size.cols + 1;
	}
}

/**
 * @brief	This routine appends a string to the write buffer
 */
void editor_buffer_append(struct append_buf *buf, const char *s, int len)
{
	char *new = realloc(buf->buffer, buf->size + len);

	if (new == NULL)
		return;
	memcpy(&new[buf->size], s, len);
	buf->buffer = new;
	buf->size += len;
}

/**
 * @brief	This routine frees the write buffer.
 */
void editor_buffer_free(struct append_buf *buf)
{
	free(buf->buffer);
}
