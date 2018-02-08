#include "stdio.h"
#include "time.h"


#define MAX_LENGTH_ARRAY	15000
int main(int argc, char* argv[])
{
	int idx;
	int bound;
	int i;
	int* data = (int*) malloc(MAX_LENGTH_ARRAY * sizeof(int));
	int temp;
	clock_t s,e;

	if(data == NULL)
	{
		printf("Error allocate memory!\n");
		return 1;
	}


	srand(time(NULL));
	while(i < MAX_LENGTH_ARRAY)
	{
		if((rand()%2) % 2 == 0)
			data[i] = (rand() % 10000) * (-1);
		else
			data[i] = rand() % 10000;
		++i;
	}

	s = clock();
	/* Bubble sort */
	for(bound = MAX_LENGTH_ARRAY - 1; bound >= 0 ; --bound)
	{
		for(idx = 0; idx < bound; ++idx)
		{
			if(data[idx] > data[idx + 1])
			{
				temp = data[idx + 1];
				data[idx + 1] = data[idx];
				data[idx] = temp;
			}

		}
	}
	e = clock();

	printf("Size of buffer %ld byte\n", sizeof(data) * MAX_LENGTH_ARRAY);
	printf("Execution time : %f ms.\n", (double)(e - s)/CLOCKS_PER_SEC * 1000);
	free(data);


}