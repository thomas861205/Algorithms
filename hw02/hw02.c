// EE3980 HW02 Random Data Searches
// 105061110, 周柏宇
// 2020/03/12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;
char **data;
int R_AVG = 500;
int R_WORST = 5000;

void readInput(void);
int Search(char *word, char **list, int n);
int BDSearch(char *word, char **list, int n);
int RDSearch(char *word, char **list, int n);

int main(void)
{
	readInput();
	printf("%d\n", Search(data[0], data, N));
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

int Search(char *word, char **list, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		if (strcmp(list[i], word) == 0) return i;
	}
	return -1;
}


int BDSearch(char *word, char **list, int n)
{
	int i;

	for (i = 0; i < n / 2; i++) {
		if (strcmp(list[i], word) == 0) return i;
		if (strcmp(list[n - i - 1], word) == 0) return n - i - 1;
	}
	return -1;
}

int RDSearch(char *word, char **list, int n)
{
	return -1;
}