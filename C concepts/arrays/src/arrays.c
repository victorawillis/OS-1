#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count)
{
    // check for valid parameters
    if (!src || !dst || elem_size == 0 || elem_count == 0)
    {
        return false;
    }

    // see that array contains valid elements
    if (memcpy(dst, src, elem_size * elem_count) != NULL)
    {
        return true;
    }

    // else array is invalid
    return false;
}

bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count)
{
    // is array initialized
    if (!data_one || !data_two || elem_size == 0 || elem_count == 0)
    {
        return false;
    }

    // if memcmp returns 0 the arrays are equal
    if (memcmp(data_one, data_two, elem_size * elem_count) == 0)
    {
        return true;
    }

    // else they are not
    return false;
}

ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count)
{
    // failure if array is not initialized
    if (!data || !target || elem_size == 0 || elem_count == 0)
    {
        return -1;
    }

    // scroll array for target index
    const char *temp = (char *) data;
    int i;
    for (i = 0; i < elem_count; i++)
    {
        if (memcmp(((elem_size * i) + temp), target, elem_size) == 0)
        {
            return i;
        }
    }

    // else, locate failed
    return -1;
}

bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count)
{
    // is array initialized
    if (!src_data || !dst_file || elem_size == 0 || elem_count == 0) 
    {
		return false;
	}

    // catch filename error, if it's blank or a newline
    int blank = strcmp(dst_file, "");
    int nline = strcmp(dst_file, "\n");

    if (blank == 0 || nline == 0)
    {
        return false;
    }

    // see that file is valid
    FILE *fp;
    if ((fp = fopen(dst_file, "wb")) != NULL)
    {
        // see that file write works
        if (fwrite((const char *) src_data, 1, (elem_count * elem_size), fp) != elem_count * elem_size)
        {
            return false;
        }

        fclose(fp);
        return true;
    }

    // else, binary write failed
    return false;
}

bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count)
{
    // see that params are valid
    if (!src_file || !dst_data || strcmp(src_file, "") == 0 || strcmp(src_file, "\n")== 0 || elem_size * elem_count == 0) 
    {
        return false;
    }

    // open file to read
    FILE* fp = fopen(src_file, "r");
    fread(dst_data, elem_size, elem_count, fp);
    fclose(fp);

	return true;
}

