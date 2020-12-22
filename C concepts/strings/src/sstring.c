#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sstring.h"

// string duplicator
char *stdup(const char *src, int length)
{
	char *dst = malloc(strlen (src) + 1);
	if (dst == NULL) 
	{   
		return NULL;
	}         
	strcpy(dst, src);                      
	return dst;                            
}

bool string_valid(const char *str, const size_t length)
{
	// is string null or empty
	if (!str || length < 1)
	{
		return false;
	}

	// is string's last element the null terminator
	if (*(str + (length -1)) == '\0')
	{
		return true;
	}

	// else string is invalid
	else
	{
	    return false;
	}
}

char *string_duplicate(const char *str, const size_t length)
{
	// is string null or empty
	if (!str || length < 1)
	{
		return NULL;
	}
	// create copy of string and return its address
	else
	{
		char * s = stdup((char *) str, length);
		return s;
	}
	// return null if unsuccessful 
	return NULL;
}

bool string_equal(const char *str_a, const char *str_b, const size_t length)
{
    // see that params are valid
	if (!str_a || !str_b || length == 0)
    {
        return false;
    }

    // compare strings, else comparison was not equal
    if (!strcmp(str_a, str_b))
    {
        return true;
    }

    return false;
}

int string_length(const char *str, const size_t length)
{
	// is string null or empty
	if (!str || length < 1)
	{
		return -1;
	}

	// calculate length and return
	int size = strlen(str);
	return size;

}

int string_tokenize(const char *str, const char *delims, const size_t str_length,char **tokens, const size_t max_token_length, const size_t requested_tokens)
{
    // see that params are valid
    if (!str || !delims || str_length == 0 || !tokens || max_token_length == 0 || requested_tokens == 0)
	{
		return 0;
	}

    // is tokens array properly allocated
    int i;
    for (i = 0; i < requested_tokens; i++)
    {
        if (!tokens[i])
        {
            return -1;
        }
    }
    
    // array for tokens
    char* temp = malloc(sizeof(str));
    strncpy(temp, str, str_length);
    char* tok2 = strtok(temp, delims);

    // initiate tokens array
    strcpy(tokens[0], tok2);

    // iterate tokens array, temp stores string, token array counts delims
    int j = 0;
    while (j < requested_tokens - 1)
    {
        j++;
        tok2 = strtok(NULL, delims); //Go to the next token
        strcpy(tokens[j], tok2);
    }

    free(temp);
    return requested_tokens;
}

bool string_to_int(const char *str, int *converted_value)
{   
    // check params
	if(!str || !converted_value)
    {
        return false;
    }

    // see that string to int converts successfully
    if (atoi(str) == -1)
    {
        return false;
    }
    // if successful set and return true
    else 
    {
        *converted_value = atoi(str);
    }
	return true;
	
}