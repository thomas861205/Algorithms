// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int n_names;
int n_links;
int n_SCCs;
char **names;
int **adj_mat;
int **adj_l;
int *adj_l_size;
int *adj_l_ptr;
int **adj_lT;
int *adj_lT_size;
int *adj_lT_ptr;
int *visited;
int *f;
int time_DFS;
int *SCCs;
int SCCs_ptr;

void readData();
int nameToIndex(char *name);
void printList();
double GetTime(void);	// get local time in seconds
void SCC();
void DFS_Call(int **G, int *len, int *order);
void DFS_d(int **G, int *len, int v);
void InsertionSort(int *keys, int *values, int low, int high);
void MergeSort(int *keys, int *values, int low, int high);
void Merge(int *keys, int *values, int low, int mid, int high);
void CountingSort(int *keys, int *values, int n, int k);
void Sort(int *list, int len);
void Sort_Call(int *keys);

int main(void)
{
	int i;
	int subgroup_idx = 0;
	clock_t start, end;
	double time_elapsed;

	readData();
	// printList();
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
	int i, j, k;
	char tmp[20], tmp2[20], tmp3[20];

	scanf("%d %d", &n_names, &n_links);
	names = (char **)malloc(sizeof(char *) * n_names);
	adj_mat = (int **)malloc(sizeof(int *) * n_names);
	adj_l = (int **)malloc(sizeof(int *) * n_names);
	adj_l_size = (int *)malloc(sizeof(int) * n_names);
	adj_l_ptr = (int *)calloc(n_names, sizeof(int));
	adj_lT = (int **)malloc(sizeof(int *) * n_names);
	adj_lT_size = (int *)malloc(sizeof(int) * n_names);
	adj_lT_ptr = (int *)calloc(n_names, sizeof(int));
	// for (i = 0; i < n_names; i++) printf("%d ", adj_l_ptr[i]);
	// printf("\n");
	for (i = 0; i < n_names; i++) {
		scanf("%s", &tmp);
		names[i] = (char *)malloc(sizeof(char) * (3 * strlen(tmp) + 1));
		strcpy(names[i], tmp);
	}
	for (i = 0; i < n_names; i++) {
		adj_mat[i] = (int *)calloc(n_names, sizeof(int));
		adj_l_size[i] = n_links / n_names;
		adj_l[i] = (int *)malloc(sizeof(int) * adj_l_size[i]);
		adj_lT_size[i] = n_links / n_names;
		adj_lT[i] = (int *)malloc(sizeof(int) * adj_lT_size[i]);
	}
	for (i = 0; i < n_links; i++) {
		scanf("%s %s %s", &tmp, &tmp2, &tmp3);
		j = nameToIndex(tmp);
		k = nameToIndex(tmp3);
		adj_mat[j][k] = 1;

		if (adj_l_ptr[j] >= adj_l_size[j]) {
			adj_l_size[j] *= 2;
			// int *newptr = (int *)realloc(adj_l[j], adj_l_size[j]);
			int *newptr = (int *)malloc(sizeof(int) * adj_l_size[j]);
			for (int l = 0; l < adj_l_ptr[j]; l++) newptr[l] = adj_l[j][l];
			free(adj_l[j]);
			// printf("%p %p\n", newptr, adj_l[j]);
			adj_l[j] = newptr;
		}
		adj_l[j][adj_l_ptr[j]] = k;
		adj_l_ptr[j]++;

		if (adj_lT_ptr[k] >= adj_lT_size[k]) {
			adj_lT_size[k] *= 2;
			int *newptr = (int *)malloc(sizeof(int) * adj_lT_size[k]);
			for (int l = 0; l < adj_lT_ptr[k]; l++) newptr[l] = adj_lT[k][l];
			free(adj_lT[k]);
			adj_lT[k] = newptr;
		}
		adj_lT[k][adj_lT_ptr[k]] = j;
		adj_lT_ptr[k]++;
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

void printList()
{
	int i, j;

	// for (i = 0; i < n_names; i++) {
	// 	printf("%d ", adj_l_size[i]);
	// }
	// printf("\n");
	// for (i = 0; i < n_names; i++) {
	// 	printf("%d ", adj_l_ptr[i]);
	// }
	// printf("\n");
	printf("G: \n");
	for (i = 0; i < n_names; i++) {
		printf("%d :", i);
		for (j = 0; j < adj_l_ptr[i]; j++) printf(" %d", adj_l[i][j]);
		printf("\n");
	}
	printf("G_T: \n");
	for (i = 0; i < n_names; i++) {
		printf("%d :", i);
		for (j = 0; j < adj_lT_ptr[i]; j++) printf(" %d", adj_lT[i][j]);
		printf("\n");
	}
	// for (i = 0; i < n_names; i++) {
	// 	printf("%d :", i);
	// 	for (j = 0; j < n_names; j++) {
	// 		if (adj_mat[i][j]) printf("%d ", j);
	// 	}
	// 	printf("\n");
	// }
}

// double GetTime(void)						// get local time in seconds
// {
// 	struct timeval tv;						// variable to store time

// 	gettimeofday(&tv, NULL);				// get local time

// 	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
// }

void SCC()
{
	int i, j;
	int *keys;

	keys = (int *)malloc(sizeof(int) * n_names);
	for (i = 0; i < n_names; i++) keys[i] = i;

	DFS_Call(adj_l, adj_l_ptr, keys);
	// InsertionSort(keys, f, 0, n_names - 1);
	// MergeSort(keys, f, 0, n_names - 1); // 1.15e-1 c9.dat
	// CountingSort(keys, f, n_names, n_names); // 1.17e-1 c9.dat
	Sort_Call(keys);

	for (i = 0; i < n_names; i++) Sort(adj_lT[i], adj_lT_ptr[i]);

	DFS_Call(adj_lT, adj_lT_ptr, keys);
}

void Sort_Call(int *keys)
{
	int j, i;							// index
	int tmp_key;				// temporary char pointer
	int tmp_value;

	for (j = 1; j < n_names; j++) {	// assume list[0 : j - 1] already sorted
		tmp_key = keys[j];
		tmp_value = f[j];				// copy the word at index j
		i = j - 1;					// initialize i with j - 1
		// repeat until list[i] is smaller 
		while ((i >= 0) && (tmp_value > f[i])) {
			keys[i + 1] = keys[i];
			// f[list[i + 1]] = f[list[i]];	// fill the previous word with current word
			i--;					// move on to the next word
		}
		keys[i + 1] = tmp_key;			// fill the word list[j] at index i + 1
		// f[list[i + 1]] = tmp_value;
	}
}

void Sort(int *list, int len)
{
	int j, i;							// index
	int tmp_key, tmp_value;				// temporary char pointer

	for (j = 1; j < len; j++) {	// assume list[0 : j - 1] already sorted
		tmp_key = list[j];
		tmp_value = f[list[j]];				// copy the word at index j
		i = j - 1;					// initialize i with j - 1
		// repeat until list[i] is smaller 
		while ((i >= 0) && (tmp_value > f[list[i]])) {
			list[i + 1] = list[i];
			// f[list[i + 1]] = f[list[i]];	// fill the previous word with current word
			i--;					// move on to the next word
		}
		list[i + 1] = tmp_key;			// fill the word list[j] at index i + 1
		// f[list[i + 1]] = tmp_value;
	}
}

void DFS_Call(int **G, int *len, int *order)
{
	int i, j;

	visited = (int *)calloc(n_names, sizeof(int));
	f = (int *)calloc(n_names, sizeof(int));
	SCCs = (int *)calloc(n_names * 2 + 1, sizeof(int));
	time_DFS = 0;
	SCCs_ptr = 0;
	n_SCCs = 0;
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if (visited[j] == 0) {
			SCCs[SCCs_ptr++] = -1;
			n_SCCs++;
			DFS_d(G, len, j);
		}
	}
	SCCs[SCCs_ptr] = -2;
}

void DFS_d(int **G, int *len, int v)
{
	int i, j;

	visited[v] = 1;
	SCCs[SCCs_ptr++] = v;
	for (i = 0; i < len[v]; i++) {
		j = G[v][i];
		if (visited[j] == 0) {
			DFS_d(G, len, j);
		}
	}
	visited[v] = 2;
	f[v] = time_DFS++; // finish order
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

	C = (int *)calloc(k, sizeof(int));
	for (i = 0; i < n; i++) C[values[i]]++;
	for (i = 1; i < k; i++) C[i] += C[i - 1];
	for (i = n - 1; i >= 0; i--) {
		keys[i] = n - (C[values[i]] - 1) - 1;
		C[values[i]]--;
	}
	free(C);
}