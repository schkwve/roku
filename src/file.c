/**
 * @file:		src/file.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for file manipulation.
 */

#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "file.h"
#include "roku.h"
#include "editor.h"

/**
 * @brief	This routine opens the specified file
 * 			and displays its contents on the screen.
 */
void file_open(char *filename)
{
	free(roku_config.filename);
	roku_config.filename = strdup(filename);

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

/**
 * @brief	Saves the buffer into a file.
 */
void file_save()
{
	// @todo Add a filename prompt
	if (roku_config.filename == NULL)
		return;
	
	int len;
	char *buf = file_rows_to_string(&len);

	int fd = open(roku_config.filename, O_RDWR | O_CREAT, 0644);
	ftruncate(fd, len);
	write(fd, buf, len);
	close(fd);
	free(buf);
}

/**
 * @brief	This routine converts all row buffers to a single string.
 * 
 * @return	Converted string buffer
 */
char *file_rows_to_string(int *buflen)
{
	int len = 0;
	int i;

	for (i = 0; i < roku_config.num_rows; i++) {
		len += roku_config.row[i].size + 1;
	}

	*buflen = len;
	char *buf = malloc(len);
	char *p = buf;

	for (i = 0; i < roku_config.num_rows; i++) {
		memcpy(p, roku_config.row[i].buf, roku_config.row[i].size);
		p += roku_config.row[i].size;
		*p = '\n';
		p++;
	}

	return buf;
}
