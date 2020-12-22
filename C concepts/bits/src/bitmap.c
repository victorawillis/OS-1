#include "../include/bitmap.h"

// data is an array of uint8_t and needs to be allocated in bitmap_create
//      and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create


bitmap_t *bitmap_create(size_t n_bits)
{
    // valid params
    if (n_bits < 1 || n_bits > SIZE_MAX-1) 
    {
		return NULL; 
	} 
    else 
    {
        size_t fin;
		size_t byte_no;
		
        // allocate mem for bytes with error check
		bitmap_t *p = (bitmap_t *)malloc(sizeof(bitmap_t));
		if (p == NULL)
            return NULL;

        // if bits align with bytes / 8, allocate mem for bits * 8
		if (n_bits % 8 == 0)
        {
			byte_no = n_bits / 8; // set the number of bytes equal to the bumber of bits divided by 8
			p->data = (uint8_t *) malloc(sizeof(uint8_t) * byte_no);
			fin = byte_no;
		} 

        // if bits do not align with bytes / 8 case, allocate mem for another byte to store leftover bits
        else
        {
			byte_no = ( 1 + (n_bits / 8));
			p->data = (uint8_t *)malloc(sizeof(uint8_t) * byte_no);
			fin = 1 + byte_no;
		}

        // init bytes array, store data, return its address
		size_t i;
		for (i = 0; i < fin; i++)
        {
			p->data[i] = 0;
		}

        p->byte_count = byte_no;
		p->bit_count = n_bits;
		
		return p;
	}

	return NULL;
}

bool bitmap_set(bitmap_t *const bitmap, const size_t bit)
{
	// check args for validity
	if (!bitmap || bit > SIZE_MAX-1 || bit > bitmap->bit_count)
        return false;
    
    // set bit / byte arrangement
	size_t i = bit / 8;
	size_t set = bit % 8;

    // set up flag and shift bits 
	uint8_t flag = 1;
	flag = flag << set;
	bitmap->data[i] = bitmap->data[i] | flag;

	return true;
}

bool bitmap_reset(bitmap_t *const bitmap, const size_t bit)
{
        // check for valid args
    if (!bitmap || bit > SIZE_MAX-1 || bit > bitmap->bit_count)
    { 
        return false;
    }

    // setup bit / byte arrangement
	size_t i = bit / 8;
	size_t set = bit % 8;

    // set flag / shift bits
	uint8_t flag = 1;
	flag = flag << set;
	flag = ~flag;

    // logical and bit / flag to init
	bitmap->data[i] = bitmap->data[i] & flag;
	return true;
}

bool bitmap_test(const bitmap_t *const bitmap, const size_t bit)
{
// check args validity
    if (!bitmap || bit > SIZE_MAX-1 || bit > bitmap->bit_count)
    {  
        return false;
    }

    // setup bit & byte arrangement
 	size_t i = bit / 8;
	size_t set = bit % 8;

    // set flag and shift bits
    uint8_t flag = 1;
	flag = flag << set;

    // logical and bit / flag for success or error
	if (bitmap->data[i] & flag)
    {
		return true;
    }
	else
    {
		return false;
    }
}

size_t bitmap_ffs(const bitmap_t *const bitmap)
{
    // check for valid args
	if (!bitmap)
    {
        return SIZE_MAX;
    }

    // scan low bit to high bit to seek value 1 set via bitmap_test
	size_t total_bits = bitmap->bit_count;
    int i;
	for (i=total_bits-1;i>=0;i--)
    {
		if (bitmap_test(bitmap, i))
        {
            return (size_t) i;
        }
	}

	return SIZE_MAX;
}

size_t bitmap_ffz(const bitmap_t *const bitmap)
{
    // check for valid pointer
	if (!bitmap)
    {
        return SIZE_MAX;
    }

    // iterate bits, return ith bit checked
	size_t i;
	for (i = 0; i < bitmap->bit_count; i++)
    {
		if(!bitmap_test(bitmap,i))
        {
            return i;
        }
	}

	return SIZE_MAX;
}

bool bitmap_destroy(bitmap_t *bitmap)
{
    // check for valid bitmap ptr then free data
	if (!bitmap) 
    {
        return false;
    }
    
	free(bitmap->data);
	free(bitmap);

	return true;
}
