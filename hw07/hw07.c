// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

// typedef struct node {
// 	int data;
// 	struct node *next;
// 	struct node *end;
// } Node;

int n_names;
int n_links;
int n_SCCs;
char **names;
int **adj_mat;
int *visited;
int *f;
int time_DFS;
int *SCCs;
int SCCs_ptr;
// Node *ll_SCCs; // linked list

void readData();
int nameToIndex(char *name);
double GetTime(void);	// get local time in seconds
void SCC();
void DFS_Call(int **adj_mat, int *order, int phase);
void DFS_d(int **adj_mat, int *order, int v, int phase);
void InsertionSort(int *keys, int *values, int low, int high);
void MergeSort(int *keys, int *values, int low, int high);
void Merge(int *keys, int *values, int low, int mid, int high);
void CountingSort(int *keys, int *values, int n, int k);

int main(void)
{
	int i;
	int subgroup_idx = 0;
	clock_t start, end;
	double time_elapsed;

	readData();
	start = clock();
	SCC();
	end = clock();
	time_elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("N = %d M = %d CPU time = %.5e\n", n_names, n_links, time_elapsed);
	printf("Number of subgroups: %d\n", n_SCCs);
	for (i = 0; i < n_names * 2 && SCCs[i] != -2; i++) {
		if (i == 0) {
			printf("  Subgroup %d:", ++subgroup_idx);
		}
		else if (SCCs[i] == -1) {
			printf("\n  Subgroup %d:", ++subgroup_idx);
		}
		else {
			printf(" %s", names[SCCs[i]]);
		}
	}
	printf("\n");

	return 0;
}

void readData()
{
	int i, j;
	char tmp[20], tmp2[20], tmp3[20];

	scanf("%d %d", &n_names, &n_links);
	names = (char **)malloc(sizeof(char *) * n_names);
	adj_mat = (int **)malloc(sizeof(int *) * n_names);
	for (i = 0; i < n_names; i++) {
		scanf("%s", &tmp);
		names[i] = (char *)malloc(sizeof(char) * (3 * strlen(tmp) + 1));
		strcpy(names[i], tmp);
	}
	for (i = 0; i < n_names; i++) {
		adj_mat[i] = (int *)malloc(sizeof(int) * n_names);
		for (j = 0; j < n_names; j++) adj_mat[i][j] = 0;
	}
	for (i = 0; i < n_links; i++) {
		scanf("%s %s %s", &tmp, &tmp2, &tmp3);
		adj_mat[nameToIndex(tmp)][nameToIndex(tmp3)] = 1;
	}
}

int nameToIndex(char *name)
{
	int i;

	for (i = 0; i < n_names; i++) {
		if (!strcmp(name, names[i])) return i;
	}
	return -1;
}

// double GetTime(void)						// get local time in seconds
// {
// 	struct timeval tv;						// variable to store time

// 	gettimeofday(&tv, NULL);				// get local time

// 	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
// }

void SCC()
{
	int i;
	int *keys;

	keys = (int *)malloc(sizeof(int) * n_names);
	for (i = 0; i < n_names; i++) keys[i] = i;

	DFS_Call(adj_mat, keys, 0);
	// InsertionSort(keys, f, 0, n_names - 1);
	// MergeSort(keys, f, 0, n_names - 1); // 1.15e-1 c9.dat
	CountingSort(keys, f, n_names, n_names); // 1.17e-1 c9.dat

	// for (i = 0; i < n_names; i++) {
	// 	printf("%s ", names[keys[i]]);
	// }
	// printf("\n");

	DFS_Call(adj_mat, keys, 1);
}

void DFS_Call(int **adj_mat, int *order, int phase)
{
	int i, j;

	visited = (int *)malloc(sizeof(int) * n_names);
	f = (int *)malloc(sizeof(int) * n_names);
	SCCs = (int *)malloc(sizeof(int) * (n_names * 2 + 1));
	for (i = 0; i < n_names; i++) {
		visited[i] = 0;
		f[i] = 0;
		SCCs[2 * i] = 0;
		SCCs[2 * i + 1] = 0;
	}
	time_DFS = 0;
	SCCs_ptr = 0;
	n_SCCs = 0;
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if (visited[j] == 0) {
			SCCs[SCCs_ptr++] = -1;
			n_SCCs++;
			DFS_d(adj_mat, order, j, phase);
		}
	}
	SCCs[SCCs_ptr] = -2;
}

void DFS_d(int **adj_mat, int *order, int v, int phase)
{
	int i, j;
	int link;

	visited[v] = 1;
	SCCs[SCCs_ptr++] = v;
	// printf("DFS_d %s d[%d] = %d\n", names[v], v, time_DFS);
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if (phase) link = adj_mat[j][v]; // transposed
		else link = adj_mat[v][j];
		if ((link == 1) && (visited[j] == 0)) {
			DFS_d(adj_mat, order, j, phase);
		}
	}
	visited[v] = 2;
	f[v] = time_DFS++; // finish order
	// printf("DFS_d %s f[%d] = %d\n", names[v], v, time_DFS);
}


void InsertionSort(int *keys, int *values, int low, int high)
{
	int j, i;							// index
	int tmp_key, tmp_value;				// temporary char pointer

	for (j = low + 1; j <= high; j++) {	// assume list[0 : j - 1] already sorted
		tmp_key = keys[j];
		tmp_value = values[j];				// copy the word at index j
		i = j - 1;					// initialize i with j - 1
		// repeat until list[i] is smaller 
		while ((i >= low) && (tmp_value > values[i])) {
			keys[i + 1] = keys[i];
			values[i + 1] = values[i];	// fill the previous word with current word
			i--;					// move on to the next word
		}
		keys[i + 1] = tmp_key;			// fill the word list[j] at index i + 1
		values[i + 1] = tmp_value;
	}
}

void MergeSort(int *keys, int *values, int low, int high)
{
	int mid;

	if (low < high) {
		mid = (low + high) / 2;
		MergeSort(keys, values, low, mid);
		MergeSort(keys, values, mid + 1, high);
		Merge(keys, values, low, mid, high);
	}
}

void Merge(int *keys, int *values, int low, int mid, int high)
{
	int k;
	int h = low; // index for lower half
	int i = low; // index for B
	int j = mid + 1; // index for upper half

	while ((h <= mid) && (j <= high)) {
		if (values[h] >= values[j]) {
			keys[i] = h;
			h++;
		}
		else {
			keys[i] = j;
			j++;
		}
		i++;
	}
	if (h > mid) {
		for (k = j; k <= high; k++) {
			keys[i] = k;
			i++;
		}
	}
	else {
		for (k = h; k <= mid; k++) {
			keys[i] = k;
			i++;
		}
	}
}

void CountingSort(int *keys, int *values, int n, int k)
{
	int i;
	int *C;

	C = (int *)malloc(sizeof(int) * k);
	for (i = 0; i < k; i++) C[i] = 0;
	for (i = 0; i < n; i++) C[values[i]]++;
	for (i = 1; i < k; i++) C[i] += C[i - 1];
	for (i = n - 1; i >= 0; i--) {
		keys[i] = n - (C[values[i]] - 1) - 1;
		C[values[i]]--;
	}
	free(C);
}