#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define N 10

void print(int *idx, int *A, int n);
void HeapSort(int *idx, int *A, int n, int dec);
void heapify(int *idx, int *A, int i, int n, int min);
void array2heap(int *idx, int *A, int n, int min);
int minHeapRemoveMin(int *idx, int *A, int n); // remove minimum from the min heap
void minHeapInsertion(int *idx, int *A, int n, int val);
int maxHeapRemoveMax(int *idx, int *A, int n); // remove maximum from the max heap
void maxHeapInsertion(int *idx, int *A, int n, int val);

int main(void) {
	int i;
	int idx[N];
	int A[N];
	int n = N;

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		idx[i] = i;
		A[i] = (rand() % N) + 1;
	}
	array2heap(idx, A, n, 1);
	print(idx, A, n);

	printf("%d\n", minHeapRemoveMin(idx, A, n--));
	print(idx, A, n);

	minHeapInsertion(idx, A, ++n, 11);
	print(idx, A, n);
	return 0;
}

void print(int *idx, int *A, int n) {

	int i;

	for (i = 0; i < n; i++) printf("%d %d\n", idx[i], A[idx[i]]);
	printf("***\n");
}


void HeapSort(int *idx, int *A, int n, int dec)
{
	// decreasing-order: use min heap
	// increasing-order: use max heap
	int i, tmp;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(idx, A, i, n, dec);
	}
	for (i = n - 1; i > 0; i--) {
		tmp = idx[0]; idx[0] = idx[i]; idx[i] = tmp;
		heapify(idx, A, 0, i, dec);
	}
}

void heapify(int *idx, int *A, int i, int n, int min)
{
	// min = 1: min heap
	// min = 0: max heap
	int j, done, tmp;

	j = 2 * (i + 1) - 1;
	tmp = idx[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if (!min) {
			if ((j < n - 1) && (A[idx[j]] < A[idx[j + 1]])) j++;
			if (A[tmp] > A[idx[j]]) done = 1;
			else {
				idx[(j + 1) / 2 - 1] = idx[j];
				j = 2 * (j + 1) - 1;
			}
		}
		else {
			if ((j < n - 1) && (A[idx[j]] > A[idx[j + 1]])) j++;
			if (A[tmp] < A[idx[j]]) done = 1;
			else {
				idx[(j + 1) / 2 - 1] = idx[j];
				j = 2 * (j + 1) - 1;
			}
		}
	}
	idx[(j + 1) / 2 - 1] = tmp;
}

void array2heap(int *idx, int *A, int n, int min) {
	int i;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(idx, A, i, n, min);
	}
}

int minHeapRemoveMin(int *idx, int *A, int n) // remove minimum from the min heap
{
	int ret, tmp;

	assert(n > 0); // empty heap
	ret = A[idx[0]]; // minimum is the root
	tmp = idx[0]; idx[0] = idx[n - 1]; idx[n - 1] = tmp;// move last node to root
	heapify(idx, A, 0, n - 1, 1); // restore min heap property

	return ret;
}

// insert a node to min heap
void minHeapInsertion(int *idx, int *A, int n, int val)
{
	int i; // index
	int tmp = idx[n - 1];

	i = n - 1; // start at last node
	A[idx[n - 1]] = val; // put new node at last node
	while ((i > 0) && (A[idx[(i + 1) / 2 - 1]] > val)) {
		idx[i] = idx[(i + 1) / 2 - 1]; // overwrite A[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	idx[i] = tmp; // put new node at proper place
}

int maxHeapRemoveMax(int *idx, int *A, int n) // remove maximum from the max heap
{
	int ret, tmp;

	assert(n > 0); // empty heap
	ret = A[idx[0]]; // maximum is the root
	tmp = idx[0]; idx[0] = idx[n - 1]; idx[n - 1] = tmp; // move last node to root
	heapify(idx, A, 0, n - 1, 0); // restore max heap property

	return ret;
}

// insert a node to max heap
void maxHeapInsertion(int *idx, int *A, int n, int val)
{
	int i; // index
	int tmp = idx[n - 1];

	i = n - 1; // start at last node
	A[idx[n - 1]] = val; // put new node at last node
	while ((i > 0) && (A[idx[(i + 1) / 2 - 1]] < val)) {
		idx[i] = idx[(i + 1) / 2 - 1]; // overwrite A[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	idx[i] = tmp; // put new node at proper place
}