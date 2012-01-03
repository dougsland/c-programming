/*
 * Copyright (C) 2012 Douglas Schilling Landgraf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Description:
 * This program will check if the mountpoint exists and write 'n' bytes into 
 * the nfs mountpoint.
 *
 * Compile: 
 * $ gcc -o nfs-check nfs-check.c -Wall
 *
 * Execute:
 * ./nfs-check /my-nfs-dir
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/statfs.h>

#define NFS_FS	0x6969  /* statfs defines that 0x6969 is NFS filesystem */

int main(int argc, char *argv[])
{
	int fd, ret;
	char str_write[] = "nfs test";
	char *fname = "/.tmp.txt";
	char *path;
	struct statfs stat_fs;

	if (argc != 2) {
		printf("Please use: %s pathname\n", argv[0]);
        	return EXIT_FAILURE;
	}

	path = (char *)calloc(strlen(argv[1]), sizeof(char));
	memset(path, '\0', strlen(path));
	strncpy(path, argv[1], strlen(argv[1]));
	strncat(path, fname, strlen(fname)); 

	if (statfs(argv[1], &stat_fs) == -1) {
		perror("Error statfs");
		printf("errno number %d\n", errno);
		return EXIT_FAILURE;
	}

	if (stat_fs.f_type != NFS_FS) {
		printf("pathname %s is not a valid mountpoint!\n", argv[1]);
		return EXIT_FAILURE;
	}

	fd = open(path, O_WRONLY|O_CREAT|O_TRUNC);
	if (fd == -1) {
		perror("Failed to open file");
		printf("errno number %d\n", errno);
		return EXIT_FAILURE;
	}

	ret = write(fd, str_write, strlen(str_write));
	if (ret == -1) {
		perror("Failed to write file");
		printf("errno number %d\n", errno);
		return EXIT_FAILURE;
	}

	printf("Written %d bytes\n", (int)strlen(str_write));

	close(fd);
	free(path);

	return 0;
}
