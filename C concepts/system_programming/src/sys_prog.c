#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/sys_prog.h"

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size)
{
	// see that params are valid
	if (!input_filename || !dst || offset > dst_size || dst_size < 1)
	{
		return false;
	}

	// see that filename is valid
	if (strcmp(input_filename, "\n") == 0 || strcmp(input_filename, "\0") == 0 || strcmp(input_filename, "") == 0)
	{
		return false;
	}

	// open file for reading
	int fstream = open(input_filename, O_RDONLY);

	if (fstream == -1) 
	{
		return false; 
	}
	else 
	{
		lseek(fstream, offset, SEEK_SET);
		if (read(fstream, dst, dst_size) == dst_size) 
		{
			if (close(fstream) != -1)
			{ 
				return true;
			}
		}
	}

	return false;
}

bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size)
{
	// see that params are valid
	if (!src || !output_filename || src_size < 1)
	{
		return false;
	}

	// see that filename is valid
	if (strcmp(output_filename, "\n") == 0 || strcmp(output_filename, "\0") == 0 || strcmp(output_filename, "") == 0)
	{ 
		return false;
	}

	// open file to write
	int fstream = open(output_filename, O_CREAT | O_TRUNC | O_WRONLY);

	if (fstream == -1) 
	{
		return false;
	} 
	else 
	{
		lseek(fstream, offset, SEEK_SET);
		if (write(fstream, src, src_size) == src_size) 
		{
			if (close(fstream) != -1) 
			{
				return true;
			}
		}
	}

	return false;
}

bool file_stat(const char *query_filename, struct stat *metadata)
{
	// valid params
	if (!query_filename || !metadata)
	{
		return false;
	}

	// see that file status is good
	if (stat(query_filename, metadata) == -1) 
	{
		return false;
	} 
	else
	{
		return true;
	}

	return false;
}

bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count)
{
	// valid params
	if (!src_data || !dst_data || src_count == 0)
	{ 
		return false;
	}

	// loop for mem shifts in src_count, temp for swap / shifts for a,b,c,d
	uint32_t temp;
	uint32_t a, b, c, d;

	for (int i = 0; i < src_count; ++i) 
	{
		a = (src_data[i] & 0x000000ff) << 24u;
		b = (src_data[i] & 0x0000ff00) << 8u;
		c = (src_data[i] & 0x00ff0000) >> 8u;
		d = (src_data[i] & 0xff000000) >> 24u;
		dst_data[i] = temp;

		// after each shift, logic OR unint32_t
		temp = a|b|c|d;
	}

	return true;
}