// EE3980 HW10 Coin Set Design
// 105061110, 周柏宇
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>

int C[] = {1, 5, 10, 50};
int* g[4];

int g_R(int n, int D);
int g_TD(int n, int D);
int g_BU(int n, int D);

int main(void)
{
	int i, j, k;
	int ans[99];

	for (i = 1; i <= 99; i++) {
		for (j = 0; j < 4; j++) {
			g[j] = (int *)malloc(sizeof(int) * (i + 1));
			g[j][0] = 0;
			if (j == 0) for (k = 1; k <= i; k++) g[j][k] = k;
			else for (k = 1; k <= i; k++) g[j][k] = -1;
		}
		ans[i - 1] = g_TD(4, i);
		printf("%d: %d\n", i, ans[i - 1]);
		for (j = 0; j < 4; j++) {
			free(g[j]);
			g[j] = NULL;
		}
	}
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

// int g_BU(int n, int D)
// {
// 	int i;
// 	int tmp, min;

// 	min = D;
// 	for (i = 1; i < n; i++) {
// 		;
// 	}
// }