/**
 * @file:		src/main.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#include "terminal.h"

/**
 * @brief	Entry point.
 */
int main()
{
	terminal_enable_raw();

	for(;;);

	return 0;
}
