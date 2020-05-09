#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct course {
	char *number;
	int credits;
	int capacity;
	int  n_class;
	int *time;
	char *time_str;
	char *name;
} COURSE;

int n;
int c_sum;
int n_selected;
int *choice;
int grid[65];
COURSE *courses;

void readData(void);
void printData(void);
void solve(void);

int main(void)
{
	int i;

	readData();
	// printData();
	c_sum = 0;
	n_selected = 0;
	choice = (int *)calloc(n, sizeof(int));
	for (i = 0; i < 65; i++) grid[i] = 0;
	solve();

	return 0;
}


void readData(void)
{
	int i, j;
	int w, t;
	int cnt;
	char tmp[100];
	char ch;

	scanf("%d\n", &n);
	courses = (COURSE *)malloc(sizeof(COURSE) * n);
	for (i = 0; i < n; i++) {
		scanf("%s", tmp);
		courses[i].number = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].number, tmp);
		scanf(" %d %d ", &courses[i].credits, &courses[i].capacity);
		scanf("%s ", tmp);
		courses[i].n_class = strlen(tmp) / 2;
		courses[i].time = (int *)malloc(sizeof(int) * courses[i].n_class);
		for (j = 0; j < strlen(tmp); j += 2) {
			switch(tmp[j]) {
				case 'M': w = 0; break;
				case 'T': w = 1; break;
				case 'W': w = 2; break;
				case 'R': w = 3; break;
				case 'F': w = 4; break;
			}
			switch(tmp[j + 1]) {
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
			// printf("%d-%d ", w, t);
			courses[i].time[j / 2] = 13 * w + t;
		}
		// printf("\n");
		courses[i].time_str = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].time_str, tmp);
		cnt = 0;
		while ((ch=getchar()) != '\n') {
			tmp[cnt++] = ch;
		}
		tmp[cnt] = '\0';
		courses[i].name = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].name, tmp);
	}
}

void printData(void)
{
	int i, j;

	for (i = 0; i < n; i++) {
		printf("%d: ", i + 1);
		printf("%s ", courses[i].number);
		printf("%d %d ", courses[i].credits, courses[i].capacity);
		// for (j = 0; j < courses[i].n_class; j++) printf("%d-", courses[i].time[j]);
		printf("%s", courses[i].time_str);
		printf(" %s", courses[i].name);
		printf("\n");
	}
}

void solve(void)
{
	int i, j;
	int cnt = 0;
	int ok;
	char weekName[] = {'M', 'T', 'W', 'R', 'F'};

	for (i = 0; i < n; i++) {
		ok = 1;
		for (j = 0; j < courses[i].n_class && ok; j++) {
			if (grid[courses[i].time[j]] == 1) ok = 0;
		}
		if (ok) {
			choice[i] = 1; n_selected++;
			for (j = 0; j < courses[i].n_class; j++) {
				grid[courses[i].time[j]] = 1;
			}
			c_sum += courses[i].credits;
		}
	}
	printf("Total credits: %d\n", c_sum);
	printf("Number of courses selected: %d\n", n_selected);
	for (i = 0; i < 65; i++) {
		if (choice[i] == 1) {
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