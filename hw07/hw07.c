// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int n_names;
int n_links;
char **names;
int *graph;

void readData();

int main(void)
{
	readData();
	return 0;
}

void readData()
{
	int i;
	char tmp[100];

	scanf("%d", &n_names);
	names = (char **)malloc(sizeof(char *) * n_names);
	for (i = 0; i < n; i++) {
		scanf("%s", &tmp);
		printf("%d\n", sizeof(tmp));
		// names[i] = (char *)malloc(sizeof(char));
	}
}