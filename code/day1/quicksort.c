#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define EXP 6
#define SIZE (1 << EXP)
#define SWAP(a, b) { tmp = a; a = b; b = tmp; }
int comparisons = 0;

/* TUTORIAL: The fill() and init() functions can be ignored. */
/** Fill an array of size n with values between 0 and n. */
void fill(int * array, size_t n) {
    int i;

    for (i = 0; i < n; ++i) {
	array[i] = random() % n;
    }
}

/** Initialize the random number generator using the current time and fill the array. */
void init(int * array, size_t n) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec);
    fill(array, SIZE);
}

/** Partition the array in to values less than and greater to the pivot, and return the middle index. */
int partition(int * array, size_t n, int pivot) {
    int i, tmp;

    SWAP(array[n - 1], array[pivot]);

    int index = 0;
    for (i = 0; i < n - 1; ++i) {
	++comparisons;
	if (array[i] < array[n - 1]) {
	    SWAP(array[i], array[index]);
	    ++index;
	}
    }

    SWAP(array[n - 1], array[index]);

    return index;
}

/** Sort the given array in ascending order. */
void quicksort(int * array, size_t n) {
    /* TUTORIAL: You can see the entire array here by typing "print n" to get the value of n,
       TUTORIAL: followed by "print (int[XXX])*array", where XXX equals the current value of n. */
    int pivot;

    if (n < 2) return;
    pivot = random() % n;
    int middle = partition(array, n, pivot);

    /* TUTORIAL: What happens when you use the step command here? How about the next command? */
    /* NOTE: Incidentally, this step can be parallelized with the SECTIONS and SECTION directives of OpenMP */
    quicksort(array, middle);
    quicksort(array + middle + 1, n - middle - 1);
}

int main(int argc, char ** argv) {
    int i;
    int array[SIZE];

    init(array, SIZE);

    /* TUTORIAL: Consider setting your first breakpoint here to skip past the initialization routine. */
    quicksort(array, SIZE);

    printf("%d comparisons to sort array\n", comparisons);
    printf("average case %d comparisons\n", SIZE * EXP);
    printf("worst case %d comparisons\n", SIZE * SIZE);

    return 0;
}
