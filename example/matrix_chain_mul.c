#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define N 6

int p[] = {30, 35, 15, 5, 10, 20, 25};
int m[N][N];
int s[N][N];

int MCM_R(int i, int j, int n, int p[]);
int MCM_TD(int i, int j, int n, int p[]);
int MCM_BU(int n, int P[]);

int main(void)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j) m[i][j] = 0;
			else m[i][j] = -1;
			s[i][j] = -1;
		}
	}
	// printf("%d\n", MCM_R(1, N, N, p));
	// printf("%d\n", MCM_TD(1, N, N, p));
	printf("%d\n", MCM_BU(N, p));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%6d", m[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%6d", s[i][j]);
		}
		printf("\n");
	}
	return 0;
}

int MCM_R(int i, int j, int n, int p[])
{
	int k;
	int u = INT_MAX;
	int v;

	if (i == j) return 0;
	for (k = i; k <= j - 1; k++) {
		v = MCM_R(i, k, n, p) + MCM_R(k + 1, j, n, p) + p[i - 1] * p[k] * p[j];
		if (v < u) u = v;
	}
	return u;
}

int MCM_TD(int i, int j, int n, int p[])
{
	int u = INT_MAX;
	int k, v;

	if (m[i - 1][j - 1] >= 0) return m[i - 1][j - 1];
	for (k = i; k <= j - 1; k++) {
		v = MCM_TD(i, k, n, p) + MCM_TD(k + 1, j, n, p) + p[i - 1] * p[k] * p[j];
		if (v < u) u = v;
	}
	m[i - 1][j - 1] = u;
	return u;
}

int MCM_BU(int n, int P[])
{
	int i, j, k, l;
	int u, v;

	for (i = 0; i < n; i++) m[i][i] = 0;
	for (l = 2; l <= n; l++) {
		for (i = 1; i <= n - l + 1; i++) {
			j = i + l - 1;
			u = INT_MAX;
			for (k = i; k <= j - 1; k++) {
				v = m[i - 1][k - 1] + m[k + 1 - 1][j - 1] + p[i - 1] * p[k] * p[j];
				if (v < u) {
					u = v;
					s[i - 1][j - 1] = k;
				}
			}
			m[i - 1][j - 1] = u;
		}
	}
	return m[0][n - 1];
}
