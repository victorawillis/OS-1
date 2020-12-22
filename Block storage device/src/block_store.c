#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


// define block store members
struct block_store 
{
	unsigned char *user_data;
	bitmap_t *bmt;
};

block_store_t *block_store_create()
{
    // make the block store
    block_store_t *bs = malloc(sizeof(block_store_t));

    // see that bs declaration successful
    if (!bs)
	{
        return NULL;
    }

    // create sapce for user data
    bs->user_data = calloc(BLOCK_STORE_NUM_BLOCKS, BLOCK_SIZE_BYTES);

    // see that space correctly allocated
    if (!bs->user_data)
	{
        block_store_destroy(bs);
        return NULL;
    }

    // initialize bitmap
    bs->bmt = bitmap_overlay(BITMAP_START_BLOCK, bs->user_data);

    return bs;
}

void block_store_destroy(block_store_t *const bs)
{
    // see that bs isn't null
	if (!bs) 
	{
		return;
	}

	// destroy bitmap
	bitmap_destroy(bs->bmt);

	// free user data and the bs itself
	free(bs->user_data);
	free(bs);

	return;
}

size_t block_store_allocate(block_store_t *const bs)
{
    // see that bs is not null
	if (!bs) 
	{
		return SIZE_MAX;
	}

	// index is first available bitmap in the block store
	size_t index = bitmap_ffz(bs->bmt);

	// if the first available zero is greater than the total blocks, the max size should be allocated
	if (index >= BLOCK_STORE_NUM_BLOCKS) 
	{
		return SIZE_MAX;
	}

	// update designated bit in block store
	bitmap_set(bs->bmt, index);

	return index;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    // see that params are valid
	if (!bs || 0 == block_id || block_id > BLOCK_STORE_NUM_BLOCKS || bitmap_test(bs->bmt, block_id)) 
	{
		return false;
	}

	// set bits of target block id to unavailable
	bitmap_set(bs->bmt, block_id);
	return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    // see that bs is not null and block_id is in bounds
	if (!bs || block_id > BLOCK_STORE_NUM_BLOCKS) 
	{
		return;
	}

	// set bits to 0 to release the space
	bitmap_reset(bs->bmt, block_id);
	return;
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    // see that bs is not null
	if (!bs) 
	{
		return SIZE_MAX;
	}

	// size to return is of of unavailable bits
	return bitmap_total_set(bs->bmt);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    // see that bs is not null
	if (!bs) 
	{
		return SIZE_MAX;
	}

	// size to return is all blocks less unavailable ones
	return BLOCK_STORE_NUM_BLOCKS - bitmap_total_set(bs->bmt);
}

size_t block_store_get_total_blocks()
{
	// get all the blocks
	return BLOCK_STORE_NUM_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{

	// see that params are valid
    if (!bs || block_id > block_store_get_total_blocks() || !buffer)
	{
        return 0;
    }

    // check that bit is set unavailable for designated block
    if (bitmap_test(bs->bmt, block_id) == 0)
	{
        return 0;
    }
	// write user data to buffer
    else
	{
        memcpy(buffer, ((BLOCK_SIZE_BYTES * block_id) + bs->user_data), BLOCK_SIZE_BYTES);

		// return the size that has been written
        return BLOCK_SIZE_BYTES;
    }
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    // see that params are valid
	if (!bs || block_store_get_total_blocks() <= block_id || !buffer)
	{
		return 0;
	}

	// check bitmap availability 
	if (bitmap_test(bs->bmt, block_id) == 0)
	{
		return 0;
	}
	// write the data to the designated block
	else
	{
		memcpy(((block_id * BLOCK_SIZE_BYTES) + bs->user_data), buffer, BLOCK_SIZE_BYTES);

		// upon successful write, return byte size of blocks that have been written to
		return BLOCK_SIZE_BYTES;
	}
}

block_store_t *block_store_deserialize(const char *const filename)
{
    // see that name is valid
    if (!filename)
	{
        return NULL;
    }

    // create descriptor for the file to read
    int desc = open(filename, O_RDONLY);

    // see that file could be opened
    if(desc < 0) 
	{
        return NULL;
    }

    // make instance of block store to send data
    block_store_t* bs = block_store_create();

    // read from the file and place in block store
    size_t check;
	check = read(desc, bs->user_data, BLOCK_STORE_NUM_BYTES);

    // see that data stored successfully
    if(check == 0)
	{
        block_store_destroy(bs);
        return NULL;
    }

	// return the instace of block store
    return bs;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    // see that params are valid
	if (!bs || !filename)
	{
		return 0;
	}

	// create descriptor to open file for writing
	int desc = open(filename, O_WRONLY);

	// see that file open was successful
	if (desc < 0)
	{
		return 0;
	}

	// data from instace of bs written to file
	size_t check;
	check = write(desc, bs->user_data, BLOCK_STORE_NUM_BYTES);

	// see that data was successfully written
	if (check == 0)
	{
		return 0;
	}

	// return size of data written
	return check;
}
