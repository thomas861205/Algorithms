// EE3980 HW01 Quadratic Sorts
// 105061110, 周柏宇
// 2020/03/12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;
char **data;	// input data
char **A;		//array to be sorted
int R = 1;

void readInput(void);
void printArray(char **A);
void copyArray(char **data, char **A);
double GetTime(void);
void SelectionSort(char **list, int n);
void InsertionSort(char **list, int n);
void BubbleSort(char **list, int n);
void ShakerSort(char **list, int n);

int main(void)
{
	int i;
	double t;
	
	readInput();
	A = (char **)malloc(sizeof(char *) * N);
	for (i = 0; i < R; i++) {
		copyArray(data, A);
		// SelectionSort(A, N);
		if (i == 0) printArray(A);
	}

	return 0;
}

void readInput(void)
{
	int i;
	char tmpWord[1000];

	scanf("%d", &N);
	data = (char **)malloc(sizeof(char *) * N);
	for (i = 0; i < N; i++) {
		scanf("%s", tmpWord);
		data[i] = (char *)malloc(sizeof(char) * (strlen(tmpWord) + 1));
		strcpy(data[i], tmpWord);
	}
}

void printArray(char **A)
{
	int i;

	for (i = 0; i < N; i++) {
		printf("%d %s\n", i + 1, A[i]);
	}
}

void copyArray(char **data, char **A)
{
	int i;

	for (i = 0; i < N; i++) {
		A[i] = data[i];
	}
}

void SelectionSort(char **list, int n)
{
	;
}

void InsertionSort(char **list, int n)
{
	;
}

void BubbleSort(char **list, int n)
{
	;
}

void ShakerSort(char **list, int n)
{
	;
}