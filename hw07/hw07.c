// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

// potential improvement
// space:
// adjacent matrix -> link list
// graphT
// SCCs
// 
// time: other sorting algorithm
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/time.h>
#include <time.h>

// typedef struct sDFSInfo {
// 	int p, d, f;
// } DFSInfo;

typedef struct node {
	int data;
	struct node *next;
} Node;

int n_names;
int n_links;
int n_SCCs;
char **names;
int **graph;
int **graphT; // transpose graph
int *visited;
int *f;
int time_DFS;
int *SCCs;
int SCCs_ptr;

void readData();
int nameToIndex(char *name);
void SCC();
void DFS_Call(int **graph, int *order);
void DFS_d(int **graph, int *order, int v);
void InsertionSort(int *keys, int *values, int low, int high);
void MergeSort(int *keys, int *values, int low, int high);
void Merge(int *keys, int *values, int low, int mid, int high);

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
	int i, j, k;
	char tmp[20], tmp2[20], tmp3[20];

	scanf("%d", &n_names);
	scanf("%d", &n_links);
	names = (char **)malloc(sizeof(char *) * n_names);
	for (i = 0; i < n_names; i++) {
		scanf("%s", &tmp);
		names[i] = (char *)malloc(sizeof(char) * (3 * strlen(tmp) + 1));
		// names[i] = (char *)malloc(sizeof(char) * (strlen(tmp) + 1)); // test c0.dat
		strcpy(names[i], tmp);
	}

	graph = (int **)malloc(sizeof(int *) * n_names);
	graphT = (int **)malloc(sizeof(int *) * n_names);
	for (i = 0; i < n_names; i++) {
		graph[i] = (int *)malloc(sizeof(int) * n_names);
		graphT[i] = (int *)malloc(sizeof(int) * n_names);
		for (j = 0; j < n_names; j++) {
			graph[i][j] = 0;
			graphT[i][j] = 0;
		}
	}

	for (i = 0; i < n_links; i++) {
		scanf("%s %s %s", &tmp, &tmp2, &tmp3);
		// printf("%d -> %d\n", nameToIndex(tmp), nameToIndex(tmp3));
		j = nameToIndex(tmp);
		k = nameToIndex(tmp3);
		graph[j][k] = 1;
		graphT[k][j] = 1;
	}

	// for (i = 0; i < n_names; i++) {
	// 	for (j = 0; j < n_names; j++) printf("%d ", graph[i][j]);
	// 	printf("\n");
	// }
	// printf("\n");
	// for (i = 0; i < n_names; i++) {
	// 	for (j = 0; j < n_names; j++) printf("%d ", graphT[i][j]);
	// 	printf("\n");
	// }
}

int nameToIndex(char *name)
{
	int i;

	for (i = 0; i < n_names; i++) {
		if (!strcmp(name, names[i])) return i;
	}
	return -1;
}

void SCC()
{
	int i;
	int *keys;

	keys = (int *)malloc(sizeof(int) * n_names);
	for (i = 0; i < n_names; i++) {
		keys[i] = i;
		// printf("%s ", names[keys[i]]);
	}
	// printf("\n");

	DFS_Call(graph, keys);
	// InsertionSort(keys, f);
	MergeSort(keys, f, 0, n_names - 1);

	// for (i = 0; i < n_names; i++) {
	// 	printf("%s ", names[keys[i]]);
	// }
	// printf("\n");

	DFS_Call(graphT, keys);
}

void DFS_Call(int **graph, int *order)
{
	int i, j;

	visited = (int *)malloc(sizeof(int) * n_names);
	f = (int *)malloc(sizeof(int) * n_names);
	SCCs = (int *)malloc(sizeof(int) * n_names * 2);
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
			SCCs[SCCs_ptr] = -1;
			SCCs_ptr++;
			n_SCCs++;
			DFS_d(graph, order, j);
		}
	}
	SCCs[SCCs_ptr] = -2;
}

void DFS_d(int **graph, int *order, int v)
{
	int i, j;

	visited[v] = 1;
	time_DFS++; // time start at 1
	SCCs[SCCs_ptr] = v;
	SCCs_ptr++;
	// printf("DFS_d %s d[%d] = %d\n", names[v], v, time_DFS);
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if ((graph[v][j] == 1) && (visited[j] == 0)) {
			DFS_d(graph, order, j);
		}
	}
	visited[v] = 2;
	time_DFS++;
	f[v] = time_DFS;
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

	if (high - low > 0) InsertionSort(keys, values, low, high);
	else {
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