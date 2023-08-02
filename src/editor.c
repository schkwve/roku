/**
 * @file:		src/editor.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
			int len =
				roku_config.row[file_row].render_size - roku_config.col_off;
			if (len < 0) {
				len = 0;
			}

			if (len > roku_config.window_size.cols) {
				len = roku_config.window_size.cols;
			}

			editor_buffer_append(
				buf, &roku_config.row[file_row].render[roku_config.col_off],
				len);
		}

		editor_buffer_append(buf, "\x1b[K", 3);
		editor_buffer_append(buf, "\r\n", 2);
	}

	write(STDOUT_FILENO, "\x1b[H]", 3);
}

/**
 * @brief	This routine draws a status bar at the bottom of the terminal window
 */
void editor_draw_statusbar(struct append_buf *buf)
{
	editor_buffer_append(buf, "\x1b[7m", 4);

	char status[80];
	// this text is aligned to the right edge of the window.
	char rstatus[80];

	int len =
		snprintf(status, sizeof(status), "%.20s - %d lines%s",
				 roku_config.filename ? roku_config.filename : "[No Name]",
				 roku_config.num_rows,
				 roku_config.file_dirty ? " (modified)" : "");
	int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", roku_config.cy + 1,
						roku_config.num_rows);
	if (len > roku_config.window_size.cols) {
		len = roku_config.window_size.cols;
	}

	editor_buffer_append(buf, status, len);

	while (len < roku_config.window_size.cols) {
		if (roku_config.window_size.cols - len == rlen) {
			editor_buffer_append(buf, rstatus, rlen);
			break;
		} else {
			editor_buffer_append(buf, " ", 1);
			len++;
		}
	}
	editor_buffer_append(buf, "\x1b[m", 3);
	editor_buffer_append(buf, "\r\n", 2);
}

/**
 * @brief	This routine draws a message bar below the status bar
 */
void editor_draw_messagebar(struct append_buf *buf)
{
	editor_buffer_append(buf, "\x1b[K", 3);

	int msg_len = strlen(roku_config.status_msg);
	if (msg_len > roku_config.window_size.cols) {
		msg_len = roku_config.window_size.cols;
	}
	if (msg_len && time(NULL) - roku_config.status_msg_time < 5) {
		editor_buffer_append(buf, roku_config.status_msg, msg_len);
	}
}

/**
 * @brief	Sets the status message to be shown on the status bar.
 */
void editor_set_status(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(roku_config.status_msg, sizeof(roku_config.status_msg), fmt, ap);
	va_end(ap);
	roku_config.status_msg_time = time(NULL);
}

/**
 * @brief	This routine informs the editor_refresh_screen()
 * 			function to update the cursor position
 */
