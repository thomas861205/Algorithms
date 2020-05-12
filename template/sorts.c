#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 50

void print(int *A, int n);
void SelectionSort(int *A, int n, int dec);
void InsertionSort(int *A, int n, int dec);
void BubbleSort(int *A, int n, int dec);
void ShakerSort(int *A, int n, int dec);
void HeapSort(int *A, int n, int dec);
void heapify(int *A, int i, int n, int dec);
void MergeSort(int *A, int n, int dec);
void mergesort(int *A, int *B, int low, int high, int dec);
void merge(int *A, int *B, int low, int mid, int high, int dec);
void QuickSort(int *A, int n, int dec);
void quicksort(int *A, int low, int high, int dec);
int partition(int *A, int low, int high, int dec);

int main(void)
{
	int i;
	int A[N];

	srand(time(NULL));
	for (i = 0; i < N; i++) {
		A[i] = (rand() % N) + 1;
	}
	QuickSort(A, N, 0);
	print(A, N);
	QuickSort(A, N, 1);
	print(A, N);

	return 0;
}

void print(int *A, int n) {

	int i;

	for (i = 0; i < n; i++) printf("%d ", A[i]);
	printf("\n");
}

void SelectionSort(int *A, int n, int dec)
{
	int i, j, k, tmp;

	for (i = 0; i < n; i++) {
		j = i;
		for (k = i + 1; k < n; k++) {
			if (!dec && (A[k] < A[j])) { // inc
				j = k;
			}
			else if (dec && (A[k] > A[j])) {
				j = k;
			}
		}
		tmp = A[i];
		A[i] = A[j];
		A[j] = tmp;
	}
}


void InsertionSort(int *A, int n, int dec)
{
	int j, i;
	int tmp;

	for (j = 1; j < n; j++) {
		tmp = A[j];
		i = j - 1;
		if (!dec) {
			while ((i >= 0) && (tmp > A[i])) {
				A[i + 1] = A[i];
				i--;
			}
		}
		else {
			while ((i >= 0) && (tmp < A[i])) {
				A[i + 1] = A[i];
				i--;
			}
		}
		A[i + 1] = tmp;
	}
}


void BubbleSort(int *A, int n, int dec)
{
	int i, j, tmp;

	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--) {
			if (!dec && (A[j] < A[j - 1])) {
				tmp = A[j];
				A[j] = A[j - 1];
				A[j - 1] = tmp;
			}
			else if (dec && (A[j] > A[j - 1])) {
				tmp = A[j];
				A[j] = A[j - 1];
				A[j - 1] = tmp;
			}
		}
	}
}


void ShakerSort(int *A, int n, int dec)
{
	int j;
	int l = 0;
	int r = n - 1;
	int tmp;

	while (l <= r) {
		for (j = r; j > l; j--) {
			if (!dec && (A[j] < A[j - 1])) {
				tmp = A[j];
				A[j] = A[j - 1];
				A[j - 1] = tmp;
			}
			else if (dec && (A[j] > A[j - 1])) {
				tmp = A[j];
				A[j] = A[j - 1];
				A[j - 1] = tmp;
			}
		}
		l++;
		for (j = l; j < r; j++) {
			if (!dec && (A[j] > A[j + 1])) {
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
			}
			else if (dec && (A[j] < A[j + 1])) {
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
			}
		}
		r--;
	}
}


void HeapSort(int *A, int n, int dec)
{
	int i, tmp;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(A, i, n, dec);
	}
	for (i = n - 1; i > 0; i--) {
		tmp = A[0]; A[0] = A[i]; A[i] = tmp;
		heapify(A, 0, i, dec);
	}
}

void heapify(int *A, int i, int n, int dec)
{
	int j, done, tmp;

	j = 2 * (i + 1) - 1;
	tmp = A[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if (!dec) {
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


void MergeSort(int *A, int n, int dec) {
	int *B;
	
	B = (int *)malloc(sizeof(int) * n);
	mergesort(A, B, 0, n - 1, dec);
	free(B);
}


void mergesort(int *A, int *B, int low, int high, int dec)
{
	int mid;

	if (low < high) {
		mid = (low + high) / 2;
		mergesort(A, B, low,      mid, dec);
		mergesort(A, B, mid + 1, high, dec);
		merge(A, B, low, mid, high, dec);
	}
}


void merge(int *A, int *B, int low, int mid,  int high, int dec)
{
	int k;
	int h = low; // index for lower half
	int i = low; // index for B
	int j = mid + 1; // index for upper half

	while ((h <= mid) && (j <= high)) {
		if (!dec) {
			if (A[h] >= A[j]) {
				B[i] = A[h];
				h++;
			}
			else {
				B[i] = A[j];
				j++;
			}
			i++;
		}
		else {
			if (A[h] <= A[j]) {
				B[i] = A[h];
				h++;
			}
			else {
				B[i] = A[j];
				j++;
			}
			i++;
		}
	}
	if (h > mid) {
		for (k = j; k <= high; k++) {
			B[i] = A[k];
			i++;
		}
	}
	else {
		for (k = h; k <= mid; k++) {
			B[i] = A[k];
			i++;
		}
	}
	for (k = low; k <= high; k++) {
		A[k] = B[k];
	}
}


void QuickSort(int *A, int n, int dec)
{
	quicksort(A, 0, n - 1, dec);
}


void quicksort(int *A, int low, int high, int dec)
{
	int mid;

	if (low < high) {
		mid = partition(A, low, high + 1, dec);
		quicksort(A, low,     mid - 1, dec);
		quicksort(A, mid + 1, high,    dec);
	}
}


int partition(int *A, int low, int high, int dec)
{
	int v, i, j, tmp;

	v = A[low]; i = low; j = high;
	do {
		if (!dec) {
			do {i++;} while ((A[i] > v) && (i < high));
			do {j--;} while ((A[j] < v) && (j > low ));
		}
		else {
			do {i++;} while ((A[i] < v) && (i < high));
			do {j--;} while ((A[j] > v) && (j > low ));
		}
		if (i < j) {
			tmp  = A[i];
			A[i] = A[j];
			A[j] = tmp;
		}
	} while (i < j);
	A[low] = A[j]; A[j] = v;

	return j;
}
