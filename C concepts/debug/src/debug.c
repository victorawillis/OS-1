#include "../include/debug.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// protected function, that only this .c can use
int comparator_func(const void *a, const void *b)
{
    return (long)a - *(long *)b;
}

bool terrible_sort(uint16_t *data_array, const size_t value_count)
{
    if (data_array == NULL || value_count <= 0)
    {
        return false;
    }

    uint16_t *new_array = (uint16_t *)malloc(sizeof(uint16_t) * value_count);

    size_t i;
    for (i = 0; i < value_count - 1; i++)
    {
        new_array[i] = data_array[i];
    }

    qsort(new_array, value_count, sizeof(uint16_t), comparator_func);

    bool status = true;
    for (i = 0; i < value_count - 1; i++)
    {
        status &= new_array[i] <= new_array[i + 1];
    }

    if (status == true)
    {
        memcpy(data_array, new_array, sizeof(uint16_t) * value_count);
    }

    free(new_array);

    return status;
}