void editor_move_curpos(int key)
{
	editor_row_t *row;
	if (roku_config.cy >= roku_config.num_rows) {
		row = NULL;
	} else {
		row = &roku_config.row[roku_config.cy];
	}

	switch (key) {
	case ARROW_LEFT:
		if (roku_config.cx != 0) {
			roku_config.cx--;
		} else if (roku_config.cy > 0) {
			roku_config.cy--;
			roku_config.cx = roku_config.row[roku_config.cy].size;
		}
		break;
	case ARROW_RIGHT:
		if (row && roku_config.cx < row->size) {
			roku_config.cx++;
		} else if (row && roku_config.cx == row->size) {
			roku_config.cy++;
			roku_config.cx = 0;
		}
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

	// roku_config.cy could point to a different line
	// than before
	if (roku_config.cy >= roku_config.num_rows) {
		row = NULL;
	} else {
		row = &roku_config.row[roku_config.cy];
	}

	int row_len = row ? row->size : 0;
	if (roku_config.cx > row_len) {
		roku_config.cx = row_len;
	}
}

/**
 * @brief	This routine initializes the editor interface
 */
void editor_init()
{
	roku_config.cx = 0;
	roku_config.cy = 0;
	roku_config.render_x = 0;
	roku_config.row_off = 0;
	roku_config.col_off = 0;
	roku_config.num_rows = 0;
	roku_config.row = NULL;
	roku_config.file_dirty = 0;
	roku_config.filename = NULL;
	roku_config.status_msg[0] = '\0';
	roku_config.status_msg_time = 0;

	if (terminal_get_window_size(&roku_config.window_size.rows,
								 &roku_config.window_size.cols) == -1) {
		die("terminal_get_window_size: couldn't get window size");
	}

	// status bar & message bar
	roku_config.window_size.rows -= 2;
}

/**
 * @brief	This routine inserts a character into the current row.
 */
void editor_insert_char(int c)
{
	if (roku_config.cy == roku_config.num_rows) {
		editor_append_row(roku_config.num_rows, "", 0);
	}

	editor_insert_into_row(&roku_config.row[roku_config.cy], roku_config.cx, c);
	roku_config.cx++;
}

/**
 * @brief	This routine deletes a character from the current row
 */
void editor_remove_char()
{
	if (roku_config.cy == roku_config.num_rows) {
		return;
	}
	if (roku_config.cx == 0 && roku_config.cy == 0) {
		return;
	}

	editor_row_t *row = &roku_config.row[roku_config.cy];
	if (roku_config.cx > 0) {
		editor_remove_from_row(row, roku_config.cx - 1);
		roku_config.cx--;
	} else {
		roku_config.cx = roku_config.row[roku_config.cy - 1].size;
		editor_row_append_string(&roku_config.row[roku_config.cy - 1], row->buf, row->size);
		editor_remove_row(roku_config.cy);
		roku_config.cy--;
	}
}

/**
 * @brief	This routine removes a character from the current row buffer.
 */
void editor_remove_from_row(editor_row_t *row, int at)
{
	if (at < 0 || at >= row->size) {
		return;
	}

	memmove(&row->buf[at], &row->buf[at + 1], row->size - at);
	row->size--;
	editor_update_row(row);
	roku_config.file_dirty++;
}

/**
 * @brief	This routine reallocates the row buffer to fit a new character.
 */
void editor_insert_into_row(editor_row_t *row, int at, int c)
{
	if (at < 0 || at > row->size) {
		at = row->size;
	}

	row->buf = realloc(row->buf, row->size + 2);
	memmove(&row->buf[at + 1], &row->buf[at], row->size - at + 1);
	row->size++;
	row->buf[at] = c;
	editor_update_row(row);
	roku_config.file_dirty++;
}

/**
 * @brief	This routine appends a row to the render buffer
 */
void editor_append_row(int at, char *s, size_t len)
{
	if (at < 0 || at > roku_config.num_rows) {
		return;
	}

	roku_config.row = realloc(roku_config.row, sizeof(editor_row_t) *
												   (roku_config.num_rows + 1));
	memmove(&roku_config.row[at + 1], &roku_config.row[at], sizeof(editor_row_t) * (roku_config.num_rows - at));

	roku_config.row[at].size = len;
	roku_config.row[at].buf = malloc(len + 1);

	memcpy(roku_config.row[at].buf, s, len);

	roku_config.row[at].buf[len] = '\0';

	roku_config.row[at].render_size = 0;
	roku_config.row[at].render = NULL;

	editor_update_row(&roku_config.row[at]);

	roku_config.num_rows++;
	roku_config.file_dirty++;
}

/**
 * @brief	This routine appends a string to the specified row.
 */
void editor_row_append_string(editor_row_t *row, char *s, size_t len)
{
	row->buf = realloc(row->buf, row->size + len + 1);
	memcpy(&row->buf[row->size], s, len);
	row->size += len;
	row->buf[row->size] = '\0';
	editor_update_row(row);
	roku_config.file_dirty++;
}

/**
 * @brief	This routine frees the row buffer
 */
void editor_free_row(editor_row_t *row)
{
	free(row->render);
	free(row->buf);
}

/**
 * @brief	This routine removes a row
 */
void editor_remove_row(int at)
{
	if (at < 0 || at >= roku_config.num_rows) {
		return;
	}

	editor_free_row(&roku_config.row[at]);
	memmove(&roku_config.row[at], &roku_config.row[at + 1], sizeof(editor_row_t) * (roku_config.num_rows - at - 1));
	roku_config.num_rows--;
	roku_config.file_dirty++;
}

/**
 * @brief	This routine updates the render buffer of a row.
 */
void editor_update_row(editor_row_t *row)
{
	int tabs = 0;
	int i;

	for (i = 0; i < row->size; i++) {
		if (row->buf[i] == '\t')
			tabs++;
	}

	free(row->render);
	row->render = malloc(row->size + tabs * (TAB_WIDTH - 1) + 1);

	int idx = 0;
	for (i = 0; i < row->size; i++) {
		if (row->buf[i] == '\t') {
			row->render[idx++] = ' ';
			while (idx % TAB_WIDTH != 0) {
				row->render[idx++] = ' ';
			}
		} else {
			row->render[idx++] = row->buf[i];
		}
	}

	row->render[idx] = '\0';
	row->render_size = idx;
}

/**
 * @brief	This routine inserts a newline
 */
void editor_insert_newline()
{
	if (roku_config.cx == 0) {
		editor_append_row(roku_config.cy, "", 0);
	} else {
		editor_row_t *row = &roku_config.row[roku_config.cy];
		editor_append_row(roku_config.cy + 1, &row->buf[roku_config.cx], row->size - roku_config.cx);
		row = &roku_config.row[roku_config.cy];
		row->size = roku_config.cx;
		row->buf[row->size] = '\0';
		editor_update_row(row);
	}
	roku_config.cy++;
	roku_config.cx = 0;
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
	editor_draw_statusbar(&buf);
	editor_draw_messagebar(&buf);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "\x1b[%d;%dH",
			 (roku_config.cy - roku_config.row_off) + 1,
			 (roku_config.render_x - roku_config.col_off) + 1);
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
	roku_config.render_x = roku_config.cx;
	if (roku_config.cy < roku_config.num_rows) {
		roku_config.render_x = editor_row_cx_to_rx(
			&roku_config.row[roku_config.cy], roku_config.cx);
	}

	if (roku_config.cy < roku_config.row_off) {
		roku_config.row_off = roku_config.cy;
	}
	if (roku_config.cy >= roku_config.row_off + roku_config.window_size.rows) {
		roku_config.row_off = roku_config.cy - roku_config.window_size.rows + 1;
	}
	if (roku_config.render_x < roku_config.col_off) {
		roku_config.col_off = roku_config.render_x;
	}
	if (roku_config.render_x >=
		roku_config.col_off + roku_config.window_size.cols) {
		roku_config.col_off =
			roku_config.render_x - roku_config.window_size.cols + 1;
	}
}

/**
 * @brief	This routine converts the buffer index into a render index
 * 
 * @return	render_x value
 */
int editor_row_cx_to_rx(editor_row_t *row, int cx)
{
	int render_x = 0;

	for (int i = 0; i < cx; i++) {
		if (row->buf[i] == '\t') {
			render_x += (TAB_WIDTH - 1) - (render_x % TAB_WIDTH);
		}
		render_x++;
	}
	return render_x;
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
