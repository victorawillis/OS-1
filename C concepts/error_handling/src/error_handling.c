#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "../include/error_handling.h"

int create_blank_records(Record_t **records, const size_t num_records)
{
	// check for bad parameters
	if (*records != NULL || num_records <= 0)
	{
		/*
		if (num_records < 0)
			return -2;
		*/
		return -1;
	}

	// *records is array of nodes of type "Record_t", each member of each node set to 0
	*records = (Record_t*) malloc(sizeof(Record_t) * num_records);

	// check that the record is no longer null
	if (*records == NULL)
	{
		return -2;
	}

	memset(*records,0,sizeof(Record_t) * num_records);
	return 0;	

	// verify initialization was successfull
	unsigned int i, j;
	for (i = 0; i < num_records; i++)
	{
		int check = 0;
		for (j =0; i < MAX_NAME_LEN; j++)
		{
			if (records[i]->name[j] != 0)
			{
				check = -1;
			}
		}
		if (records[i]->age != 0 || check == -1)
		{
			return -2;
		}
	}

	return 0;
}

int read_records(const char *input_filename, Record_t *records, const size_t num_records)
{
	// see that params are valid
	if (!input_filename || !records || num_records <= 0)
	{
		return -1;
	}

	// open file to read, check status
	int fr = open(input_filename, O_RDONLY);

	// -2: failed file opening
	if (fr == -1)
	{
		return -2;
	}

	// -3: failed file reading
	size_t i = 0;
  	size_t seeker = 0;

  	for (i = 0; i < num_records; ++i)
	{
		seeker = read(fr,&records[i], sizeof(Record_t));
		if (seeker <= 0 || seeker > 200)
		{
			return -3;
		}
	}

	return 0;
}

int create_record(Record_t **new_record, const char* name, int age)
{
	// keep age between [1, 200]
	if (age <= 0 || age > 200)
	{
		return -1;
	}

	// names can't be null, a newline, or too long
	if (name == NULL || (char)name[0] == '\n' || strlen(name) > MAX_NAME_LEN)
	{
		return -1;
	}

	if (*new_record != NULL)
	{
		return -1;
	}

	*new_record = (Record_t*) malloc(sizeof(Record_t));
	
	memcpy((*new_record)->name,name,sizeof(char) * strlen(name));
	(*new_record)->name[MAX_NAME_LEN - 1] = 0;	
	(*new_record)->age = age;
	return 0;

}
