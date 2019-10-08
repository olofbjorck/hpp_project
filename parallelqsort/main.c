#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> // For timings
#include <limits.h> // For max int
#include "parallelqsort.h"

static void checkSorted(int arr[], int arrsize);

static void printArrSample(int arr[], int arrsize);

/**
 * Main function
 *
 */
int main(int argc, char const *argv[]) {

	// Check proper number of input arguments
	if (argc < 3 || argc > 4) {
		printf("%s\n", "INPUT ERROR: Expected input:\t"
			"./parallelqsort_demo arrsize nthreads "
			"\n\t\t\t\tor "
			"\n\t\t\t\t./parallelqsort_demo arrsize nthreads seed ");
		return 1;
	}

	printf("\nStarting demonstration of parallel quicksort ...\n");
	// Read input from command line
	const int arrsize = atoi(argv[1]);
	const int nthreads = atoi(argv[2]);
	int seedInt;
	if (argc == 4) {
		seedInt = atoi(argv[3]);
	} else {
		seedInt = 100;
	}
	const int seed = seedInt;
	printf("nthreads = %d\n", nthreads);
	printf("arrsize = %d\n", arrsize);

	// Loop index
	int i;

	// Create array and fill with random integers between 0 and 1000
	printf("\nCreating arrRandom ...\n");
	int* arrRandom = (int*) malloc(arrsize * sizeof(int));
	srand(seed);
	for (i = 0; i < arrsize; i++) {
		arrRandom[i] = rand() % 1000;
	}
	printf("%s\n", "... arrRandom created");

	// Print sample of arrRandom
	printArrSample(arrRandom, arrsize);
	checkSorted(arrRandom, arrsize);

	// Create timings
	struct timeval  tv1, tv2;
	double extime;

	// Get time before start
	gettimeofday(&tv1, NULL);

	printf("\nCalling parallelqsort(arrRandom, %d, %d) ...\n",
		arrsize, nthreads);
	parallelqsort(arrRandom, arrsize, nthreads);

	// Get time after execution
	gettimeofday(&tv2, NULL);
	extime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
				(double) (tv2.tv_sec - tv1.tv_sec);
	printf("... it took %f seconds\n", extime);

	printArrSample(arrRandom, arrsize);
	checkSorted(arrRandom, arrsize);

	// Free arrays
	free(arrRandom);

	// Success
	return 0;
}

static void printArrSample(int arr[], int arrsize) {

	printf("\nPrinting sample ...\n");
	int i;
	for (i = 0; i < arrsize; i += arrsize/10) {
		printf("%d ", arr[i]);
	}
	printf("%d\n", arr[arrsize - 1]);
}


static void checkSorted(int arr[], int arrsize) {

	int sorted = 1;
	printf("\nChecking if sorted ...\n");
	int i;
	for (i = 0; i < arrsize - 1; i++) {
		if (arr[i] > arr[i + 1]) {
			sorted = 0;
			printf("... arr[%d] = %d > arr[%d] = %d\n",
				i, arr[i], i + 1, arr[i + 1]);
			break;
		}
	}

	if (sorted) {
		printf("... success: is sorted\n");
	} else {
		printf("... NOT SORTED!\n");
	}
}
