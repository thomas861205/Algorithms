// EE3980 HW10 Coin Set Design
// 105061110, 周柏宇
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>

int C[] = {1, 5, 10, 50};
int* g[4];
int g_min;

float P1(void);
void P2(void);
void P3(void);
void P4(void);
int g_R(int n, int D);
int g_TD(int n, int D);
int g_BU(int n, int D);

int main(void)
{
	int i, j, k;
	// int avg;

	for (j = 0; j < 4; j++) {
		g[j] = (int *)malloc(sizeof(int) * (99 + 1));
		g[j][0] = 0;
		if (j == 0) for (k = 1; k <= 99; k++) g[j][k] = k;
		else for (k = 1; k <= 99; k++) g[j][k] = -1;
	}
	g_min = P1();
	P2();
	P3();
	for (j = 0; j < 4; j++) free(g[j]);
	return 0;
}

int g_R(int n, int D)
{
	int i;
	int tmp, min;

	if (n == 1) return D; // D $1 coins
	else {
		min = g_R(n - 1, D);
		for (i = 1; i <= (D / C[n - 1]); i++) {
			tmp = i + g_R(n - 1, D - i * C[n - 1]);
			if (tmp < min) min = tmp;
		}
	}
	return min;
}

float P1(void)
{
	int i;
	int sum = 0;
	float avg;

	for (i = 1; i <= 99; i++) {
		sum += g_BU(4, i);
	}
	avg = (float)sum / 99;
	printf("For coin set {1, 5, 10, 50} the average is %.5f\n", avg);
	return sum;
}

void P2(void)
{
	int i, j;
	int sum;
	int min = g_min;
	int min_coin = 50;

	for (i = 51; i <= 99; i++) {
		C[3] = i;
		sum = 0;
		for (j = 1; j <= 99; j++) {
			sum += g_BU(4, j);
		}
		// printf("%d: %d\n", i, sum);
		if (sum < min) {
			min = sum;
			min_coin = i;
		}
	}
	printf("Coin set {1, 5, 10, %d} has the minimum average of %.5f\n", 
		min_coin, (float)min / 99);
	C[3] = 50;
}

void P3(void)
{
	int i, j;
	int sum;
	int min = g_min;
	int min_coin = 5;

	// printf("%d: %d\n", 5, min);
	for (i = 6; i <= 49; i++) {
		C[2] = i;
		sum = 0;
		for (j = 1; j <= 99; j++) {
			g[2][j] = -1;
			g[3][j] = -1;
		}
		for (j = 1; j <= 99; j++) {
			sum += g_BU(4, j);
		}
		// printf("%d: %d\n", i, sum);
		if (sum < min) {
			min = sum;
			min_coin = i;
		}
	}
	printf("Coin set {1, 5, %d, 50} has the minimum average of %.5f\n", 
		min_coin, (float)min / 99);
	C[2] = 10;
}

void P4(void)
{
	;
}

int g_TD(int n, int D)
{
	int i;
	int tmp, min;

	// if (n == 1) return D; // D $1 coins
	if (g[n - 1][D] >= 0) return g[n - 1][D];
	else {
		min = g_TD(n - 1, D);
		for (i = 1; i <= (D / C[n - 1]); i++) {
			// printf("Calling g_TD(%d, %d)\n", n - 1, D - i * C[n - 1]);
			tmp = i + g_TD(n - 1, D - i * C[n - 1]);
			// printf("g_TD(%d, %d) = %d\n", n, D, tmp);
			if (tmp < min) min = tmp;
		}
	}
	// printf("g_TD(%d, %d) = %d\n", n, D, min);
	g[n - 1][D] = min;
	return min;
}

int g_BU(int n, int D)
{
	int i, j;
	int tmp, min;

	min = D;
	for (i = 1; i < n; i++) {
		min = g[i - 1][D];
		for (j = 1; j <= (D / C[i]); j++) {
			tmp = j + g[i - 1][D - j * C[i]];
			if (tmp < min) min = tmp;
		}
		g[i][D] = min;
	}

	return g[n - 1][D];
}