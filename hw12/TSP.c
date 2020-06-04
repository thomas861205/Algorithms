#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000

typedef struct node {
	int x;
	int y;
	int lb;
	struct node *next;
} NODE;

int N;
char **city;
int **cost;
int lb;
int *r_available;
int *c_available;
int *path;
NODE *q = NULL;

void readInput(void);
int rowR(int real);
int colR(int real);
int isCycle(int x, int y);
void decidePath(void);
void print(void);
void solve(void);
int retrieveCost(void);

int main(void)
{
	int i;

	readInput();
	print();
	printf("lb=%d\n", rowR(1));
	print();
	printf("lb=%d\n", colR(1));
	print();
	// solve();
	// decidePath();
	return 0;
}

void readInput(void)
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
			if (i == j) cost[i][j] = -1;
		}
	}
	r_available = (int *)malloc(sizeof(int) * N);
	c_available = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) {
		r_available[i] = 1;
		c_available[i] = 1;
	}
}

void print(void)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (cost[i][j] < 0) printf("  x ");
			else printf("%3d ", cost[i][j]);
		}
		printf("\n");
	}
}

int rowR(int real)
{
	int i, j;
	int has0;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		if (r_available[i]) {
			has0 = 0;
			min = INT_MAX;
			for (j = 0; j < N && !has0; j++) {
				if ((cost[i][j] == 0) && c_available[j])
					has0 = 1;
			}
			if (!has0) {
				for (j = 0; j < N; j++) {
					if (c_available[j]) {
						if (cost[i][j] < 0) continue; //
						else if (cost[i][j] < min) min = cost[i][j];
					}
				}
				s_min = (min == INT_MAX) ? s_min : s_min + min;
				if (real) {
					for (j = 0; j < N; j++) cost[i][j] -= min;
				}
			}
		}
	}
	return s_min;
}

int colR(int real)
{
	int i, j;
	int has0;
	int min;
	int s_min = 0;

	for (i = 0; i < N; i++) {
		if (c_available[i]) {
			has0 = 0;
			min = INT_MAX;
			for (j = 0; j < N && !has0; j++) {
				if ((cost[j][i] == 0) && r_available[j])
					has0 = 1;
			}
			if (!has0) {
				for (j = 0; j < N; j++) {
					if (r_available[j]) {
						if (cost[j][i] < 0) continue; //
						else if (cost[j][i] < min) min = cost[j][i];
					}
				}
				s_min = (min == INT_MAX) ? s_min : s_min + min;
				if (real) {
					for (j = 0; j < N; j++) cost[j][i] -= min;
				}
			}
		}
	}
	return s_min;
}

int isCycle(int x, int y) {
	int i = 0;
	int cpy;
	int tmp = x;
	int flag = -1;
	// -1: initial
	// 0: loop, not ok
	// 1: full loop, ok
	// 2: no loop, ok

	cpy = path[x];
	path[x] = y;
	while (flag == -1) {
		tmp = path[tmp];
		i++;
		if (tmp == -1) flag = 2;
		else if (path[tmp] == x) {
			if (i == N - 1) flag = 1;
			else flag = 0;
		}
	}
	path[x] = cpy;
	if (flag) return 0;
	else return 1;
	// return 0;
}

void solve(void)
{
	int i, j;
	int nl; // # links
	int lb = 0;
	int l_lb, r_lb;
	int x, y;
	int tmp;
	int max_diff;
	NODE *new_node;

	path = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) path[i] = -1;
	r_available = (int *)malloc(sizeof(int) * N);
	c_available = (int *)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++) {
		r_available[i] = 1;
		c_available[i] = 1;
	}

	/*
	real reduce
	*/
	lb += rowR(1);
	lb += colR(1);
	print();
	for (nl = 0; nl < N; nl++) {
		/*
		find link
		*/
		max_diff = -1;
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (i != j && r_available[i] && c_available[j] && !isCycle(i, j)) {
					r_lb = 0; // diff
					tmp = cost[i][j];
					cost[i][j] = -1;
					r_lb += rowR(0);
					r_lb += colR(0);
					cost[i][j] = tmp;
					if (r_lb > max_diff) {
						max_diff = r_lb;
						x = i;
						y = j;
					}
				}
			}
		}
		path[x] = y;
		printf("[%d][%d] = %d\n", x + 1, y + 1, max_diff);
		/*
		find right-lb, left-lb
		*/
		l_lb = lb;
		r_available[x] = 0;
		c_available[y] = 0;
		tmp = cost[y][x];
		cost[y][x] = -1;
		l_lb += rowR(0);
		l_lb += colR(0);
		r_available[x] = 1;
		c_available[y] = 1;
		cost[y][x] = tmp;

		r_lb = lb; // diff
		tmp = cost[x][y];
		cost[x][y] = -1;
		r_lb += rowR(0);
		r_lb += colR(0);
		cost[x][y] = tmp;
		/*
		store link in queue
		*/
		new_node = (NODE *)malloc(sizeof(NODE) * 1);
		new_node->x = x;
		new_node->y = y;
		new_node->lb = r_lb;
		if (q == NULL) {
			new_node->next = NULL;
			q = new_node;
		}
		else {
			new_node->next = q;
			q = new_node;
		}
		/*
		cross deactivate
		*/
		r_available[x] = 0;
		c_available[y] = 0;
		/*
		transpose deactivate
		*/
		cost[y][x] = -1;
		/*
		real reduce
		*/
		rowR(1);
		colR(1);
		/*
		update lb
		*/
		printf("%d %d %d\n", lb, l_lb, r_lb);
		lb = l_lb;
		print();
	}

}

void decidePath(void)
{
	int i = 0;
	int tmp = 0;

	while (i < N) {
		printf("%d -> %d\n", tmp, path[tmp]);
		tmp = path[tmp];
		i++;
	}
}
