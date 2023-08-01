/**
 * @file:		src/roku.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#ifndef __ROKU_H_
#define __ROKU_H_

#include <termios.h>

typedef struct {
	struct termios orig_termios;
} roku_config_t;

/**
 * @brief	This function displays an error message and exits with status -1.
 */
void die(char *msg);

extern roku_config_t roku_config;

#endif // __ROKU_H_
