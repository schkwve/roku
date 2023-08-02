/**
 * @file:		src/find.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines to search queries
 */

#ifndef __FIND_H_
#define __FIND_H_

/**
 * @brief	This routine searches for a query and shows it if found.
 */
void find();

/** 
 * @brief	This routine is a callback to the find() function
 */
void *find_callback(char *query, int key);

#endif // __FIND_H_
