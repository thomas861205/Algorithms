// EE3980 HW01 Quadratic Sorts
// 105061110, 周柏宇
// 2020/03/12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define METHOD 3

int N;
char **data;	// input data
char **A;		//array to be sorted
int R = 1;
char algNames[4][10] = {"Selection", "Insertion", "Bubble", "Shaker"};
void (*Sort[4])(char **list, int n);

void readInput(void);
void printArray(char **A);
void copyArray(char **data, char **A);
double GetTime(void);
void SelectionSort(char **list, int n);
void InsertionSort(char **list, int n);
void BubbleSort(char **list, int n);
void ShakerSort(char **list, int n);
void swap(char **a, char **b);
void freeMemory(char **list, int n);

int main(void)
{
	int i;
	double t;
	
	readInput();
	t = GetTime();
	A = (char **)malloc(sizeof(char *) * N);
	Sort[0] = SelectionSort;
	Sort[1] = InsertionSort;
	Sort[2] = BubbleSort;
	Sort[3] = ShakerSort;
	for (i = 0; i < R; i++) {
		copyArray(data, A);
		(*Sort[METHOD])(A, N);
		if (i == 0) {
			printArray(A);
		}
	}
	t = (GetTime() - t) / R;
	printf("%s Sort:\n", algNames[METHOD]);
	printf("  N = %d\n", N);
	printf("  CPU time = %.5e seconds\n", t);
	freeMemory(data, N);
	free(A);

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

double GetTime(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void SelectionSort(char **list, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++) {
		j = i;
		for (k = i + 1; k < n; k++) {
			if (strcmp(list[k], list[j]) < 0) {
				j = k;
			}
		}
		swap(&list[i], &list[j]);
	}
}

void InsertionSort(char **list, int n)
{
	int j, i;
	char *tmp;

	for (j = 1; j < n; j++) {
		tmp = list[j];
		i = j - 1;
		while ((i >= 0) && (strcmp(tmp, list[i]) < 0)) {
			list[i + 1] = list[i];
			i--;
		}
		list[i + 1] = tmp;
	}
}

void BubbleSort(char **list, int n)
{
	int i, j;

	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--) {
			if (strcmp(list[j], list[j - 1]) < 0) {
				swap(&list[j], &list[j - 1]);
			}
		}
	}
}

void ShakerSort(char **list, int n)
{
	int j;
	int l = 0;
	int r = n - 1;

	while (l <= r) {
		for (j = r; j > l; j--) {
			if (strcmp(list[j], list[j - 1]) < 0) {
				swap(&list[j], &list[j - 1]);
			}
		}
		l++;
		for (j = l; j < r; j++) {
			if (strcmp(list[j], list[j + 1]) > 0) {
				swap(&list[j], &list[j + 1]);
			}
		}
		r--;
	}
}

void swap(char **a, char **b)
{
	char *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void freeMemory(char **list, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		free(list[i]);
	}
	free(list);
}
