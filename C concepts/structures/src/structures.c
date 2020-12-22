#include "../include/structures.h"
#include <stdio.h>

int compare_structs(sample_t* a, sample_t* b)
{
	// members of structs must match
	if (a->a == b->a && a->b == b->b && a->c == b->c)
	{
		return 1;
	}
	return 0;
}

void print_alignments()
{
	printf("Alignment of int is %zu bytes\n",__alignof__(int));
	printf("Alignment of double is %zu bytes\n",__alignof__(double));
	printf("Alignment of float is %zu bytes\n",__alignof__(float));
	printf("Alignment of char is %zu bytes\n",__alignof__(char));
	printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
	printf("Alignment of short is %zu bytes\n",__alignof__(short));
	printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}

int sort_fruit(const fruit_t* a,int* apples,int* oranges, const size_t size)
{
	// see that params are valid
	if (!a || *apples < 0 || *oranges < 0 || size <= 0) 
	{
		return -1;
	}

	// loop array to check presence of apple and orange
	size_t i = 0;
	for (i = 0; i < size; ++i) 
	{
		if (a[i].type == 1) 
		{
			*apples += 1;
		} 
		else if (a[i].type == 2) 
		{
			*oranges += 1;
		}
	}
	return (int)size;
}

int initialize_array(fruit_t* a, int apples, int oranges)
{
	// see that params are valid
	if (!a || apples < 0 || oranges < 0) 
	{
		return -1;
	}

	// step through fruit array to assign apple count
	int i;
	int j;
	int k = 0;

	for (i = 0; i < apples; i++) 
	{
		a[k].type = 1;
		k++;
	}

	// continue through array to assign orange count
	for (j = 0; j < oranges; j++) 
	{
		a[k].type = 2;
		k++;
	}

	return 0;
}

int initialize_orange(orange_t* a)
{
	// check that a is valid
	if (!a)
	{
		return -1;
	}

	// assign members to orange
	a->type = 2;
	a->weight = 6;
	a->peeled = 9;
	return 0;

}

int initialize_apple(apple_t* a)
{
	// check that a is valid
	if (!a)
	{
		return -1;
	}

	// assign members to apple
	a->type = 1;
	a->weight = 21;
	a->worms = 0;
	return 0;
}
