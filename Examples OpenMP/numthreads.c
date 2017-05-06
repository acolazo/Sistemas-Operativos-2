#include <omp.h>
#include <stdio.h>


int main()
{
    //omp_set_num_threads(4);

#pragma omp parallel num_threads(8)
    {
        printf("Thread ID: %d\n", omp_get_thread_num());
    }

    return 0;
}
