#include "bb_sort.h"
#include <time.h>

static uint8_t rd_array[MAX_ARRAY_LEN];

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

void BubbleSort_Start(void)
{
  int i;
  srand(time(NULL));
  for (i = 0; i < MAX_ARRAY_LEN; ++i)
  {
    rd_array[i] = rand()%RAND_RANGE + 1;
  }
  
  bubble_sort(rd_array, MAX_ARRAY_LEN);
}

