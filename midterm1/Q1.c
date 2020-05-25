// Q1 EE3980 Algorithms midterm exam
// u105061110, 周柏宇
// 2020/04/28

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;                      // number of words
char **A;                   // array of words
int R = 1;                  // Repetitions

double GetTime(void);       // current time in sec
void Q1(char **A, int N);   // to set the first 2 and last 2 words
void heapS(char **A, int N, int nth);
void Heapify(char **A, int i, int n);
void minHeapify(char **A, int i, int n);

int main(void)
{
	int i;
	char tmp[100];

	scanf("%d", &N);
	A = (char **)malloc(sizeof(char *) * N);
	for (i = 0; i < N; i++) {
		scanf("%s", tmp);
		A[i] = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(A[i], tmp);
	}

	 //heapS(A, N, -1);

	//for (i = 0; i < N; i++) printf("%s\n", A[i]);

	Q1(A, N);
}

void Q1(char **A, int N)    // to set the first 2 and last 2 words
{
	heapS(A, N, -1);
	heapS(A, N, -2);
	heapS(A, N, 1);
	heapS(A, N, 2);
}

void heapS(char **A, int N, int nth)
{
	int i;
	int done = 0;
	char *tmp;
	int abs;

	for (i = N / 2 - 1; i >= 0; i--) {
		if (nth < 0) minHeapify(A, i, N);
		else Heapify(A, i, N);
	}
	for (i = N - 1; i > 0 && !done; i--) {
		tmp = A[0];
		A[0] = A[i];
		A[i] = tmp;

		abs = nth > 0 ? nth : -nth;

		if ((abs + i) == N) {
			printf("%s\n", A[i]);
			done = 1;
		}
		else{
			if (nth < 0) minHeapify(A, 0, i);
			else Heapify(A, 0, i);
		}
	}
}

void Heapify(char **A, int i, int n)
{
	int j;
	int done;
	char *tmp;

	j = 2 * (i + 1) - 1;
	tmp = A[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if ((j < n - 1) && (strcmp(A[j], A[j + 1]) < 0)) {
			j++;
		}
		if (strcmp(tmp, A[j]) > 0) {
			done = 1;
		}
		else {
			A[(j + 1) / 2 - 1] = A[j];
			j = 2 * (j + 1) - 1;
		}
	}
	A[(j + 1) / 2 - 1] = tmp;
}

void minHeapify(char **A, int i, int n)
{
	int j;
	int done;
	char *tmp;

	j = 2 * (i + 1) - 1;
	tmp = A[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if ((j < n - 1) && (strcmp(A[j], A[j + 1]) > 0)) {
			j++;
		}
		if (strcmp(tmp, A[j]) < 0) {
			done = 1;
		}
		else {
			A[(j + 1) / 2 - 1] = A[j];
			j = 2 * (j + 1) - 1;
		}
	}
	A[(j + 1) / 2 - 1] = tmp;
}

double GetTime(void)        // current time in sec
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}
