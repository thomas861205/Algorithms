#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define INF 1000000

int N;
char **city;
int **cost;
int *visited;
int LB = INF;
int n_step = 0;

void ReadandInit(void);
void printCost(int **cost);
int **copy(int **from);
void freeCopy(int **cost);
void DFS_Call(void);
void DFS_d(int nth, int u, int v, int R, int **cost);
int RowR(int **cost);
int ColR(int **cost);

int main(void)
{
	clock_t start, end;

	ReadandInit();
	start = clock();
	DFS_Call();
	end = clock();
	printf("%d steps\n", n_step);
	printf("total cost is %d.\n", LB);
	printf("%.5lfs\n", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
}

void ReadandInit(void)
{
	int i, j;
	char ch;
	char tmp[100];

	scanf("%d ", &N);
	city = (char **)malloc(sizeof(char *) * N);
	for (i = 0; i < N; i++) {
		j = 0;
		while ((ch = getchar()) != '\n') {
			tmp[j++] = ch;
		}
		tmp[j] = '\0';
		city[i] = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
		strcpy(city[i], tmp);
		// printf("%s\n", city[i]);
	}
	cost = (int **)malloc(sizeof(int *) * N);
	for (i = 0; i < N; i++) {
		cost[i] = (int *)malloc(sizeof(int) * N);
		for (j = 0; j < N; j++) {
			scanf("%d", &cost[i][j]);
			if (i == j) cost[i][j] = INF;
		}
	}
	visited = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) {
		visited[i] = 0;
	}
}

void printCost(int** cost)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (cost[i][j] == INF) printf("  x ");
			else printf("%3d ", cost[i][j]);
		}
		printf("\n");
	}
}

int **copy(int **from)
{
	int i, j;
	int **to;

	to = (int **)malloc(sizeof(int *) * N);
	for (i = 0; i < N; i++) {
		to[i] = (int *)malloc(sizeof(int) * N);
		for (j = 0; j < N; j++) to[i][j] = from[i][j];
	}
	return to;
}

void DFS_Call(void)
{
	int i;
	int R = 0;
	int **cost_cpy;

	visited[0] = 1; // node 0 has been visited
	cost_cpy = copy(cost);
	R += RowR(cost_cpy);
	R += ColR(cost_cpy);
	// printCost(cost_cpy);
	n_step++;
	// printf("%d. %d\n\n", n_step, R);
	for (i = 1; i < N; i++) {
		if (!visited[i]) {
			DFS_d(1, 0, i, R, cost_cpy);
		}
	}
	freeCopy(cost_cpy);
}

void DFS_d(int nth, int u, int v, int R, int **cost)
{
	int i;
	int r = 0;
	int **cost_cpy;

	// printf("(%d, %d)\n", u, v);
	visited[v] = 1;
	cost_cpy = copy(cost);
	r +=  cost[u][v];
	for (i = 0; i < N; i++) cost_cpy[u][i] = INF;
	for (i = 0; i < N; i++) cost_cpy[i][v] = INF;
	cost_cpy[v][u] = INF;
	r += RowR(cost_cpy);
	r += ColR(cost_cpy);
	// printCost(cost_cpy);
	n_step++;
	// printf("%d. %d %d\n\n", n_step, LB, R + r);

	if (nth == N - 1) {
		LB = R;
		visited[v] = 0;
		return;
	}
	if (R + r <= LB) {
		for (i = 1; i < N; i++) {
			if (!visited[i]) {
				DFS_d(nth + 1, v, i, R + r, cost_cpy);
			}
		}
	}
	visited[v] = 0;
	freeCopy(cost_cpy);
}

int RowR(int **cost)
{
	int i, j;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		min = INF;
		for (j = 0; j < N; j++) {
			if ((cost[i][j] < min))
				min = cost[i][j];
		}
		if (min != INF) {
			for (j = 0; j < N; j++) {
				if (cost[i][j] != INF) cost[i][j] -= min;
			}
			s_min += min;
		}
	}
	return s_min;
}

int ColR(int **cost)
{
	int i, j;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		min = INF;
		for (j = 0; j < N; j++) {
			if ((cost[j][i] < min))
				min = cost[j][i];
		}
		if (min != INF) {
			for (j = 0; j < N; j++) {
				if (cost[j][i] != INF) cost[j][i] -= min;
			}
			s_min += min;
		}
	}
	return s_min;
}

void freeCopy(int **cost)
{
	int i;

	for (i = 0; i < N; i++) free(cost[i]);
	free(cost);
}
