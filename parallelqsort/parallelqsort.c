#include "parallelqsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*******************************************************************************
STATIC FUNCTION DECLARATIONS
*******************************************************************************/

/**
 * Compares integers a and b.
 *
 * @param  a First item to compare
 * @param  b Second item to compare
 *
 * @return   Returns 0 if a == b, < 0 if a < b, > 0 if a > b
 */
static int compare(
		const void* __restrict a,
		const void* __restrict b)__attribute__((pure));

/**
 * Sorts arrs[] by splitting and merging recursively. Assumes the arrays
 * in arrs are sorted.
 *
 * @param arrs           Arrays to be sorted
 * @param arrsizes       Sizes of arrays in arrs
 * @param threadIdxStart Thread interval start (inclusive)
 * @param threadIdxEnd   Thread interval end (exclusive)
 */
static void splitAndMerge(
		int* __restrict arrs[],
		int* __restrict arrsizes,
		const int threadIdxStart,
		const int threadIdxEnd);

/**
 * Merges arrFirst and arrSecond into arrFinal sorted. Assumes arrFirst and
 * arrSecond are sorted.
 *
 * @param arrFinal    Array to store sorted merge.
 * @param startFinal  [description]
 * @param endFinal    [description]
 * @param arrFirst    [description]
 * @param startFirst  [description]
 * @param endFirst    [description]
 * @param arrSecond   [description]
 * @param startSecond [description]
 * @param endSecond   [description]
 */
static void merge(
		int* __restrict arrFinal, const int startFinal, const int endFinal,
		int* __restrict arrFirst, const int startFirst, const int endFirst,
		int* __restrict arrSecond, const int startSecond,
			const int endSecond);

/*******************************************************************************
FUNCTION DEFINITIONS
*******************************************************************************/

void parallelqsort(
		int* __restrict arr,
		const int arrsize,
		const int nthreadsArg) {

	// Make sure nthreadsArg is a power of 2
	int powOf2 = 1;
	while (powOf2 < nthreadsArg) {
		powOf2 *= 2;
	}
	if (powOf2 > nthreadsArg) { // nthreadsArg is not a power of 2
		printf("\n");
		if (nthreadsArg < 1) { // Can't use < 1 threads
			powOf2 = 1;
			printf("WARNING: invalid nthreads = %d < 1. "
				"Setting nthreads = %d.\n\n",
				nthreadsArg, powOf2);
		} else { //
			powOf2 = powOf2/2; // Round down
			printf("WARNING: nthreads = %d is not a power of 2. "
				"Using nthreads = %d instead.\n\n", nthreadsArg, powOf2);
		}
	}
	const int nthreads = powOf2;

	if (nthreads > 1) {

		// Set number of threads to use
		omp_set_num_threads(nthreads);

		// Declare sort interval variables
		int threadWorkSize; // Work size for all but the last thread
		int leftoverWorkSize; // Work size not covered by threadWorkSize
		int start[nthreads]; // The start indices for each thread (inclusive)
		int end[nthreads]; // The end indices for each thread (exclusive)

		// Compute thread work size
		leftoverWorkSize = arrsize % nthreads;
		if (leftoverWorkSize) {
			threadWorkSize = (arrsize - leftoverWorkSize)/nthreads;
		} else {
			threadWorkSize = arrsize/nthreads;
		}

		// Work for each thread variable declaration
		int* arrs[nthreads]; // Pointers to each array
		int arrsizes[nthreads]; // Length of each array

		// Loop indices declaration
		unsigned int i;
		#pragma omp parallel
		{
			// Compute interval for each thread to sort
			#pragma omp for schedule(auto) nowait
			for (i = 0; i < nthreads - 1; i++) {
				// All but last interval
				start[i] = i * threadWorkSize;
				end[i] = (i + 1) * threadWorkSize;
			}
			// Last interval, includes work not covered by threadWorkSize
			#pragma omp single
			{
				start[nthreads - 1] = arrsize
										- threadWorkSize
										- leftoverWorkSize;
				end[nthreads - 1] = arrsize;
			}

			// Divide arr into nthreads parts
			#pragma omp for schedule(auto)
			for (i = 0; i < nthreads; i++) {

				// The arr part and their size
				arrs[i] = (int*) malloc((end[i] - start[i]) * sizeof(int));
				arrsizes[i] = end[i] - start[i];

				// Copy from arr to part
				unsigned int j;
				for (j = 0; j < arrsizes[i]; j++) {
					arrs[i][j] = arr[start[i] + j];
				}

				// Quicksort the part
				qsort(arrs[i], arrsizes[i], sizeof(int), compare);
			}
		}

		// Split and merge each thread
		splitAndMerge(arrs, arrsizes, 0, nthreads);

		// Copy sorted malloced arrays to arr
		#pragma omp parallel
		{
			// Find new start indices
			// (Faster to do this separately than inside next two #pragma omps)
			#pragma omp for schedule(auto)
			for (i = 0; i < nthreads; i++) {
				int idx = 0;
				unsigned int j;
				for (j = 0; j < i; j++) {
					idx += arrsizes[j];
				}
				start[i] = idx;
			}

			// Copy back to original array
			#pragma omp for schedule(auto) nowait
			for (i = 0; i < nthreads - 1; i++) {

				// Copy back
				unsigned int j;
				for (j = 0; j < arrsizes[i]; j++) {
					arr[start[i] + j] = arrs[i][j];
				}

				// Free thread array
				free(arrs[i]);
			}
			// Last interval
			#pragma omp single
			{

				// Copy back
				i = nthreads - 1;
				unsigned int j;
				for (j = 0; j < arrsizes[i]; j++) {
					arr[start[i] + j] = arrs[i][j];
				}

				// Free thread array
				free(arrs[i]);
			}
		}

	} else { // else: nthreads = 1, use regular qsort

		qsort(arr, arrsize, sizeof(int), compare);
	}
}

