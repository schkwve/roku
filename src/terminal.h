/**
 * @file:		src/terminal.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains support routines for
 * 				terminal and input/output functionality.
 */

#ifndef __TERMINAL_H_
#define __TERMINAL_H_

/**
 * @brief	This function saves current terminal flags,
 * 			enables raw mode and registers terminal_reset()
 * 			to run at exit.
 */
void terminal_enable_raw();

/**
 * @brief	This function resets the terminal flags to their original
 * 			state saved in g_orig_terminal_flags.
 */
void terminal_reset();

#endif // __TERMINAL_H_
