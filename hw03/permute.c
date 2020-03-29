#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 13

int A[N];
int A_copy[N];
int min_cnt = 10000;
float avg_cnt = 0;
int max_cnt = -1;
unsigned int factorial = 0;

void Permutation(int* A, int k, int n);
int HeapSort(int *list, int n);
int Heapify(int *list, int i, int n);
void copyArray(int *A, int *B, int n);

int main(void)
{
	int i;

	printf("%d\n", N);
	for (i = 0; i < N; i++) A[i] = i;
	Permutation(A, 0, N);
	printf("max = %d, avg = %.2f, min = %d\n", max_cnt, avg_cnt, min_cnt);

	for (i = 0; i < N; i++) A_copy[i] = i;
	printf("Increasing order: %d\n", HeapSort(A_copy, N));

	for (i = 0; i < N; i++) A_copy[N - 1 - i] = i;
	printf("Decreasing order: %d\n", HeapSort(A_copy, N));
	return 0;
}

void Permutation(int *A, int k, int n)
{
	int i;
	int t;
	int cnt;

	if (k == n) {
		// for (i = 0; i < n; i++) printf("%d ", A[i]);
		// printf("-> ");
		copyArray(A, A_copy, n);
		cnt = HeapSort(A_copy, n);
		// for (i = 0; i < n; i++) printf("%d ", A_copy[i]);
		// printf("cnt: %d\n", cnt);
		if (factorial < 4294967295) factorial++;
		avg_cnt = avg_cnt + (cnt - avg_cnt) / (factorial);
		if (cnt > max_cnt) max_cnt = cnt;
		else if (cnt < min_cnt) min_cnt = cnt;
	}
	else {
		for (i = k; i < n; i++) {
			t = A[k]; A[k] = A[i]; A[i] = t;
			Permutation(A, k + 1, n);
			t = A[k]; A[k] = A[i]; A[i] = t;
		}
	}
}

int HeapSort(int *list, int n)
{
	int i;
	int t;
	int cnt = 0;

	for (i = n / 2 - 1; i >= 0; i--) {
		cnt += Heapify(list, i, n);
	}
	for (i = n - 1; i > 0; i--) {
		t = list[0]; list[0] = list[i]; list[i] = t;
		cnt += Heapify(list, 0, i);
	}

	return cnt;
}

int Heapify(int *list, int i, int n)
{
	int j;
	int done;
	int tmpWord;
	int cnt = 0;

	j = 2 * (i + 1) - 1;
	tmpWord = list[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		cnt++;
		if ((j < n - 1) && (list[j] < list[j + 1])) {
			j++;
		}
		if (tmpWord > list[j]) {
			done = 1;
		}
		else {
			list[(j + 1) / 2 - 1] = list[j];
			j = 2 * (j + 1) - 1;
		}
	}
	list[(j + 1) / 2 - 1] = tmpWord;

	return cnt;
}


void copyArray(int *A, int *B, int n)
{
	int i;

	for (i = 0; i < n; i++) B[i] = A[i];
}