// EE3980 HW10 Coin Set Design
// 105061110, 周柏宇
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>

int g_C[] = {1, 5, 10, 50};
int x[4][100];
int g[4][100];

void P1(void);
void P2(void);
void P3(void);
void P4(void);
int g_TD(int n, int D);
int g_BU(int n, int D);
int g_sol(int n, int D, int C[]);
void verify(int C[]);

int main(void)
{
	int i, j, k;

	for (j = 0; j < 4; j++) { // # choice of coins
		g[j][0] = 0; // value = 0
		x[j][0] = 0;
		if (j == 0) { // $1
			for (k = 1; k <= 99; k++) {
				g[j][k] = k;
				x[j][k] = k;
			}
		}
		else {
			for (k = 1; k <= 99; k++) {
				g[j][k] = -1;
				x[j][k] = -1;
			}
		}
	}

	// verify(g_C);
	// k = 99;
	// printf("%d\n", g_TD(4, k));
	// printf("%d\n", g_BU(4, k));
	P1();
	P2();
	P3();
	P4();
	for (j = 0; j < 4; j++) free(g[j]);
	return 0;
}

void P1(void)
{
	int i;
	int sum = 0;

	for (i = 1; i <= 99; i++) {
		// sum += g_TD(4, i);
		sum += g_BU(4, i);
	}
	printf("For coin set {1, 5, 10, 50} the average is %.5f\n",
		(float)sum / 99);
}

void P2(void)
{
	int i, j;
	int sum;
	int min = 100000000;
	int minCoin = -1;

	for (i = 51; i <= 99; i++) {
		g_C[3] = i;
		sum = 0;
		for (j = 1; j <= 99; j++) g[3][j] = -1;
		for (j = 1; j <= 99; j++) {
			// sum += g_TD(4, j);
			sum += g_BU(4, j);
		}
		if (sum < min) {
			min = sum;
			minCoin = i;
		}
	}
	g_C[3] = 50;
	printf("Coin set {1, 5, 10, %d} has the minimum average of %.5f\n", 
		minCoin, (float)min / 99);
}

void P3(void)
{
	int i, j;
	int sum;
	int min = 100000000;
	int minCoin = -1;

	for (i = 6; i <= 49; i++) {
		g_C[2] = i;
		sum = 0;
		for (j = 1; j <= 99; j++) {
			g[2][j] = -1;
			g[3][j] = -1;
		}
		for (j = 1; j <= 99; j++) {
			// sum += g_TD(4, j);
			sum += g_BU(4, j);
		}
		if (sum < min) {
			min = sum;
			minCoin = i;
		}
	}
	printf("Coin set {1, 5, %d, 50} has the minimum average of %.5f\n",
		minCoin, (float)min / 99);
	g_C[2] = 10;
}

void P4(void)
{
	int i, j, k, l;
	int sum;
	int min = 100000000;
	int minCoinA = -1;
	int minCoinB = -1;

	for (i = 6; i <= 98; i++) {
		for (j = i + 1; j <= 99; j++) {
			g_C[2] = i;
			g_C[3] = j;
			sum = 0;
			for (k = 1; k <= 99; k++) {
				g[2][k] = -1;
				g[3][k] = -1;
			}
			for (k = 1; k <= 99; k++) {
				sum += g_TD(4, k);
				// sum += g_BU(4, k);
			}
			if (sum < min) {
				min = sum;
				minCoinA = i;
				minCoinB = j;
			}
		}
	}
	printf("Coin set {1, 5, %d, %d} has the minimum average of %.5f\n", 
		minCoinA, minCoinB, (float)min / 99);
	g_C[2] = 10;
	g_C[3] = 50;
}

int g_TD(int n, int D)
{
	int i;
	int tmp, min;

	if (g[n - 1][D] >= 0) return g[n - 1][D];
	else {
		min = g_TD(n - 1, D);
		for (i = 1; i <= (D / g_C[n - 1]); i++) {
			tmp = i + g_TD(n - 1, D - i * g_C[n - 1]);
			if (tmp < min) min = tmp;
		}
	}
	g[n - 1][D] = min;
	return min;
}

int g_BU(int n, int D)
{
	int i, j, k;
	int tmp, min;

	for (i = 1; i < n; i++) { // # choice of coins
		for (j = 1; j <= D; j++) { // total value
			min = g[i - 1][j];
			for (k = 0; k <= (j / g_C[i]); k++) {
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
	int ans;

	printf(" D: %2d %2d %2d %2d ans\n", C[0], C[1], C[2], C[3]);
	for (i = 1; i <= 99; i++) {
		ans = g_sol(4, i, C);
	}
}