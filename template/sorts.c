#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 10

void print(int *A, int n);
void SelectionSort(int *A, int n, int dec);
void InsertionSort(int *A, int n, int dec);
void BubbleSort(int *A, int n, int dec);
void ShakerSort(int *A, int n, int dec);
void HeapSort(int *A, int n);
void MergeSort(int *A, int n);
void merge();
void QuickSort(int *A, int n);

int main(void)
{
	int i;
	int A[N];

	srand(0);
	for (i = 0; i < N; i++) {
		A[i] = (rand() % N) + 1;
	}
	ShakerSort(A, N, 0);
	print(A, N);
	ShakerSort(A, N, 1);
	print(A, N);

	return 0;
}

void print(int *A, int n)
{
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