/*******************************************************************************
STATIC FUNCTION DEFINITIONS
*******************************************************************************/

static int compare(
		const void* __restrict a,
		const void* __restrict b) {

	return(*((int*) a) - *((int*) b));
}

static void splitAndMerge(
		int* __restrict arrs[],
		int* __restrict arrsizes,
		const int threadIdxStart,
		const int threadIdxEnd) {

	// Number of threads for the new group of processors
	const int nthreads = threadIdxEnd - threadIdxStart;
	const int nthreadsGroup = nthreads/2;
	const int threadIdxMid = threadIdxStart + nthreadsGroup;

	// Loop counter
	unsigned int i;

	// Select pivot; median in first thread
	const int pivot = arrs[threadIdxStart][arrsizes[threadIdxStart]/2];

	// Indices where to split
	int split[nthreads];

	// Old array pointers
	int* oldArrs[nthreads];
	int oldArrsizes[nthreads];

	#pragma omp parallel
	{
		// Find split between > pivot and < pivot
		#pragma omp for schedule(auto) nowait
		for (i = threadIdxStart; i < threadIdxEnd; i++) {
			int j;
			int iAdjusted = i - threadIdxStart; // Index for split and oldArrs
			split[iAdjusted] = arrsizes[i];
			for (j = 0; j < arrsizes[i]; j++) {
				if (arrs[i][j] > pivot) {
					split[iAdjusted] = j;
					break;
				}
			}
		}

		// Save old array pointers
		#pragma omp for schedule(auto)
		for (i = threadIdxStart; i < threadIdxEnd; i++) {
			int iAdjusted = i - threadIdxStart; // Index for split and oldArrs
			oldArrs[iAdjusted] = arrs[i];
			oldArrsizes[iAdjusted] = arrsizes[i];
		}

		// Now, group processors in 2 groups and exchange data pair-wise

		// Elements smaller than pivot
		#pragma omp for schedule(auto) nowait
		for (i = threadIdxStart; i < threadIdxMid; i++) {

			// Index for split and oldArrs
			const int iAdjusted = i - threadIdxStart;
			const int iAdjustedNthreadsGroup = iAdjusted + nthreadsGroup;

			// Create new array for the smaller than pivot elements
			arrsizes[i] = split[iAdjusted] +
							split[iAdjustedNthreadsGroup];
			arrs[i] = (int*) malloc(arrsizes[i] * sizeof(int));

			// Merge the two intervals into the new array
			merge(	arrs[i], 0, arrsizes[i],
					oldArrs[iAdjusted], 0, split[iAdjusted],
					oldArrs[iAdjustedNthreadsGroup],
						0, split[iAdjustedNthreadsGroup]
			);
		}

		// Elements larger than pivot
		#pragma omp for schedule(auto)
		for (i = threadIdxMid; i < threadIdxEnd; i++) {

			// Index for split and oldArrs
			const int iAdjusted = i - threadIdxStart;
			const int iAdjustedNthreadsGroup = iAdjusted - nthreadsGroup;

			// Create new array for the larger than pivot elements
			arrsizes[i] = oldArrsizes[iAdjusted] - split[iAdjusted] +
							oldArrsizes[iAdjustedNthreadsGroup] -
								split[iAdjustedNthreadsGroup];
			arrs[i] = (int*) malloc(arrsizes[i] * sizeof(int));

			// Merge the two intervals into the new array
			merge(	arrs[i], 0, arrsizes[i],
					oldArrs[iAdjusted], split[iAdjusted],
						oldArrsizes[iAdjusted],
					oldArrs[iAdjustedNthreadsGroup],
						split[iAdjustedNthreadsGroup],
						oldArrsizes[iAdjustedNthreadsGroup]
			);
		}

		// Free the obsolete arrs stored in oldArrs
		#pragma omp for schedule(auto)
		for (i = 0; i < nthreads; i++) {
			free(oldArrs[i]);
		}

	}

	// Recurse if there is more sorting to do
	// (Faster to do it here than inside parallel)
	if (nthreadsGroup > 1) {
		splitAndMerge(arrs, arrsizes, threadIdxStart, threadIdxMid);
		splitAndMerge(arrs, arrsizes, threadIdxMid, threadIdxEnd);
	}
}

static void merge(
		int* __restrict arrFinal, const int startFinal, const int endFinal,
		int* __restrict arrFirst, const int startFirst, const int endFirst,
		int* __restrict arrSecond, const int startSecond,
			const int endSecond) {

	// Merge arrFirst and arrSecond into arrFinal
	unsigned int iFinal = startFinal; // Index in arrFinal
	unsigned int iFirst = startFirst; // Index in arrFirst
	unsigned int iSecond = startSecond; // Index in arrSecond
	while (iFirst < endFirst && iSecond < endSecond) {

		// Find which from which array to merge
		if (arrFirst[iFirst] < arrSecond[iSecond]) { // Merge from arrFirst
			arrFinal[iFinal] = arrFirst[iFirst];
			iFirst++;
		} else { // Merge from arrSecond
			arrFinal[iFinal] = arrSecond[iSecond];
			iSecond++;
		}
		iFinal++; // Increment arrFinal position
	}

	// Remaining arrFirst
	while (iFirst < endFirst) {
		arrFinal[iFinal] = arrFirst[iFirst];
		iFirst++; // Increment iFirst position
		iFinal++; // Increment arrFinal position
	}

	// Remaining arrSecond
	while (iSecond < endSecond) {
		arrFinal[iFinal] = arrSecond[iSecond];
		iSecond++; // Increment iSecond position
		iFinal++; // Increment arrFinal position
	}
}
