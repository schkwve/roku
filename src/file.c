/**
 * @file:		src/file.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for file manipulation.
 */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "file.h"
#include "roku.h"
#include "editor.h"

/**
 * @brief	This routine opens the specified file
 * 			and displays its contents on the screen.
 */
void file_open(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		die("fopen: couldn't open file");
	}

	char *line = NULL;
	size_t linecap = 0;
	ssize_t length;
	while ((length = getline(&line, &linecap, fp)) != -1) {
		while (length > 0 &&
			   (line[length - 1] == '\n' || line[length - 1] == '\r')) {
			length--;
		}

		editor_append_row(line, length);
	}
	free(line);
	fclose(fp);
}
