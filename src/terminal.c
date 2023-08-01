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
#include "roku.h"

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
	if (tcgetattr(STDIN_FILENO, &g_orig_terminal_flags) == -1) {
		die("tcgetattr: couldn't get terminal flags");
	}

	// register terminal_reset()
	atexit(terminal_reset);

	struct termios new_terminal_flags;

	// modify local terminal flags:
	//	~ECHO - no echoing
	//	~ICANON - read input byte-by-byte
	//	~IEXTEN - disable ctrl-v processing
	//			(also fixes ctrl-o on macOS)
	//	~ISIG - disable signal processing (SIGINT, ...)
	////////////////////////////////////////////////////////////////////////////
	// these flags are most likely useless and might be removed in the future
	// they are left for legacy terminals
	//	~BRKINT - a break condition will not cause a SIGINT anymore
	//	 CS8 - sets character size to 8 bits per byte
	new_terminal_flags.c_lflag &= ~(ECHO);
	new_terminal_flags.c_lflag &= ~(ICANON);
	new_terminal_flags.c_lflag &= ~(IEXTEN);
	new_terminal_flags.c_lflag &= ~(ISIG);

	new_terminal_flags.c_lflag &= ~(BRKINT);
	new_terminal_flags.c_lflag |=  (CS8);

	// modify input terminal flags;
	//	~IXON - disable software flow control processing
	//			(Ctrl-S, Ctrl-Q, ...)
	new_terminal_flags.c_iflag &= ~(IXON);

	// modify output terminal flags:
	//	~OPOST - disable output processing
	new_terminal_flags.c_oflag &= ~(OPOST);

	// modify control characters:
	//	VMIN - minimum number of bytes before read() can return
	//	VTIME - read() timeout (set in 1/10th of a second)
	new_terminal_flags.c_cc[VMIN] = 0;
	new_terminal_flags.c_cc[VTIME] = 1;
	
	// set updated terminal flags
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_terminal_flags) == -1) {
		die("tcsetattr: couldn't set terminal flags");
	}
}

/**
 * @brief	This function resets the terminal flags to their original
 * 			state saved in g_orig_terminal_flags.
 */
void terminal_reset()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_terminal_flags);
}
