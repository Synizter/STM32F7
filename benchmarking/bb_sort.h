#ifndef __BB_SORT_H__
#define __BB_SORT_H__

#include <stdint.h>

#define MAX_ARRAY_LEN           64 * 4
#define RAND_RANGE              256

uint8_t bubble_sort(uint8_t* ptData, int size);
void BubbleSort_Start(void);

void BubbleSort(uint8_t* data);
void Sensor_Read(uint8_t* data);

#endif