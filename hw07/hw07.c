// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/time.h>
#include <time.h>

// typedef struct sDFSInfo {
// 	int p, d, f;
// } DFSInfo;

int n_names;
int n_links;
char **names;
int **graph;
int **graphT; // transpose graph
int *visited;
int *p, *d, *f;
int time_DFS;
int *SCCs;
int SCCs_ptr;

void readData();
int nameToIndex(char *name);
void SCC();
void DFS_Call(int **graph, int *order);
void DFS_d(int **graph, int *order, int v);
void InsertionSort(int *keys, int *values);

int main(void)
{
	int i;
	time_t start, end;

	start = time(NULL);
	readData();
	SCC();
	end = time(NULL);
	// for (i = 0 ; i < n_names; i++) printf("%s\n", names[i]);
	// printf("%d\n", nameToIndex(names[2]));
	// for (i = 0; i < n_names; i++) printf("%d ", d[i]);
	// printf("\n");
	// for (i = 0; i < n_names; i++) printf("%d ", f[i]);
	// printf("\n");
	printf("%lf s\n", difftime(end, start));
	return 0;
}

void readData()
{
	int i, j;
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
		graph[nameToIndex(tmp)][nameToIndex(tmp3)] = 1;
		graphT[nameToIndex(tmp3)][nameToIndex(tmp)] = 1;
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

	// printf("Before:\n");
	// for (i = 0; i < n_names; i++) printf("f[%d] = %d ", i, f[i]);
	// printf("\n");
	DFS_Call(graph, keys);
	InsertionSort(keys, f);
	// printf("After:\n");
	// for (i = 0; i < n_names; i++) {
	// 	printf("f[%d] = %d ", keys[i], f[i]);
	// }
	// printf("\n");
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
	p = (int *)malloc(sizeof(int) * n_names);
	d = (int *)malloc(sizeof(int) * n_names);
	f = (int *)malloc(sizeof(int) * n_names);
	SCCs = (int *)malloc(sizeof(int) * n_names * 2);
	for (i = 0; i < n_names; i++) {
		visited[i] = 0;
		p[i] = 0;
		d[i] = 0;
		f[i] = 0;
		SCCs[2 * i] = 0;
		SCCs[2 * i + 1] = 0;
	}
	time_DFS = 0;
	SCCs_ptr = 0;
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if (visited[j] == 0) {
			SCCs[SCCs_ptr] = -1;
			SCCs_ptr++;
			DFS_d(graph, order, j);
		}
	}
	SCCs[SCCs_ptr] = -2;

	for (i = 0; i < n_names * 2 && SCCs[i] != -2; i++) {
		if (SCCs[i] != -1) printf("%s ", names[SCCs[i]]);
		else printf("\n");
	}
	printf("\n");
}

void DFS_d(int **graph, int *order, int v)
{
	int i, j;

	visited[v] = 1;
	time_DFS++; // time start at 1
	d[v] = time_DFS;
	SCCs[SCCs_ptr] = v;
	SCCs_ptr++;
	// printf("DFS_d %s d[%d] = %d\n", names[v], v, time_DFS);
	for (i = 0; i < n_names; i++) {
		j = order[i];
		if ((graph[v][j] == 1) && (visited[j] == 0)) {
			p[j] = v;
			DFS_d(graph, order, j);
		}
	}
	visited[v] = 2;
	time_DFS = time_DFS + 1;
	f[v] = time_DFS;
	// printf("DFS_d %s f[%d] = %d\n", names[v], v, time_DFS);
}


void InsertionSort(int *keys, int *values)
{
	int j, i;							// index
	int tmp_key, tmp_value;				// temporary char pointer

	for (j = 1; j < n_names; j++) {	// assume list[0 : j - 1] already sorted
		tmp_key = keys[j];
		tmp_value = values[j];				// copy the word at index j
		i = j - 1;					// initialize i with j - 1
		// repeat until list[i] is smaller 
		while ((i >= 0) && (tmp_value > values[i])) {
			keys[i + 1] = keys[i];
			values[i + 1] = values[i];	// fill the previous word with current word
			i--;					// move on to the next word
		}
		keys[i + 1] = tmp_key;			// fill the word list[j] at index i + 1
		values[i + 1] = tmp_value;
	}
}