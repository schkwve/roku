/**
 * @file:		src/editor.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#ifndef __EDITOR_H_
#define __EDITOR_H_

#include "roku.h"

#define APPEND_BUF_INIT {NULL, 0}

/**
 * @brief	Write buffer
 */
struct append_buf {
	char *buffer;
	int size;
};

/**
 * @brief	This routine draws every row on the screen.
 * 			If a row hasn't been specified to be drawn,
 * 			the first character of it is replaced by a tilde (~).
 */
void editor_draw_row(struct append_buf *buf);

/**
 * @brief	This routine draws a status bar at the bottom of the terminal window
 */
void editor_draw_statusbar(struct append_buf *buf);

/**
 * @brief	This routine draws a message bar below the status bar
 */
void editor_draw_messagebar(struct append_buf *buf);

/**
 * @brief	This routine displays a prompt in the message bar.
 * 
 * @return	User's input.
 */
char *editor_display_prompt(char *prompt, void *(callback)(char *, int));

/**
 * @brief	Sets the status message to be shown on the status bar.
 */
void editor_set_status(const char *fmt, ...);

/**
 * @brief	This routine informs the editor_refresh_screen()
 * 			function to update the cursor position
 */
void editor_move_curpos(int key);

/**
 * @brief	This routine initializes the editor interface
 */
void editor_init();

/**
 * @brief	This routine inserts a character into the current row.
*/
void editor_insert_char(int c);

/**
 * @brief	This routine deletes a character from the current row
 */
void editor_remove_char();

/**
 * @brief	This routine removes a character from the current row buffer.
 */
void editor_remove_from_row(editor_row_t *row, int at);

/**
 * @brief	This routine reallocates the row buffer to fit a new character.
 */
void editor_insert_into_row(editor_row_t *row, int at, int c);

/**
 * @brief	This routine appends a row to the render buffer
 */
void editor_append_row(int at, char *s, size_t len);

/**
 * @brief	This routine appends a string to the specified row.
 */
void editor_row_append_string(editor_row_t *row, char *s, size_t len);

/**
 * @brief	This routine frees the row buffer
 */
void editor_free_row(editor_row_t *row);

/**
 * @brief	This routine removes a row
 */
void editor_remove_row(int at);

/**
 * @brief	This routine updates the render buffer of a row.
 */
void editor_update_row(editor_row_t *row);

/**
 * @brief	This routine inserts a newline
 */
void editor_insert_newline();

/**
 * @brief	This routine is called upon screen refresh.
 */
void editor_refresh_screen();

/**
 * @brief	This routine handles buffer scrolling
 * 			if requested.
 */
void editor_handle_scrolling();

/**
 * @brief	This routine converts the buffer index into a render index
 * 
 * @return	render_x value
 */
int editor_row_cur_x_to_rx(editor_row_t *row, int cur_x);

/**
 * @brief	This routine converts the render index into a buffer index
 * 
 * @return	cur_x value
 */
int editor_row_rx_to_cur_x(editor_row_t *row, int rx);

/**
 * @brief	This routine appends a string to the write buffer
 */
void editor_buffer_append(struct append_buf *buf, const char *s, int len);

/**
 * @brief	This routine frees the write buffer.
 */
void editor_buffer_free(struct append_buf *buf);

#endif // __EDITOR_H_
