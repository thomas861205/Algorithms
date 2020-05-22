// EE3980 HW10 Coin Set Design
// 105061110, 周柏宇
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#define MAX 99 // D from 1 to MAX

int g_C[] = {1, 5, 10, 50};  // {$1, $5, $10, $50}
int x[4][MAX + 1]; // solution array
int g[4][MAX + 1]; // dynamic programming (DP) table
int sol[MAX - 6][MAX - 6]; // answer table for P4

void P1(void); // problem 1
void P2(void); // problem 2
void P3(void); // problem 3
void P4(void); // problem 4
int g_TD(int n, int D); // calculate g using top-down DP
int g_BU(int n, int D, int skip); // calculate g using bottom-up DP
int g_sol(int n, int D, int C[]); // calculate g with solution
void verify(int C[]); // verify the answer to the problem using coin set C
double GetTime(void); // get local time in seconds

int main(void)
{
	int i, j, k;
	double start, end;

	for (j = 0; j < 4; j++) { // # choice of coins
		g[j][0] = 0; // D = 0, needn't to take
		x[j][0] = 0;
		if (j == 0) { // $1
			for (k = 1; k <= MAX; k++) { // D = 1 ~ 99
				g[j][k] = k; // k $1 coins
				x[j][k] = k;
			}
		}
		else { // C2, C3, C4
			for (k = 1; k <= MAX; k++) {
				g[j][k] = -1; // initialize as -1
				x[j][k] = -1;
			}
		}
	}
	for (i = 0; i < (MAX - 6); i++) {
		for (j = 0; j < (MAX - 6); j++) sol[i][j] = -1;
	}
	// start = GetTime();
	P1(); // problem 1
	// verify(g_C);
	P2(); // problem 2
	P3(); // problem 3
	P4(); // problem 4
	// end = GetTime();
	// printf("CPU time%.5fs\n", end - start);
	// printf("  CPU time = %.5e seconds\n", end - start);

	return 0;
}

void P1(void) // problem 1
{
	// Given {C1, C2, C3, C4} = {$1, $5, $10, $50},
	// find the average number of coins for D = 1 to 99.
	int i;
	int sum = 0; // sum of solutions

	for (i = 1; i <= MAX; i++) { // D = 1 ~ 99
		// accumulate g[4][i], skipping $1 calculation
		sum += g_BU(4, i, 1); // using bottom-up DP
		// sum += g_TD(4, i); // using top-down DP
	}
	printf("For coin set {1, 5, 10, 50} the average is %.5f\n",
		(float)sum / MAX);
}

void P2(void) // problem 2
{
	// Given {$1, $5, $10, C4}. Assuming C4 is a variable,
	// find its value that minimizes the average for D = 1 to 99.
	int i, j;
	int sum; // sum of solutions
	int min = INT_MAX; // minimal solution
	int minCoin = -1; // coin achieving the minimum

	for (i = 11; i <= MAX; i++) { // C4 = 11 ~ 99
		g_C[3] = i; // vary C4
		sum = 0;
		for (j = 1; j <= MAX; j++) { // D = 1 ~ 99
			g[3][j] = -1; // initialize the table for n = 4
			// accumulate g[4][D], skipping {$1, $5, $10} calculation
			sum += g_BU(4, j, 3); // using bottom-up DP
			// sum += g_TD(4, j); // using top-down DP
		}
		if (sum < min) {
			min = sum; // store minimum
			minCoin = i; // $i achieves minimum
		}
	}
	g_C[3] = 50; // restore coin set
	printf("Coin set {1, 5, 10, %d} has the minimum average of %.5f\n", 
		minCoin, (float)min / MAX);
}

void P3(void) // problem 3
{
	// Given {$1, $5, C3, $50}. Assuming C3 is a variable,
	// find its value that minimizes the average for D = 1 to 99.
	int i, j;
	int sum; // sum of solutions
	int min = INT_MAX; // minimal solution
	int minCoin = -1; // coin achieving the minimum

	for (i = 6; i <= 49; i++) { // C3 = 6 ~ 49
		g_C[2] = i; // vary C3
		sum = 0;
		for (j = 1; j <= MAX; j++) {
			g[2][j] = -1; // initialize the table for n = 3
			g[3][j] = -1; // initialize the table for n = 4
		}
		for (j = 1; j <= MAX; j++) { // D = 1 ~ 99
			// accumulate g[4][D], skipping {$1, $5} calculation
			sum += g_BU(4, j, 2); // using bottom-up DP
			// sum += g_TD(4, j); // using top-down DP
		}
		if (sum < min) {
			min = sum; // store minimum
			minCoin = i; // $i achieves minimum
		}
	}
	g_C[2] = 10; // restore coin set
	printf("Coin set {1, 5, %d, 50} has the minimum average of %.5f\n",
		minCoin, (float)min / MAX);
}

void P4(void) // problem 4
{
	// Given {$1, $5, C3, C4}. Assuming both C3 and C4 are variables,
	// find their values that minimizes the average for D = 1 to 99.
	int i, j, k;
	int sum; // sum of solutions
	int min = INT_MAX; // minimal solution
	int minCoinA = -1; // C3 achieving the minimum
	int minCoinB = -1; // C4 achieving the minimum

	for (i = 6; i <= 98; i++) { // C3 = 6 ~ 98
		for (j = i + 1; j <= MAX; j++) { // C4 = 7 ~ 99
			g_C[2] = i; // vary C3
			g_C[3] = j; // vary C4
			sum = 0;
			for (k = 1; k <= MAX; k++) {
				g[2][k] = -1; // initialize the table for n = 3
				g[3][k] = -1; // initialize the table for n = 4
			}
			for (k = 1; k <= MAX; k++) { // D = 1 ~ 99
				// accumulate g[4][D], skipping {$1, $5} calculation
				sum += g_BU(4, k, 2); // using bottom-up DP
				// sum += g_TD(4, k); // using top-down DP
			}
			sol[i - 6][j - 7] = sum; // save the sum of solutions
			if (sum < min) {
				min = sum; // store minimum
				minCoinA = i; // C3 = $i
				minCoinB = j; // C4 = $j
			}
		}
	}
	g_C[2] = 10; // restore coin set
	g_C[3] = 50;
	printf("Coin set {1, 5, %d, %d} has the minimum average of %.5f\n", 
		minCoinA, minCoinB, (float)min / MAX);
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

// double GetTime(void)						// get local time in seconds
// {
// 	struct timeval tv;						// variable to store time

// 	gettimeofday(&tv, NULL);				// get local time

// 	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
// }