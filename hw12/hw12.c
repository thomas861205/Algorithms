#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 5

int visited[N];
int stack[N * N];
int ans[N];
int n = 0;

void DFS_Call(void);
void DFS_d(int idx, int v);
void dfs(int s);
int push(int i);
int top(void);
int pop(void);

int main(void) {
	int i;

	for (i = 0; i < N; i++) visited[i] = 0;
	// dfs(0);
	DFS_Call();
	return 0;
}

void DFS_Call(void)
{
	int i;

	ans[0] = 0;
	visited[0] = 1;
	for (i = 1; i < N; i++) {
		if (!visited[i]) {
			DFS_d(1, i);
		}
	}
}

void DFS_d(int idx, int v)
{
	int i;

	visited[v] = 1;
	ans[idx] = v;
	if (idx == N - 1) {
		for (i = 0; i < N; i++) printf("%d ", ans[i]);
		printf("\n");
		visited[v] = 0;
		return;
	}
	for (i = 1; i < N; i++) {
		if (!visited[i]) {
			DFS_d(idx + 1, i);
		}
	}
	visited[v] = 0;
}

void dfs(int s)
{
	int i, j;

	push(s);
	while (n > 0) {
		i = top();
		pop();
		if (!visited[i]) {
			printf("%d\n", i);
			visited[i] = 1;
		}
		for (j = 0; j < N; j++) {
			if (!visited[j]) push(j);
		}
	}
}

int push(int i)
{
	if (n == N) return -1;
	else {
		stack[n++] = i;
		return 0;
	}
}

int top(void)
{
	return stack[n - 1];
}

int pop(void)
{
	if (n > 0) {
		n--;
		return 0;
	}
	else return -1;
}