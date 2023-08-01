/**
 * @file:		src/terminal.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains support routines for
 * 				terminal and input/output functionality.
 */

#ifndef __TERMINAL_H_
#define __TERMINAL_H_

typedef struct {
	int rows;
	int cols;
} terminal_winsize_t;

/**
 * @brief	This routine clears the terminal screen and repositions
 * 			the cursor to 0,0.
 */
void terminal_clear_screen();

/**
 * @brief	This routine fetches the terminal window size and sets it
 * 			to the parameters
 * 
 * @param 	rows	pointer to integer
 * @param 	cols	pointer to integer
 * 
 * @return	status code
 */
int terminal_get_window_size(int *rows, int *cols);

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
