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
		while (length > 0 && (line[length - 1] == '\n' ||
								line[length - 1] == '\r')) {
									length--;
								}

		roku_config.row = realloc(roku_config.row, sizeof(editor_row_t) * (roku_config.num_rows + 1));

		int at = roku_config.num_rows;
		roku_config.row[at].size = length;
		roku_config.row[at].buf = malloc(length + 1);
		memcpy(roku_config.row[at].buf, line, length);
		roku_config.row[at].buf[length] = '\0';
		roku_config.num_rows++;
	}
	free(line);
	fclose(fp);
}
