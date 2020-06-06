#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define INF 1000000

typedef struct node {
	int idx;
	int R;
} NODE;

int N;
char **city;
int **cost;
int *visited;
int LB = INF;
int n_step = 0;
int *row_avl;
int *col_avl;
int *ans;

void ReadandInit(void);
void printCost(int **cost);
int **copy(int **from);
void freeCopy(int **cost);
void DFS_Call(void);
void DFS_d(int nth, int u, int R, int **cost);
int RowR(int **cost, int doit);
int ColR(int **cost, int doit);
int RowCf(int **cost, int *min);
int ColCf(int **cost, int *min);
void array2heap(NODE *A, int n);
void heapify(NODE *A, int i, int n);
int nextH(NODE *A, int n, NODE *ret);
int nextA(NODE *A, int n, NODE *ret);

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
	row_avl = (int *)malloc(sizeof(int) * N);
	col_avl = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) {
		visited[i] = 0;
		row_avl[i] = 1;
		col_avl[i] = 1;
	}
}

void printCost(int** cost)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			// if (cost[i][j] == INF) printf("  x ");
			if (!row_avl[i] || !col_avl[j] || cost[i][j] == INF)
				printf("  x ");
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
	int R;
	int **cost_cpy;

	visited[0] = 1; // node 0 has been visited
	cost_cpy = copy(cost);
	R = RowR(cost_cpy, 1) + ColR(cost_cpy, 1);
	n_step++;
	DFS_d(1, 0, R, cost_cpy);
	freeCopy(cost_cpy);
}

void DFS_d(int nth, int u, int R, int **cost)
{
	int i; // loop index
	int r; // temporary lower bound
	int tmp; // temporary variable
	int i_order = 0; // index
	int **cost_cpy; // a duplicate of cost table
	int status; // return value of next()
	int v; // link (u, v)
	NODE min; // next minimum
	NODE *order = (NODE *)malloc(sizeof(NODE) * (N - nth));

	row_avl[u] = 0; // set row u to INF
	for (i = 0; i < N; i++) {
		if (!visited[i]) { // try all available cities
			col_avl[i] = 0; // set column i to INF
			tmp = cost[i][u];
			cost[i][u] = INF; // set cost[i][u] to INF

			order[i_order].idx = i;
			order[i_order].R = R + cost[u][i] + RowR(cost, 0) + ColR(cost, 0);
			n_step++; // number of steps (calculation of lower bound) increase by 1
			// printf("%d. %d %d\n", n_step, i + 1, order[i_order].R);
			i_order++;

			cost[i][u] = tmp; // recover cost[i][u]
			col_avl[i] = 1; // recover column i
		}
	}
	if (nth == N - 1) { // lowest non-leaf node
		row_avl[u] = 1; // recover row u
		tmp = order[0].R;
		// printf("done %d %d\n", LB, tmp);
		LB = tmp < LB ? tmp : LB; // update lower bound
		free(order);
		return;
	}

	// array2heap(order, i_order); // make the array 'order' a min heap

	// while (((status = nextH(order, i_order--, &min) != -1)) && (min.R <= LB)) {
	while (((status = nextA(order, i_order--, &min) != -1)) && (min.R <= LB)) {
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
	free(order);
}

void array2heap(NODE *A, int n)
{
	int i;

	for (i = n / 2 - 1; i >= 0; i--) {
		heapify(A, i, n);
	}
}

void heapify(NODE *A, int i, int n)
{
	int j, done;
	NODE tmp;

	j = 2 * (i + 1) - 1;
	tmp = A[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if ((j < n - 1) && (A[j].R > A[j + 1].R)) j++;
		if (tmp.R < A[j].R) done = 1;
		else {
			A[(j + 1) / 2 - 1] = A[j];
			j = 2 * (j + 1) - 1;
		}
	}
	A[(j + 1) / 2 - 1] = tmp;
}

int nextH(NODE *A, int n, NODE *ret) // return next minimum in heap
{
	if (n == 1) {
		// only one element in heap, no need to swap and heapify
		*ret = A[0];
		return 0;
	}
	else if (n > 1) {
		// more than one element, need to swap
		*ret = A[0]; A[0] = A[n - 1]; A[n - 1] = *ret;
		heapify(A, 0, n - 1);
		return 0;
	}
	else return -1;
}

int nextA(NODE *A, int n, NODE *ret){
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

int RowR(int **cost, int doit)
{
	int i, j;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		if (row_avl[i]) {
			min = INF;
			for (j = 0; j < N; j++) {
				if (col_avl[j] && (cost[i][j] < min))
					min = cost[i][j];
			}
			if (min != INF) {
				if (doit) {
					for (j = 0; j < N; j++) {
						if (col_avl[j] && cost[i][j] != INF)
							cost[i][j] -= min;
					}
				}
				s_min += min;
			}
		}
	}
	return s_min;
}

int ColR(int **cost, int doit)
{
	int i, j;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		if (col_avl[i]) {
			min = INF;
			for (j = 0; j < N; j++) {
				if (row_avl[j] && (cost[j][i] < min))
					min = cost[j][i];
			}
			if (min != INF) {
				if (doit) {
					for (j = 0; j < N; j++) {
						if (row_avl[j] && cost[j][i] != INF)
							cost[j][i] -= min;
					}
				}
				s_min += min;
			}
		}
	}
	return s_min;
}

// int RowCf(int **cost, int *min);
// int ColCf(int **cost, int *min);

void freeCopy(int **cost)
{
	int i;

	for (i = 0; i < N; i++) free(cost[i]);
	free(cost);
}
