#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int V, E;
int **G;
char **names;
int *visited, *f;
int t;

void readData(void);
void DFS_Call(void);
void DFS_d(int v);

int main(void)
{
	int i, j;
	int *t;

	readData();
/*	for (i = 0; i < V; i++) printf("%s\n", names[i]);
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) printf("%d ", G[i][j]);
		printf("\n");
	}
*/
	DFS_Call();

	return 0;
}

void readData(void)
{
	int i, j;
	char tmp[100];
	int v1, v2;

	scanf("%d %d", &V, &E);
	names = (char **)malloc(sizeof(char *) * V);
	G = (int **)malloc(sizeof(int *) * V);
	for (i = 0; i < V; i++) {
		scanf("%s", tmp);
		names[i] = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(names[i], tmp);
		G[i] = (int *)calloc(V, sizeof(int));
	}
	for (i = 0; i < E; i++) {
		scanf("%d %d", &v1, &v2);
		G[v1 - 1][v2 - 1] = 1;
	}
	visited = (int *)malloc(sizeof(int) * V);
	f = (int *)malloc(sizeof(int) * V);
}

void DFS_Call(void)
{
	int i;

	for (i = 0; i < V; i++) {
		visited[i] = 0;
		f[i] = 0;
	}
	t = 0;
	for (i = 0; i < V; i++) {
		if (visited[i] == 0) DFS_d(i);
	}
}

void DFS_d(int v)
{
	int i;

	visited[v] = 1;
	t++;
	for (i = 0; i < V; i++) {
		if (G[v][i] && visited[i] == 0) DFS_d(i);
	}
	f[v] = ++t;
	printf("f[%d] = %d\n", v + 1, f[v]);
}
