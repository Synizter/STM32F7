#include "bb_sort.h"


void swap(uint8_t* a, uint8_t* b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

 uint8_t bubble_sort(uint8_t* ptData, int size)
 {
    int idx;
    int bound;

    for(bound = size - 1; bound >= 0 ; --bound)
    {
        for(idx = 0; idx < bound; ++idx)
        {
            if(ptData[idx] > ptData[idx + 1])
            {
                swap(&ptData[idx], &ptData[idx + 1]);
            }
        }
    }

 	return 1;

 }
