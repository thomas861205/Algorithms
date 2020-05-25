#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define N 10

void print(int *A, int n);
void HeapSort(int *A, int n, int dec);
void heapify(int *A, int i, int n, int min);
void array2heap(int *A, int n, int min);
int minHeapRemoveMin(int *A, int n); // remove minimum from the min heap
void minHeapInsertion(int *A, int n, int val);
int maxHeapRemoveMax(int *A, int n); // remove maximum from the max heap
void maxHeapInsertion(int *A, int n, int val);

int main(void) {
	int i;
	int A[N];
	int n = N;

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		A[i] = (rand() % N) + 1;
	}
	array2heap(A, n, 0);
	print(A, n);

	printf("%d\n", maxHeapRemoveMax(A, n--));
	print(A, n);

	maxHeapInsertion(A, ++n, 11);
	print(A, n);
	return 0;
}

void print(int *A, int n) {

	int i;

	for (i = 0; i < n; i++) printf("%d ", A[i]);
	printf("\n");
}

void HeapSort(int *A, int n, int dec)
{	
	// decreasing-order: use min heap
	// increasing-order: use max heap
	int i, tmp;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(A, i, n, dec);
	}
	for (i = n - 1; i > 0; i--) {
		tmp = A[0]; A[0] = A[i]; A[i] = tmp;
		heapify(A, 0, i, dec);
	}
}

void heapify(int *A, int i, int n, int min)
{
	// dec = 1: min heap
	// dec = 0: max heap
	int j, done, tmp;

	j = 2 * (i + 1) - 1;
	tmp = A[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if (!min) {
			if ((j < n - 1) && (A[j] < A[j + 1])) j++;
			if (tmp > A[j]) done = 1;
			else {
				A[(j + 1) / 2 - 1] = A[j];
				j = 2 * (j + 1) - 1;
			}
		}
		else {
			if ((j < n - 1) && (A[j] > A[j + 1])) j++;
			if (tmp < A[j]) done = 1;
			else {
				A[(j + 1) / 2 - 1] = A[j];
				j = 2 * (j + 1) - 1;
			}
		}
	}
	A[(j + 1) / 2 - 1] = tmp;
}


void array2heap(int *A, int n, int min) {
	int i;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(A, i, n, min);
	}
}

int minHeapRemoveMin(int *A, int n) // remove minimum from the min heap
{
	int tmp;

	assert(n > 0); // empty heap
	tmp = A[0]; // minimum is the root
	A[0] = A[n - 1]; // move last node to root
	heapify(A, 0, n - 1, 1); // restore min heap property

	return tmp;
}

// insert a node to min heap
void minHeapInsertion(int *A, int n, int val)
{
	int i; // index

	i = n - 1; // start at last node
	A[n - 1] = val; // put new node at last node
	while ((i > 0) && (A[(i + 1) / 2 - 1] > val)) {
		A[i] = A[(i + 1) / 2 - 1]; // overwrite A[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	A[i] = val; // put new node at proper place
}

int maxHeapRemoveMax(int *A, int n) // remove maximum from the max heap
{
	int tmp;

	assert(n > 0); // empty heap
	tmp = A[0]; // maximum is the root
	A[0] = A[n - 1]; // move last node to root
	heapify(A, 0, n - 1, 0); // restore max heap property

	return tmp;
}

// insert a node to max heap
void maxHeapInsertion(int *A, int n, int val)
{
	int i; // index

	i = n - 1; // start at last node
	A[n - 1] = val; // put new node at last node
	while ((i > 0) && (A[(i + 1) / 2 - 1] < val)) {
		A[i] = A[(i + 1) / 2 - 1]; // overwrite A[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	A[i] = val; // put new node at proper place
}