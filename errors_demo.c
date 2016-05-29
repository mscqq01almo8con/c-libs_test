/* File: errors_demo.c */
#include <stdio.h>// For fopen, printf
#include <stdlib.h>// For malloc
#include <errno.h>// For errno variable

main()
{
	FILE *fp; char *p; int stat;
	fp = fopen("non_existent_file", "r");
	if (fp == NULL) 
	{
	// Check for error
		printf("errno = %d\n", errno);
		perror("fopen"); 
	}
	p = malloc(4000000000U);
	if (p == NULL) 
	{
		// Check for error
		printf("errno = %d\n", errno);
		perror("malloc"); 
	}
	// BE CAREFUL: unlink tries to remove a file
	stat = unlink("/etc/motdasda123@");
	if (stat == -1) 
	{
		// Check for error
		printf("errno = %d\n", errno);
		perror("unlink"); 
	}
}
