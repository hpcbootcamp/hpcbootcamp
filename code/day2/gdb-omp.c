#include <stdlib.h>
#include <stdio.h>

int main() {
    int i = 0, t = 0;
    size_t max = omp_get_max_threads();
    int * array = (int *) calloc(sizeof(int), max);

    #pragma omp parallel private(t)
    {
        /* TUTORIAL: Set your initial breakpoint at the next line. Notice how some threads aren't out of the barrier yet. */
        t = omp_get_thread_num();

        #pragma omp for
        for (i = 0; i < 10000000; ++i) {
            /* TUTORIAL: Set a breakpoint here for thread 1, display the array, and use continue to iterate through the
               TUTORIAL: loop on thread 1. How do the contents change between breakpoints? */
            ++array[t];
        }
    }


    printf("array == {");
    for (i = 0; i < max - 1; ++i) {
        printf("%d, ", array[i]);
    }
    printf("%d}\n", array[max - 1]);

    return 0;
}
