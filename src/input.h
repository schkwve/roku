/**
 * @file:		src/input.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to handle
 * 				keyboard input.
 */

#ifndef __INPUT_H_
#define __INPUT_H_

#define CTRL_KEY(k) ((k) & 0x1f)

enum input_keys {
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN
};

/**
 * @brief	This routine reads keyboard input and returns it.
 */
int input_get_keypress();

/**
 * @brief	This routine handles keyboard input.
 */
void input_handle_keypress();

#endif // __INPUT_H_
