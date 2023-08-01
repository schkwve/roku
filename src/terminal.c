/**
 * @file:		src/terminal.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains support routines for
 * 				terminal and input/output functionality.
 */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "terminal.h"
#include "roku.h"

/**
 * @brief	This routine clears the terminal screen and repositions
 * 			the cursor to 0,0.
 */
void terminal_clear_screen()
{
	write(STDOUT_FILENO, "\x1b[2J]", 4);
	write(STDOUT_FILENO, "\x1b[H]", 3);
}

/**
 * @brief	This routine fetches the terminal window size and sets it
 * 			to the parameters
 * 
 * @param 	rows	pointer to integer
 * @param 	cols	pointer to integer
 * 
 * @return	status code
 */
int terminal_get_window_size(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B]]", 12) != 12) return -1;
		return terminal_get_curpos(rows, cols);
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

/**
 * @brief	Fetches the current cursor position
 * 
 * @param	rows	pointer to integer
 * @param	cols	pointer to integer
 * 
 * @return	status code
 */
int terminal_get_curpos(int *rows, int *cols)
{
	char buf[32];
	uint32_t i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n]", 4) != 4)
		return -1;

	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[')
		return -1;
	
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return -1;

	return -1;
}

/**
 * @brief	This routine saves current terminal flags,
 * 			enables raw mode and registers terminal_reset()
 * 			to run at exit.
 */
void terminal_enable_raw()
{
	// get current terminal flags
	if (tcgetattr(STDIN_FILENO, &roku_config.orig_termios) == -1) {
		die("tcgetattr: couldn't get terminal flags");
	}

	// register terminal_reset()
	atexit(terminal_reset);

	struct termios new_terminal_flags = roku_config.orig_termios;

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
 * @brief	This routine resets the terminal flags to their original
 * 			state saved in g_orig_terminal_flags.
 */
void terminal_reset()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &roku_config.orig_termios);
}
