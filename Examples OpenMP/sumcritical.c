#include <omp.h>
#include <stdio.h>


int main()
{
    unsigned long long sum = 0;
    unsigned long long i;

#pragma omp parallel num_threads(4) private(i)
    {
//#pragma omp critical
        // Final sum = 262144
        for (i = 0; i < (1<<16); ++i) sum += 1;
    }

    printf("sum=%d\n", sum);

    return 0;
}
