// EE3980 HW02 Random Data Searches
// 105061110, 周柏宇
// submitted on 2020/03/22
// revised on 2020/03/25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;				// input size
int R_AVG = 500;	// number of repetitions for testing average case
int R_WORSE = 5000;	// number of repetitions for testing worse-case
int WORSE_CASE = 0;	// flag for testing worse-case
char **data;		// input data
int (*search[3])(char *word, char **list, int n);	// store function pointers
char algNames[][17] = {								// name of algorithms
	"Linear", "Bidirection", "Random-direction"
};

void readInput(void);							// read all inputs
double GetTime(void);							// get local time in seconds
int Search(char *word, char **list, int n);		// Linear Search
int BDSearch(char *word, char **list, int n);	// Bidirection Search
int RDSearch(char *word, char **list, int n);	// Random-direction Search
void freeMemory(char **list, int n);			// free allocated memory

int main(void)
{
	int i, j, k;				// index
	double t;					// local time
	int worseIdx[3];			// index for worse-case word

	readInput();				// store inputs in array
	printf("n: %d\n", N);		// print input size
	search[0] = Search;			// store function pointers
	search[1] = BDSearch;
	search[2] = RDSearch;
	worseIdx[0] = N - 1;		// store worse-case index
	worseIdx[1] = N / 2;		// store worse-case index
	worseIdx[2] = 0;			// worse-case undetermined, initialize as 0
	for (i = 0; i < 3; i++) {
		t = GetTime();			// get local time
		for (j = 0; j < R_AVG; j++) {
			for (k = 0; k < N; k++) {			// list all words
				search[i](data[k], data, N);	// execute search algorithm
			}
		}
		t = (GetTime() - t) / (R_AVG * N);		// calculate average CPU time
		printf("%s search average CPU time: %.5e\n", algNames[i], t);
										// print algorithm name and CPU time
	}
	WORSE_CASE = 1;						// testing for worse-case
	for (i = 0; i < 3; i++) {
		t = GetTime();					// get local time
		for (j = 0; j < R_WORSE; j++) {
			search[i](data[worseIdx[i]], data, N); // execute search algorithm
		}
		t = (GetTime() - t) / R_WORSE;	// calculate average CPU time
		printf("%s search worse-case CPU time: %.5e\n", algNames[i], t);
										// print algorithm name and CPU time
	}
	freeMemory(data, N);				// free array data

	return 0;
}

void readInput(void)				// read all inputs
{
	int i;							// index
	char tmpWord[1000];				// store input temporarily

	scanf("%d", &N);				// input number of entries
	data = (char **)malloc(sizeof(char *) * N);	// allocate memory for pointers
	for (i = 0; i < N; i++) {
		scanf("%s", tmpWord);		// input a word
		// allocate memory just enough to fit the word
		data[i] = (char *)malloc(sizeof(char) * (strlen(tmpWord) + 1));
		strcpy(data[i], tmpWord);	// transfer the input to array
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}

int Search(char *word, char **list, int n)	// Linear Search
{
	int i;						// index

	for (i = 0; i < n; i++) {	// compare all possible entries
		if (strcmp(list[i], word) == 0) {
			return i;
		}
	}

	return -1;					// unsuccessful search
}

int BDSearch(char *word, char **list, int n)	// Bidirection Search
{
	int i;										// index

	for (i = 0; i < n / 2; i++) {
		if (strcmp(list[i], word) == 0) {		// compare words from head
			return i;
		}
		if (strcmp(list[n - i - 1], word) == 0) { // compare words from tail
			return n - i - 1;
		}
	}

	return -1;									// unsuccessful search
}

int RDSearch(char *word, char **list, int n)	// Random-direction Search
{
	int j, i;				// index

	j = rand() % 2;			// choose direction randomly
	// if we are testing worse-case but the target is not the worse-case word
	if (WORSE_CASE && j) {
		word = list[n - 1];	// change the target to the worse-case word
	}
	if (j == 1) {			// forward linear search
		for (i = 0; i < n; i++) {
			if (strcmp(list[i], word) == 0) {
				return i;
			}
		}
	}
	else {					// backward linear search
		for (i = n - 1; i >= 0; i--) {
			if (strcmp(list[i], word) == 0) {
				return i;
			}
		}
	}

	return -1;				// unsuccessful search
}

void freeMemory(char **list, int n)	// free allocated memory
{
	int i;							// index

	for (i = 0; i < n; i++) {
		free(list[i]);				// free the memory that stores the words
	}
	free(list);						// free the memory that stores the pointers
}
