#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct course {
	char *crs_num; // course number
	int credits; // course credits
	int capacity; // class capacity
	int  n_class; // number of classes per week
	char *time_str; // raw class times
	int *time; // decoded class times
	char *name; // course name
	float adv;
	float ratio;
} COURSE;

typedef struct node {
	int data;
	struct node *next;
} NODE;

typedef struct day {
	int idx;
	int option;
} DAY;

int N; // number of courses
int c_sum = 0; // sum of credits
int n_selected = 0; // number of courses selected
int *selected;
COURSE *courses;

void readData(void);
void printData(void);
void BestInGreedy(void);
void BestInGreedy2(void);
// void solve3(void);
// void updateAdv(NODE *occupy[], int *selected);
void InsertionSort(COURSE *A, int n, int dec);
void InsertionSort2(DAY A[], int n, int dec);
void freeAll(void);

int main(void)
{
	int i;

	readData();
	// printData();
	BestInGreedy();

	return 0;
}

void InsertionSort(COURSE *A, int n, int dec)
{
	int j, i;
	COURSE tmp;

	for (j = 1; j < n; j++) {
		tmp = A[j];
		i = j - 1;
		if (dec) {
			// while ((i >= 0) && ((tmp.credits / tmp.n_class) > (A[i].credits / A[i].n_class))) {
			while ((i >= 0) && (tmp.credits > A[i].credits)) {
				A[i + 1] = A[i];
				i--;
			}
		}
		else {
			// while ((i >= 0) && ((tmp.credits / tmp.n_class) < (A[i].credits / A[i].n_class))) {
			while ((i >= 0) && (tmp.credits < A[i].credits)) {
				A[i + 1] = A[i];
				i--;
			}
		}
		A[i + 1] = tmp;
	}
}

void InsertionSort2(DAY *A, int n, int dec)
{
	int j, i;
	DAY tmp;

	for (j = 1; j < n; j++) {
		tmp = A[j];
		i = j - 1;
		if (dec) {
			while ((i >= 0) && (tmp.option > A[i].option)) {
				A[i + 1] = A[i];
				i--;
			}
		}
		else {
			while ((i >= 0) && (tmp.option < A[i].option)) {
				A[i + 1] = A[i];
				i--;
			}
		}
		A[i + 1] = tmp;
	}
}

void readData(void) // read input data
{
	int i, j, k; // indices
	int w, t; // day of week, time of day
	char tmp[100];
	char ch;

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

void printData(void)
{
	int i, j;

	for (i = 0; i < N; i++) {
		printf("%d: ", i + 1);
		printf("%s ", courses[i].crs_num);
		printf("%d %d ", courses[i].credits, courses[i].capacity);
		// for (j = 0; j < courses[i].n_class; j++) printf("%d-", courses[i].time[j]);
		printf("%s", courses[i].time_str);
		printf(" %s", courses[i].name);
		// printf(" %.3f", courses[i].ratio);
		printf("\n");
	}
}


void BestInGreedy(void)
{
	int i, j; // indices
	int feas; // loop flag
	int occupied[65]; // time occupied
	int cnt = 0;
	char weekName[] = {'M', 'T', 'W', 'R', 'F'};

	for (i = 0; i < 65; i++) occupied[i] = 0; // initialize
	selected = (int *)calloc(N, sizeof(int)); // choice of courses

	InsertionSort(courses, N, 1);
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
			c_sum += courses[i].credits; // accumulate the credits
		}
	}
	printf("Total credits: %d\n", c_sum);
	printf("Number of courses selected: %d\n", n_selected);
	for (i = 0; i < 65; i++) {
		if (selected[i] == 1) {
			printf("%d: %s %d %s %s\n", ++cnt, courses[i].crs_num, 
				courses[i].credits, courses[i].time_str, courses[i].name);
		}
	}
	printf("Weekly schedule:\n");
	printf("  1 2 3 4 n 5 6 7 8 9 a b c\n");
	for (i = 0; i < 5; i++) {
		printf("%c ", weekName[i]);
		for (j = 0; j < 13; j++) {
			if (occupied[13 * i + j] == 1) printf("V");
			else printf(".");
			if (j != 12) printf(" ");
			else printf("\n");
		}
	}
}

