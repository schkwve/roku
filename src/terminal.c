/**
 * @file:		src/terminal.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains support routines for
 * 				terminal and input/output functionality.
 */

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

#include "terminal.h"

/**
 * @brief	This variable contains the original terminal flags state.
 */
struct termios g_orig_terminal_flags;

/**
 * @brief	This function saves current terminal flags,
 * 			enables raw mode and registers terminal_reset()
 * 			to run at exit.
 */
void terminal_enable_raw()
{
	// get current terminal flags
	tcgetattr(STDIN_FILENO, &g_orig_terminal_flags);
	atexit(terminal_reset);

	struct termios new_terminal_flags;

	// modify terminal flags:
	//	~ECHO - no echoing
	//	~ICANON - read input byte-by-byte
	new_terminal_flags.c_lflag &= ~(ECHO);
	new_terminal_flags.c_lflag &= ~(ICANON);
	
	// set updated terminal flags
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_terminal_flags);
}

/**
 * @brief	This function resets the terminal flags to their original
 * 			state saved in g_orig_terminal_flags.
 */
void terminal_reset()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_terminal_flags);
}
