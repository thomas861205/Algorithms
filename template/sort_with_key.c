#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 10

void print(int *idx, int *A, int n);
void SelectionSort(int *idx, int *A, int n, int dec);
void InsertionSort(int *idx, int *A, int n, int dec);
void HeapSort(int *idx, int *A, int n, int dec);
void heapify(int *idx, int *A, int i, int n, int dec);
void array2heap(int *idx, int *A, int n, int min);
void QuickSort(int *idx, int *A, int n, int dec);
void quicksort(int *idx, int *A, int low, int high, int dec);
int partition(int *idx, int *A, int low, int high, int dec);

int main(void)
{
	int i;
	int idx[N];
	int A[N];

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		idx[i] = i;
		A[i] = (rand() % N) + 1;
	}
	QuickSort(idx, A, N, 0);
	print(idx, A, N);
	QuickSort(idx, A, N, 1);
	print(idx, A, N);

	return 0;
}

void print(int *idx, int *A, int n) {

	int i;

	for (i = 0; i < n; i++) printf("%d %d\n", idx[i], A[idx[i]]);
}

void SelectionSort(int *idx, int *A, int n, int dec)
{
	int i, j, k, tmp;

	for (i = 0; i < n; i++) {
		j = i;
		for (k = i + 1; k < n; k++) {
			if (!dec && (A[idx[k]] < A[idx[j]])) { // inc
				j = k;
			}
			else if (dec && (A[idx[k]] > A[idx[j]])) {
				j = k;
			}
		}
		tmp = idx[i];
		idx[i] = idx[j];
		idx[j] = tmp;
	}
}


void InsertionSort(int *idx, int *A, int n, int dec)
{
	int j, i;
	int tmp;

	for (j = 1; j < n; j++) {
		tmp = idx[j];
		i = j - 1;
		if (dec) {
			while ((i >= 0) && (A[tmp] > A[idx[i]])) {
				idx[i + 1] = idx[i];
				i--;
			}
		}
		else {
			while ((i >= 0) && (A[tmp] < A[idx[i]])) {
				idx[i + 1] = idx[i];
				i--;
			}
		}
		idx[i + 1] = tmp;
	}
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

void heapify(int *idx, int *A, int i, int n, int dec)
{
	// dec = 1: min heap
	// dec = 0: max heap
	int j, done, tmp;

	j = 2 * (i + 1) - 1;
	tmp = idx[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if (!dec) {
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

void QuickSort(int *idx, int *A, int n, int dec)
{
	quicksort(idx, A, 0, n - 1, dec);
}


void quicksort(int *idx, int *A, int low, int high, int dec)
{
	int mid;

	if (low < high) {
		mid = partition(idx, A, low, high + 1, dec);
		quicksort(idx, A, low,     mid - 1, dec);
		quicksort(idx, A, mid + 1, high,    dec);
	}
}


int partition(int *idx, int *A, int low, int high, int dec)
{
	int v, i, j, tmp;

	v = idx[low]; i = low; j = high;
	do {
		if (dec) {
			do {i++;} while ((A[idx[i]] > A[v]) && (i < high));
			do {j--;} while ((A[idx[j]] < A[v]) && (j > low ));
		}
		else {
			do {i++;} while ((A[idx[i]] < A[v]) && (i < high));
			do {j--;} while ((A[idx[j]] > A[v]) && (j > low ));
		}
		if (i < j) {
			tmp = idx[i];
			idx[i] = idx[j];
			idx[j] = tmp;
		}
	} while (i < j);
	idx[low] = idx[j]; idx[j] = v;

	return j;
}