/*
void BestInGreedy2(void)
{
	int i, j, k;
	int t; // temporary variable to store time index
	int c_sum = 0;
	int cnt = 0;
	int *selected = calloc(N, sizeof(int));
	int done;
	int ok;
	int occupied[65];
	char weekName[] = {'M', 'T', 'W', 'R', 'F'};
	NODE *has_crs[65];
	NODE *tmp;

	for (i = 0; i < 65; i++) { // initialize
		has_crs[i] = NULL;
		occupied[i] = 0;
	}
	for (i = 0; i < N; i++) { // for all courses
		for (j = 0; j < courses[i].n_class; j++) {
			t = courses[i].time[j]; // class time
			if (has_crs[t] == NULL) { // first course of the list
				has_crs[t] = (NODE *)malloc(sizeof(NODE));
				has_crs[t]->data = i; // at time t has courses[i]
				has_crs[t]->next = NULL;
			}
			else {
				tmp = (NODE *)malloc(sizeof(NODE));
				tmp->data = i; // at time t has courses[i]
				tmp->next = has_crs[t];
				has_crs[t] = tmp;
			}
		}
	}

	for (i = 0; i < 65; i++) {
		if (occupied[i] == 0) {
			tmp = has_crs[i];
			done = 0;
			while (!done && (tmp != NULL)) {
				if (selected[tmp->data] == 0) {
					ok = 1;
					for (j = 0; j < courses[tmp->data].n_class && ok; j++) {
						if (occupied[courses[tmp->data].time[j]] == 1) ok = 0;
					}
					if (ok) {
						for (j = 0; j < courses[tmp->data].n_class; j++) {
							occupied[courses[tmp->data].time[j]] = 1;
						}
						selected[tmp->data] = 1;
						c_sum += courses[tmp->data].credits;
						n_selected++;
						printf("%d %d %s\n", i, tmp->data, courses[tmp->data].name);
						printf("%s\n", courses[tmp->data].time_str);
						done = 1;
					}
					else tmp = tmp->next;
				}
				else tmp = tmp->next;
			}
		}
	}
	printf("Total credits: %d\n", c_sum);
	printf("Number of courses selected: %d\n", n_selected);
	for (i = 0; i < 65; i++) {
		if (selected[i] == 1) {
			printf("%d: %s %d %s %s\n", ++cnt, courses[i].crs_num, 
				courses[i].credits, courses[i].time_str, courses[i].name);
		}
	}
	printf("Weekly schedule:\n");
	printf("  1 2 3 4 n 5 6 7 8 9 a b c\n");
	for (i = 0; i < 5; i++) {
		printf("%c ", weekName[i]);
		for (j = 0; j < 13; j++) {
			if (occupied[13 * i + j] == 1) printf("V");
			else printf(".");
			if (j != 12) printf(" ");
			else printf("\n");
		}
	}
}
*/

