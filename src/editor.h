/**
 * @file:		src/editor.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#ifndef __EDITOR_H_
#define __EDITOR_H_

#define APPEND_BUF_INIT {NULL, 0}

/**
 * @brief	Write buffer
 */
struct append_buf {
	char *buffer;
	int size;
};

/**
 * @brief	This routine draws tildes at the beginning of every line
 */
void editor_draw_row_tildes(struct append_buf *buf);

/**
 * @brief	This routine initializes the editor interface
 */
void editor_init();

/**
 * @brief	This routine is called upon screen refresh.
 */
void editor_refresh_screen();

/**
 * @brief	This routine appends a string to the write buffer
 */
void editor_buffer_append(struct append_buf *buf, const char *s, int len);

/**
 * @brief	This routine frees the write buffer.
 */
void editor_buffer_free(struct append_buf *buf);

#endif // __EDITOR_H_
