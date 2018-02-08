#include "stdio.h"
#include "time.h"

int is_prime_num(int n)
{
	int i;

	if(n % 2 == 0)
		return 0;
	else
		for(i = 3; i < n; i += 2)
		{
			if(n % i == 0)
				return 0;
		}
	return 1;
}

int random_prime()
{
	int p_n;
	while(1)
	{
		p = rand() %  
	}

}

int main(int argc, char* argv[])
{
	/* RSA Encryption */
	srand(time(NULL));


	return 0;
}