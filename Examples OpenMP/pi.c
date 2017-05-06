#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main()
{
    unsigned long long sum_circle = 0;
    unsigned long long sum_total = 0;
    double x, y, pi;
    unsigned long long i;

    srand(time(NULL));

#pragma omp parallel num_threads(4) private(i)
    {
        for (i = 0; i < (1<<16); ++i) {
            x = ((double) rand()) / ((double) RAND_MAX);
            y = ((double) rand()) / ((double) RAND_MAX);
            if ( (pow(x, 2.0) + pow(y, 2.0)) <= 1.0 ) {
                sum_circle += 1;
            }
            sum_total += 1;
        }
    }

    pi = 4.0 * ((double) sum_circle) / ((double) sum_total);
    printf("pi=%f\n", pi);

    return 0;
}
