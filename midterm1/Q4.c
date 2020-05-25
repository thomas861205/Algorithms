// Q4 EE3980 Algorithms midterm exam
// u105061110, 周柏宇
// 2020/04/28

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int V;
int E;
int R = 1;                  // Repetitions
int cnt = 0;

double GetTime(void);       // current time in sec
// please create your own global variabls and functions for this problem
char **names;
int **G;
int n2i(char *name);
int *visited;
void solve_call();
void tps(int v);
int *slist;

int main(void)
{
	int i, j;
	char tmp[100], tmp2[100], tmp3[100];
	int m, n;

	scanf("%d %d", &V, &E);
	names = (char **)malloc(sizeof(char *) * V);
	for (i = 0; i < V; i++) {
		scanf("%s", tmp);
		names[i] = (char *)malloc(sizeof(char) * (3 * strlen(tmp) + 1));
		strcpy(names[i], tmp);
	}

	G = (int **)malloc(sizeof(int *) * V);
	for (i = 0; i < V; i++) {
		G[i] = (int *)malloc(sizeof(int) * V);
		for (j = 0; j < V; j++) {
			G[i][j] = 0;
		}
	}
	for (i = 0; i < E; i++) {
		scanf("%s %s %s", tmp, tmp2, tmp3);
		m = n2i(tmp);
		n = n2i(tmp3);
//		printf("%d -> %d\n", m ,n);
		G[m][n] = 1;
	}
	solve_call();
}



void solve_call()
{
	int i;

	visited = (int *)calloc(V, sizeof(int));
	slist = (int *)malloc(sizeof(int) * (2 * V));

	for (i = 0; i < V; i++) {
		if (!visited[i]) {
			tps(i);
			slist[cnt++] = -1;
		} 
	}
	for (i = 0; i < cnt; i++) {
		// if (slist[i] == -1) printf("\n");
		// else printf("%s ", names[slist[i]]);
		if (slist[i + 1] == -1) printf("%s \n", names[i]);
	}
}

void tps(int v)
{
	int i;

	visited[v] = 1;
	slist[cnt++] = v;
	for (i = 0; i < V; i++) {
		if (G[v][i] && !visited[i]) {
			tps(i);
		}
	}
}

int n2i(char *name)
{
	int i;

	for(i = 0; i < V; i++) {
		if (!strcmp(name, names[i])) return i;
	}
	return -1;
}

double GetTime(void)        // current time in sec
{
    struct timeval tv;

//    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}
