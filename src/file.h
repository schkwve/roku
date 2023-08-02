/**
 * @file:		src/file.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for file manipulation.
 */

#ifndef __FILE_H_
#define __FILE_H_

/**
 * @brief	This routine opens the specified file
 * 			and displays its contents on the screen.
 */
void file_open(char *filename);

/**
 * @brief	Saves the buffer into a file.
 */
void file_save();

/**
 * @brief	This routine converts all row buffers to a single string.
 * 
 * @return	Converted string buffer
 */
char *file_rows_to_string(int *buflen);

#endif // __FILE_H_
