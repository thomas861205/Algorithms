// Q2 EE3980 Algorithms midterm exam
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
void findk2(char **A, int N, int k);    // to find k and k+1 words
void QS(char **A, int low, int high);
int ptt(char **A, int low, int high);

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
	QS(A, 0, N - 1);
	for (i = 0; i < N; i++) printf("%s\n", A[i]);
}

void findk2(char **A, int N, int k)     // to find k and k+1 words
{

}

void QS(char **A, int low, int high)
{
	int mid;

	if (low < high) {
		mid = ptt(A, low, high);
		// printf("%d\n", mid);
		QS(A, low, mid - 1);
		QS(A, mid + 1, high);
	}
}

int ptt(char **A, int low, int high)
{
	char *v = A[low];
	int i = low;
	int j = high;
	char *tmp;

	do {
		do {i++;} while ((i <= high) && strcmp(A[i], v) > 0);
		do {j--;} while ((j >= low) && strcmp(A[j], v) < 0);
		if (i < j) {
			tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;
		}
	} while (i < j);

	A[low] = A[j];
	A[j] = v;
	
	return j;
}

double GetTime(void)        // current time in sec
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}
