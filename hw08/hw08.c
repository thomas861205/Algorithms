#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// data structure to store course information
typedef struct course {
	char *crs_num; // course number
	int credits; // course credits
	int capacity; // class capacity
	int  n_class; // number of classes per week
	char *time_str; // raw class times
	int *time; // decoded class times
	char *name; // course name
} COURSE;

int N; // number of courses
int sum_c; // sum of credits
int n_selected; // number of courses selected
int *selected; // courses selected
int occupied[65]; // time occupied
char dayName[5] = { // abbreviation of the day of the week
	'M', 'T', 'W', 'R', 'F'};
COURSE *courses; // course information

void readData(void); // read input data
void BestInGreedy(void); // best-in greedy algorithm
void Sort(void); // sort courses wrt their credits in non-ascending order
void freeAll(void); // free allocated memory

int main(void)
{
	int i, j; // indices
	int cnt = 0; // label

	readData(); // read input data
	BestInGreedy(); // selecting courses using best-in greedy algorithm

	printf("Total credits: %d\n", sum_c);
	printf("Number of courses selected: %d\n", n_selected);
	for (i = 0; i < N; i++) { // print information of selected courses
		if (selected[i]) {
			printf("  %d: %s %d %s %s\n", ++cnt, courses[i].crs_num,
				courses[i].credits, courses[i].time_str, courses[i].name);
		}
	}
	printf("Weekly schedule:\n");
	printf("    1 2 3 4 n 5 6 7 8 9 a b c\n");
	for (i = 0; i < 5; i++) { // visualize the schedule
		printf("  %c ", dayName[i]);
		for (j = 0; j < 13; j++) {
			if (occupied[13 * i + j] == 1) printf("V");
			else printf(".");
			if (j != 12) printf(" ");
			else printf("\n");
		}
	}

	freeAll(); // free allocated memory

	return 0;
}

void Sort(void) // sort courses wrt their credits in non-ascending order
{
	// implement insertion sort
	int j, i; // indices
	COURSE tmp; // temporary variable

	for (j = 1; j < N; j++) {
		tmp = courses[j];
		i = j - 1;
		// repeat until courses[i].credits is bigger
		while ((i >= 0) && (tmp.credits > courses[i].credits)) {
			courses[i + 1] = courses[i];
			i--;
		}
		courses[i + 1] = tmp; // move courses[j] to correct place
	}
}

void readData(void) // read input data
{
	int i, j, k; // indices
	int w, t; // day of week, time of day
	char tmp[100]; // temporary variable
	char ch; // temporary variable

	scanf("%d\n", &N); // input number of courses
	// allocate memory for course infomation
	courses = (COURSE *)malloc(sizeof(COURSE) * N);
	for (i = 0; i < N; i++) {
		scanf("%s", tmp); // input course number
		// allocate memory for course number
		courses[i].crs_num = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].crs_num, tmp);

		// input course credits and class capacity
		scanf(" %d %d ", &courses[i].credits, &courses[i].capacity);

		scanf("%s ", tmp); // input class times
		// allocate memory for raw class times
		courses[i].time_str = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].time_str, tmp);
		courses[i].n_class = strlen(tmp) / 2; // get number of classes per week
		// allocate memory for decoded class time
		courses[i].time = (int *)malloc(sizeof(int) * courses[i].n_class);
		for (j = 0; j < strlen(tmp); j += 2) { // decode the class time string
			switch(tmp[j]) { // decide day of the week
				case 'M': w = 0; break;
				case 'T': w = 1; break;
				case 'W': w = 2; break;
				case 'R': w = 3; break;
				case 'F': w = 4; break;
			}
			switch(tmp[j + 1]) { // decide time of the day
				case '1': t = 0; break;
				case '2': t = 1; break;
				case '3': t = 2; break;
				case '4': t = 3; break;
				case 'n': t = 4; break;
				case '5': t = 5; break;
				case '6': t = 6; break;
				case '7': t = 7; break;
				case '8': t = 8; break;
				case '9': t = 9; break;
				case 'a': t = 10; break;
				case 'b': t = 11; break;
				case 'c': t = 12; break;
			}
			courses[i].time[j / 2] = 13 * w + t; // express linearly
		}

		k = 0;
		// store the rest of characters of the line
		while ((ch=getchar()) != '\n') tmp[k++] = ch;
		tmp[k] = '\0';
		// allocate memory for course name
		courses[i].name = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].name, tmp);
	}
}

void BestInGreedy(void)
{
	int i, j; // indices
	int feas; // loop flag

	// initialization
	sum_c = 0;
	n_selected = 0;
	selected = (int *)calloc(N, sizeof(int));
	for (i = 0; i < 65; i++) occupied[i] = 0;

	Sort(courses);
	for (i = 0; i < N; i++) { // try all courses
		feas = 1;
		for (j = 0; j < courses[i].n_class && feas; j++) {
			// check if courses[i] overlaps with other selected courses
			if (occupied[courses[i].time[j]] == 1) feas = 0;
		}
		if (feas) { // courses[i] is feasible
			selected[i] = 1;
			n_selected++;
			// update the occupied time
			for (j = 0; j < courses[i].n_class; j++) {
				occupied[courses[i].time[j]] = 1;
			}
			sum_c += courses[i].credits; // accumulate the credits
		}
	}
}

void freeAll(void) // free allocated memory
{
	int i;
	COURSE *tmp;

	for (i = 0; i < N; i++) {
		free(courses[i].crs_num);
		free(courses[i].time_str);
		free(courses[i].time);
		free(courses[i].name);
	}
	free(courses);
	free(selected);
}
