#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#define INF 1000000

// data structure to store the information of links
typedef struct node {
	int idx; // index of the city
	int R; // lower bound when adding the link
} NODE;

int N; // number of cities
char **city; // name of the cities
int **cost; // cost table
int *visited; // record if the city is visited
int LB = INF; // lower bound of the cost
int n_step = 0; // number of steps (calculation of lower bound)
int *row_avl; // record if the row is available
int *col_avl; // record if the column is available
int *ans; // visiting order
int *ans_tmp; // temporary visiting order

void ReadandInit(void); // read input and initialize
int **copy(int **from); // copy the cost table
void freeCopy(int **cost); // free duplicate of cost table
void DFS_Call(void);
void DFS_d(int nth, int u, int R, int **cost);
int RowR(int **cost, int doit); // reduce row cost
int ColR(int **cost, int doit); // reduce column cost
int nextA(NODE *A, int n, NODE *ret);
void freeAll(void); // free allocated memory

int main(void)
{
	int i;
	clock_t start, end;

	ReadandInit();
	start = clock();
	DFS_Call();
	end = clock();
	printf("%d steps\n", n_step);
	printf("%.5lfs\n", (double)(end - start) / CLOCKS_PER_SEC);
	printf("Minimum distance route:\n");
	for (i = 0; i < N; i++) {
		printf(" %s -> %s\n", city[ans[i]], city[ans[(i + 1) % N]]);
	}
	printf("Total travelling distance: %d\n", LB);
	freeAll();
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
	row_avl = (int *)malloc(sizeof(int) * N);
	col_avl = (int *)malloc(sizeof(int) * N);
	ans = (int *)malloc(sizeof(int) * N);
	ans_tmp = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) {
		visited[i] = 0;
		row_avl[i] = 1;
		col_avl[i] = 1;
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
	int R;
	int **cost_cpy;

	visited[0] = 1; // city 0 has been visited
	cost_cpy = copy(cost); // a duplicate of cost table
	R = RowR(cost_cpy, 1) + ColR(cost_cpy, 1); // row and column reduce
	n_step++; // number of steps (calculation of lower bound) increase by 1
	DFS_d(1, 0, R, cost_cpy);
	freeCopy(cost_cpy); // free allocated memory
}

void DFS_d(int nth, int u, int R, int **cost)
{
	int i; // loop index
	int r; // temporary lower bound
	int tmp; // temporary variable
	int i_adj = 0; // index
	int **cost_cpy; // a duplicate of cost table
	int status; // return value of next()
	int v; // link (u, v)
	NODE min; // next minimum
	NODE *adj = (NODE *)malloc(sizeof(NODE) * (N - nth)); // adjacent cities

	row_avl[u] = 0; // set row u to INF
	ans_tmp[nth - 1] = u;
	for (i = 0; i < N; i++) {
		if (!visited[i]) { // try unvisited cities
			col_avl[i] = 0; // set column i to INF
			tmp = cost[i][u];
			cost[i][u] = INF; // set cost[i][u] to INF
			adj[i_adj].idx = i;
			// calculate the lower bound
			adj[i_adj].R = R + cost[u][i] + RowR(cost, 0) + ColR(cost, 0);
			i_adj++;
			// number of steps (calculation of lower bound) increase by 1
			n_step++;
			cost[i][u] = tmp; // recover cost[i][u]
			col_avl[i] = 1; // recover column i
		}
	}
	if (nth == N - 1) { // lowest non-leaf node
		ans_tmp[nth] = adj[0].idx;
		row_avl[u] = 1; // recover row u
		tmp = adj[0].R;
		if (tmp < LB) {
			LB = tmp; // update lower bound
			for (i = 0; i < N; i++) ans[i] = ans_tmp[i];
		}
		free(adj);
		return;
	}
	// try next city according to least-cost, stop when dominated
	while (((status = nextA(adj, i_adj--, &min) != -1)) && (min.R <= LB)) {
		v = min.idx;
		visited[v] = 1; // set city v as visited
		col_avl[v] = 0; // set column v to INF
		cost_cpy = copy(cost); // a duplicate of cost table
		cost_cpy[v][u] = INF; // set cost[v][u] to INF

		r = R + cost[u][v] + RowR(cost_cpy, 1) + ColR(cost_cpy, 1);
		DFS_d(nth + 1, v, r, cost_cpy); // next city

		col_avl[v] = 1; // recover col v
		visited[v] = 0; // set city v as unvisited
		freeCopy(cost_cpy);
	}
	row_avl[u] = 1; // recover row u
	free(adj);
}

int nextA(NODE *A, int n, NODE *ret)
{
	int i;
	int min = INF;
	int i_min;

	if (n < 0) return -1;
	else if (n == 1) {
		*ret = A[0];
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (A[i].R < min) {
			min = A[i].R;
			i_min = i;
		}
	}
	*ret = A[i_min]; A[i_min] = A[n - 1]; A[n - 1] = *ret;
	return 0;
}

int RowR(int **cost, int doit) // reduce row cost
{
	int i, j; // loop indices
	int min; // minimum of the row
	int s_min = 0; // cumulated minimum

	for (i = 0; i < N; i++) {
		if (row_avl[i]) {
			min = INF;
			for (j = 0; j < N; j++) {
				if (col_avl[j] && (cost[i][j] < min)) {
					min = cost[i][j]; // update the minimum of the row
				}
			}
			if (min != INF) {
				if (doit) { // update the table of cost
					for (j = 0; j < N; j++) {
						if (col_avl[j] && cost[i][j] != INF) {
							cost[i][j] -= min;
						}
					}
				}
				s_min += min; // accumulate the minimum
			}
		}
	}
	return s_min;
}

int ColR(int **cost, int doit) // reduce column cost
{
	int i, j; // loop indices
	int min; // minimum of the column
	int s_min = 0; // cumulated minimum

	for (i = 0; i < N; i++) {
		if (col_avl[i]) {
			min = INF;
			for (j = 0; j < N; j++) {
				if (row_avl[j] && (cost[j][i] < min)) {
					min = cost[j][i]; // update the minimum of the column
				}
			}
			if (min != INF) {
				if (doit) { // update the table of cost
					for (j = 0; j < N; j++) {
						if (row_avl[j] && cost[j][i] != INF) {
							cost[j][i] -= min;
						}
					}
				}
				s_min += min; // accumulate the minimum
			}
		}
	}
	return s_min;
}

void freeCopy(int **cost) // free duplicate of cost table
{
	int i;

	for (i = 0; i < N; i++) free(cost[i]);
	free(cost);
}

void freeAll(void) // free allocated memory
{
	int i;

	for (i = 0; i < N; i++) {
		free(city[i]);
		free(cost[i]);
	}
	free(city);
	free(cost);
	free(visited);
	free(row_avl);
	free(col_avl);
	free(ans);
	free(ans_tmp);
}