/*
void solve3(void)
{
	int i, j, k;
	int t;
	int c_sum = 0;
	int cnt = 0;
	NODE *occupy[65];
	NODE *tmp;
	int *selected = calloc(n, sizeof(int));
	char weekName[] = {'M', 'T', 'W', 'R', 'F'};
	DAY order[65];
	int done;
	int ok;
	int opp_cost_d;
	int opp_cost;
	float MAX_adv;
	int MAX_id;

	for (i = 0; i < 65; i++) occupy[i] = NULL;
	for (i = 0; i < n; i++) {
		for (j = 0; j < courses[i].n_class; j++) {
			t = courses[i].time[j];
			if (occupy[t] == NULL) {
				occupy[t] = (NODE *)malloc(sizeof(NODE));
				occupy[t]->data = i;
				occupy[t]->next = NULL;
			}
			else {
				tmp = (NODE *)malloc(sizeof(NODE));
				tmp->data = i;
				tmp->next = occupy[t];
				occupy[t] = tmp;
			}
		}
	}

	updateAdv(occupy, selected);
	// selected[50] = 1; selected[51] = 1;
	// updateAdv(occupy, selected);
	// InsertionSort2(order, 65, 0);
	cnt = 0;
	for (k = 0; k < 65; k++) {
		// i = order[k].idx;
		// i = 65 - k % 65;
		i = k % 65;
		if (k == 65) printf("***************************************\n");
		if (grid[i] == 0) {
			tmp = occupy[i]; // head of list
			done = 0; // early exit if found
			MAX_adv = -100;
			MAX_id = -1;
			while (!done && (tmp != NULL)) {
				if (selected[tmp->data] == 0) { // if course not selected
					ok = 1; // if this course does not overlap with other courses
					for (j = 0; j < courses[tmp->data].n_class && ok; j++) {
						if (grid[courses[tmp->data].time[j]] == 1) ok = 0;
					}
					if (ok) { // this is a feasible course
						if (courses[tmp->data].adv > MAX_adv) {
							MAX_adv = courses[tmp->data].adv;
							MAX_id = tmp->data;
						}
					}
					tmp = tmp->next;
				}
				else tmp = tmp->next;
			}
			if (MAX_id != -1) {
				// if (MAX_adv < 0) printf("skipped\n");
				// else{
					// printf("%f\n", MAX_adv);
					for (j = 0; j < courses[MAX_id].n_class; j++) {
							grid[courses[MAX_id].time[j]] = 1; // update the schedule
					}
					selected[MAX_id] = 1; // mark as selected
					c_sum += courses[MAX_id].credits;
					n_selected++;
					printf("%d %d %s\n", i, MAX_id, courses[MAX_id].name);
					printf("%s\n", courses[MAX_id].time_str);
					// done = 1;
					printf("  1 2 3 4 n 5 6 7 8 9 a b c\n");
					for (i = 0; i < 5; i++) {
						printf("%c ", weekName[i]);
						for (j = 0; j < 13; j++) {
							if (grid[13 * i + j] == 1) printf("V");
							else printf(".");
							if (j != 12) printf(" ");
							else printf("\n");
						}
					} printf("\n");
					updateAdv(occupy, selected);
				// }
			}
		}
	}
	printf("Total credits: %d\n", c_sum);
	printf("Number of courses selected: %d\n", n_selected);
	for (i = 0; i < 65; i++) {
		if (selected[i] == 1) {
			printf("%d: %s %d %s %s\n", ++cnt, courses[i].number, 
				courses[i].credits, courses[i].time_str, courses[i].name);
		}
	}
	printf("Weekly schedule:\n");
	printf("  1 2 3 4 n 5 6 7 8 9 a b c\n");
	for (i = 0; i < 5; i++) {
		printf("%c ", weekName[i]);
		for (j = 0; j < 13; j++) {
			if (grid[13 * i + j] == 1) printf("V");
			else printf(".");
			if (j != 12) printf(" ");
			else printf("\n");
		}
	}
}

void updateAdv(NODE *occupy[], int *selected)
{
	int i, j;
	float opp_cost;
	float opp_cost_d;
	int t;
	float rec;
	float ttmp;
	NODE *tmp;
	int cnt;

	for (i = 0; i < n; i++) {
		opp_cost = 0;
		cnt = 0;
		for (j = 0; j < courses[i].n_class; j++) {
			opp_cost_d = 0;
			t = courses[i].time[j];
			// printf("  %d: ", t);
			tmp = occupy[t]; // head of list
			while (tmp != NULL) {
				// printf("    %s: %d %s\n", courses[tmp->data].name, courses[tmp->data].credits, courses[tmp->data].time_str);
				// if (selected[tmp->data] == 0 && (courses[tmp->data].credits > opp_cost_d)) opp_cost_d = courses[tmp->data].credits;
				// if (selected[tmp->data] == 0 && (courses[tmp->data].ratio > opp_cost_d)) opp_cost_d = courses[tmp->data].ratio;
				if (selected[tmp->data] == 0) {
					opp_cost_d += courses[tmp->data].ratio;
					cnt++;
				}
				tmp = tmp->next;
			}
			// printf("%d\n", opp_cost_d);
			// if (opp_cost_d > opp_cost) opp_cost = opp_cost_d;
			opp_cost += opp_cost_d;
		}
		// rec = courses[i].credits - opp_cost;
		rec = courses[i].credits - opp_cost / cnt;
		if (rec != courses[i].adv) {
			// printf("%s: %d %s\n", courses[i].name, courses[i].credits, courses[i].time_str);
			// printf("  Advantage: %.3f -> %.3f\n", courses[i].adv, rec);
			courses[i].adv = rec;
		}
	}
}
*/