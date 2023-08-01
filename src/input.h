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

/**
 * @brief	This routine reads keyboard input and returns it.
 */
char input_get_keypress();

/**
 * @brief	This routine handles keyboard input.
 */
void input_handle_keypress();

#endif // __INPUT_H_
