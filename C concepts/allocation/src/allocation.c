#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>


void* allocate_array(size_t member_size, size_t nmember,bool clear)
{
    // see that params are valid
	if (member_size == 0 || nmember == 0)
    {
        return NULL;
    }

    // calloc memory for array if clear is true
	if (clear == true)
    {
        return calloc(nmember, member_size);
    }

    // if clear is false, malloc
    else
    {
        return malloc(member_size*nmember);
    }
}

void* reallocate_array(void* ptr, size_t size)
{
    // check for valid array
	if (!ptr)
    {
        return NULL;
    }

    // return reallocated array
	return realloc(ptr, size);
}

void deallocate_array(void** ptr)
{
    // see that array is valid
	if (!*ptr)
    {
        return;
    }

    // free the memory array is pointing at, initiate the array to NULL
	free(*ptr);
	*ptr = NULL;

	return;
}

char* read_line_to_buffer(char* filename)
{
    // check that filename is valid
	if (!filename)
    {
        return NULL;
    }

    // Open file, create dynamic array to hold lines of characters in file
	FILE* fptr = fopen(filename, "r");
	char* buff = malloc(BUFSIZ * sizeof(char));

    // Read through lines in the buffer and store in the array
	fgets(buff, BUFSIZ, fptr);
	fclose(fptr);
	return buff;
}
