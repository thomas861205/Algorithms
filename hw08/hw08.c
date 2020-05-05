#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct course {
	char *id;
	int credits;
	int capacity;
	int time[5][13];
	char *name;
} COURSE;

char col_name[][13] = {
	"CLASS NUMBER",
	"CREDITS",
	"CAPACITY",
	"TIME",
	"NAME"
};

int n;
COURSE *courses;

void readData(void);

int main(void)
{
	int i;
	int cnt = 0;
	int name_start;
	int col_cnt = 0;
	char ch;
	char tmp[200];

	scanf("%d\n", &n);
	while ((ch=getchar()) != EOF) {
		tmp[cnt++] = ch;
		if (ch == ' ') {
			col_cnt++;
		}
		else if (ch == '\n') {
			tmp[cnt] = '\0';
			printf("%d %s", name_start, tmp + name_start);
			cnt = 0;
			col_cnt = 0;
		}
		if (col_cnt == 3) name_start = cnt; 
		// printf("%c", ch);
	}
	return 0;
}


void readData(void)
{
	int i;
	char tmp[100];

	scanf("%d", &n);
	courses = (COURSE *)malloc(sizeof(COURSE) * n);
	for (i = 0; i < n; i++) {
		scanf("%s", tmp);
		courses[i].id = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(courses[i].id, tmp);
		scanf("%d %d", courses[i].credits, courses[i].capacity);
		scanf("%s", tmp);
	}
}
