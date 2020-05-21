// EE3980 HW10 Coin Set Design
// 105061110, 周柏宇
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
// #include <sys/time.h>
#define MAX 25

int g_C[] = {1, 5, 10, 50};
int test_C[] = {1, 5, 18, 25};
int x[4][MAX + 1];
int g[4][MAX + 1];
int sol[MAX - 6][MAX - 6]; // [6 ~ 98][7 ~ 99]

void P1(void);
void P2(void);
void P3(void);
void P4(void);
int g_TD(int n, int D);
int g_BU(int n, int D, int skip);
int g_sol(int n, int D, int C[]);
void verify(int C[]);
void printSol(void);
double GetTime(void); // get local time in seconds

int main(void)
{
	int i, j, k;
	clock_t start, end;

	for (i = 0; i < (MAX - 6); i++) {
		for (j = 0; j < (MAX - 6); j++) sol[i][j] = -1;
	}
	for (j = 0; j < 4; j++) { // # choice of coins
		g[j][0] = 0; // value = 0
		x[j][0] = 0;
		if (j == 0) { // $1
			for (k = 1; k <= MAX; k++) {
				g[j][k] = k;
				x[j][k] = k;
			}
		}
		else {
			for (k = 1; k <= MAX; k++) {
				g[j][k] = -1;
				x[j][k] = -1;
			}
		}
	}
	start = clock();
	P1();
	// P2();
	// P3();
	P4();
	end = clock();
	// verify(test_C);
	printSol();
	printf("%.5fs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	return 0;
}

void P1(void)
{
	int i;
	int sum = 0;

	for (i = 1; i <= MAX; i++) {
		sum += g_TD(4, i);
		// sum += g_BU(4, i, 1);
	}
	printf("For coin set {1, 5, 10, 50} the average is %.5f\n",
		(float)sum / MAX);
}

void P2(void)
{
	int i, j;
	int sum;
	int min = INT_MAX;
	int minCoin = -1;

	for (i = 51; i <= MAX; i++) {
		g_C[3] = i;
		sum = 0;
		for (j = 1; j <= MAX; j++) {
			g[3][j] = -1; // init
			sum += g_TD(4, j);
			// sum += g_BU(4, j, 3);
		}
		if (sum < min) {
			min = sum; // record min # coins
			minCoin = i; // $i
		}
	}
	g_C[3] = 50; // restore coin set
	printf("Coin set {1, 5, 10, %d} has the minimum average of %.5f\n", 
		minCoin, (float)min / MAX);
}

void P3(void)
{
	int i, j;
	int sum;
	int min = INT_MAX;
	int minCoin = -1;

	for (i = 6; i <= 49; i++) {
		g_C[2] = i;
		sum = 0;
		for (j = 1; j <= MAX; j++) {
			g[2][j] = -1;
			g[3][j] = -1;
		}
		for (j = 1; j <= MAX; j++) {
			sum += g_TD(4, j);
			// sum += g_BU(4, j, 2);
		}
		if (sum < min) {
			min = sum;
			minCoin = i;
		}
	}
	printf("Coin set {1, 5, %d, 50} has the minimum average of %.5f\n",
		minCoin, (float)min / MAX);
	g_C[2] = 10;
}

void P4(void)
{
	int i, j, k, l;
	int sum;
	int min = INT_MAX;
	int minCoinA = -1;
	int minCoinB = -1;

	for (i = 6; i <= 98; i++) {
		for (j = i + 1; j <= MAX; j++) {
			g_C[2] = i;
			g_C[3] = j;
			sum = 0;
			for (k = 1; k <= MAX; k++) {
				g[2][k] = -1;
				g[3][k] = -1;
			}
			for (k = 1; k <= MAX; k++) {
				sum += g_TD(4, k);
				// sum += g_BU(4, k, 2);
			}
			sol[i - 6][j - 7] = sum;
			if (sum < min) {
				min = sum;
				minCoinA = i;
				minCoinB = j;
			}
		}
	}
	printf("Coin set {1, 5, %d, %d} has the minimum average of %.5f\n", 
		minCoinA, minCoinB, (float)min / MAX);
	g_C[2] = 10;
	g_C[3] = 50;
}

int g_TD(int n, int D)
{
	int i;
	int tmp, min;

	if (g[n - 1][D] >= 0) return g[n - 1][D];
	min = g_TD(n - 1, D);
	for (i = 1; i <= (D / g_C[n - 1]); i++) {
		tmp = i + g_TD(n - 1, D - i * g_C[n - 1]);
		if (tmp < min) min = tmp;
	}
	g[n - 1][D] = min;

	return min;
}

int g_BU(int n, int D, int skip)
{
	int i, j, k;
	int tmp, min;

	for (i = skip; i < n; i++) { // # choice of coins
		for (j = 1; j <= D; j++) { // total value
			min = g[i - 1][j];
			for (k = 1; k <= (j / g_C[i]); k++) {
				tmp = k + g[i - 1][j - k * g_C[i]];
				if (tmp < min) min = tmp;
			}
			g[i][j] = min;
		}
	}

	return g[n - 1][D];
}

int g_sol(int n, int D, int C[])
{
	int i, j, k;
	int tmp, min;
	int sol[4];

	for (i = 1; i < n; i++) { // # choice of coins
		for (j = 1; j <= D; j++) { // total value
			min = g[i - 1][j];
			x[i][j] = 0;
			for (k = 0; k <= (j / C[i]); k++) {
				tmp = k + g[i - 1][j - k * C[i]];
				if (tmp < min) {
					min = tmp;
					x[i][j] = k;
				}
			}
			g[i][j] = min;
		}
	}

	tmp = D;
	for (i = n - 1; i >= 0; i--) {
		sol[i] = x[i][tmp];
		tmp -= sol[i] * C[i];
	}
	printf("%2d: %2d %2d %2d %2d %3d", 
		D, sol[0], sol[1], sol[2], sol[3], g[n - 1][D]);
	printf("\n");
	return g[n - 1][D];
}

void verify(int C[])
{
	int i;
	int ans = 0;

	printf(" D: %2d %2d %2d %2d ans\n", C[0], C[1], C[2], C[3]);
	printf("---------------------\n");
	for (i = 1; i <= MAX; i++) {
		ans += g_sol(4, i, C);
	}
	printf("---------------------\n");
	printf("Total:            %3d\n", ans);
	printf("Average:      %.5f\n", (float)ans / MAX);
}

void printSol(void)
{
	int i, j;

	for (i = 0; i < (MAX - 6); i++) {
		for (j = 0; j < (MAX - 6); j++) {
			if (sol[i][j] > 0) printf("%4d", sol[i][j]);
			else printf("  _ ");
		}
		printf("\n");
	}
}

// double GetTime(void)						// get local time in seconds
// {
// 	struct timeval tv;						// variable to store time

// 	gettimeofday(&tv, NULL);				// get local time

// 	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
// }