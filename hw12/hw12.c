// EE3980 HW12 Travelling Salesperson Problem
// 105061110, 周柏宇
// 2020/06/07

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define INF 1000000

// data structure to store the information of links
typedef struct node {
	int idx; // index of the city
	int R; // lower bound when adding the link
} NODE;

int N; // number of cities
char **city; // name of cities
int **cost; // cost table
int *visited; // record if the city is visited
int LB = INF; // lower bound of the cost
int n_step = 0; // number of steps (calculation of lower bound)
int *row_avl; // record if the row is available
int *col_avl; // record if the column is available
int *ans; // optimal visiting order
int *ans_tmp; // temporary visiting order

void ReadandInit(void); // read input and initialize
int **copy(int **from); // copy the cost table
void freeCopy(int **cost); // free duplicate of cost table
void LC_Call(void); // initialization call of least cost traversal
void LC_d(int nth, int u, int R, int **cost); // recursive least cost traversal
int RowR(int **cost, int doit); // row reduce
int ColR(int **cost, int doit); // column reduce
int nextA(NODE *A, int n, NODE *ret); // next minimum of the array
void freeAll(void); // free allocated memory
double GetTime(void); // get local time in seconds

int main(void)
{
	int i; // loop index
	double start, end; // timestamps

	ReadandInit(); // read input and initialize
	start = GetTime(); // start the timer
	LC_Call(); // initialization call of the least cost traversal
	end = GetTime(); // stop the timer
	// printf("%d steps\n", n_step);
	// printf("%.5lfs\n", (end - start));
	printf("Minimum distance route:\n");
	for (i = 0; i < N; i++) { // print the visiting order of cities
		printf(" %s -> %s\n", city[ans[i]], city[ans[(i + 1) % N]]);
	}
	printf("Total travelling distance: %d\n", LB);
	freeAll(); // free allocated memory
	return 0;
}

void ReadandInit(void) // read input and initialize
{
	int i, j; // loop indices
	char ch; // temporary character
	char tmp[100]; // temporary string

	scanf("%d ", &N); // input number of cities
	city = (char **)malloc(sizeof(char *) * N);
	for (i = 0; i < N; i++) {
		j = 0;
		while ((ch = getchar()) != '\n') { // input name of cities
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
			scanf("%d", &cost[i][j]); // input cost
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

int **copy(int **from) // copy the cost table
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

void freeCopy(int **cost) // free duplicate of cost table
{
	int i;

	for (i = 0; i < N; i++) free(cost[i]);
	free(cost);
}

void LC_Call(void) // initialization call of least cost traversal
{
	int R; // temporary lower bound
	int **cost_cpy; // a duplicate of cost table

	visited[0] = 1; // city 0 has been visited
	cost_cpy = copy(cost); // a duplicate of cost table
	R = RowR(cost_cpy, 1) + ColR(cost_cpy, 1); // row and column reduce
	n_step++; // number of steps (calculation of lower bound) increase by 1
	LC_d(1, 0, R, cost_cpy); // recursive least cost traversal
	freeCopy(cost_cpy); // free allocated memory
}

void LC_d(int nth, int u, int R, int **cost) // recursive least cost traversal
{
	int i; // loop index
	int r; // temporary lower bound
	int tmp; // temporary variable
	int i_adj = 0; // index
	int **cost_cpy; // a duplicate of cost table
	int status; // return value of next()
	int v; // link (u, v)
	NODE min; // next minimum
	NODE *adj = (NODE *)malloc(sizeof(NODE) * (N - nth)); // available cities

	row_avl[u] = 0; // set row u to INF
	ans_tmp[nth - 1] = u; // put city u in array of visit
	for (i = 0; i < N; i++) {
		if (!visited[i]) { // try all available cities
			col_avl[i] = 0; // set column i to INF
			tmp = cost[i][u];
			cost[i][u] = INF; // set cost[i][u] to INF

			adj[i_adj].idx = i; // store the available city
			// calculate the lower bound (without modify the cost table)
			adj[i_adj].R = R + cost[u][i] + RowR(cost, 0) + ColR(cost, 0);
			// number of steps (calculation of lower bound) increase by 1
			n_step++;
			i_adj++; // number of available cities increase by 1

			cost[i][u] = tmp; // recover cost[i][u]
			col_avl[i] = 1; // recover column i
		}
	}
	if (nth == N - 1) { // lowest non-leaf node
		ans_tmp[nth] = adj[0].idx; // put the leaf node in array of visit
		row_avl[u] = 1; // recover row u
		tmp = adj[0].R;
		if (tmp < LB) { // if the circuit has lower cost
			LB = tmp; // update the lower bound
			for (i = 0; i < N; i++) {
				ans[i] = ans_tmp[i]; // update the array of visit
			}
		}
		free(adj);
		return;
	}
	// Try next city in least-cost order, stop if the lower bound is dominated.
	while (((status = nextA(adj, i_adj--, &min) != -1)) && (min.R <= LB)) {
		v = min.idx;
		visited[v] = 1; // set city v as visited
		col_avl[v] = 0; // set column v to INF
		cost_cpy = copy(cost); // a duplicate of cost table
		cost_cpy[v][u] = INF; // set cost[v][u] to INF

		// calculate the lower bound again, but this time modify the cost table
		r = R + cost[u][v] + RowR(cost_cpy, 1) + ColR(cost_cpy, 1);
		LC_d(nth + 1, v, r, cost_cpy); // next city

		col_avl[v] = 1; // recover col v
		visited[v] = 0; // set city v as unvisited
		freeCopy(cost_cpy);
	}
	row_avl[u] = 1; // recover row u
	free(adj);
}

int nextA(NODE *A, int n, NODE *ret) // next minimum of the array
{
	int i; // loop index
	int min = INF; // minimum
	int i_min; // index of the minimum

	if (n < 0) return -1; //fail to return
	else if (n == 1) { // only one element, no need to move
		*ret = A[0];
		return 0;
	}
	for (i = 0; i < n; i++) { // find the minimum and its index
		if (A[i].R < min) {
			min = A[i].R;
			i_min = i;
		}
	}
	// move the minimum to the end of array
	*ret = A[i_min]; A[i_min] = A[n - 1]; A[n - 1] = *ret;
	return 0;
}

int RowR(int **cost, int doit) // row reduce
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
				if (doit) { // modify the table of cost
					for (j = 0; j < N; j++) {
						if (col_avl[j] && (cost[i][j] != INF)) {
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

int ColR(int **cost, int doit) // column reduce
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
				if (doit) { // modify the table of cost
					for (j = 0; j < N; j++) {
						if (row_avl[j] && (cost[j][i] != INF)) {
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

double GetTime(void) // get local time in seconds
{
	struct timeval tv; // variable to store time

	gettimeofday(&tv, NULL); // get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec; // return local time in seconds
}
