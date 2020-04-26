// EE3980 HW07 Grouping Friends
// 105061110, 周柏宇
// 2020/04/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define N 100 // number of class in the hash table

// structure to store the index whose name is in the same class
typedef struct node {
	int idx;
	struct node *next;
} Node;

int n_names; // number of people
int n_links; // number of communication records
int n_SCCs; // number of friend groups
char **names; // array of names
int **adj_l; // adjacency list
int *adj_l_size; // size of list in adj_l
int *adj_l_ptr; // current position of list in adj_l
int **adj_lT; // adjacency list (transposed)
int *adj_lT_size; // size of list in adj_lT
int *adj_lT_ptr; // current position of list in adj_lT
int *f; // node's finish order
int *idx; // index mapping
int *visited; // record visited nodes
int time_DFS; // DFS clock
int *SCCs; // record nodes traversed by DFS
int SCCs_ptr; // current position of SCCs
int *C; // temporary array for CountingSort()
Node **bucket; // hash table

void readData(); // read data, construct the graph and its transposed version
int nameToIndex(char *name); // convert name to array index
unsigned long hash(char *str); // hash function
double GetTime(void); // get local time in seconds
void SCC(); // find strongly connected components of a graph
void DFS_Call(int **G, int *len, int *idx);
							  // initialization and recursive DFS function call
void DFS_d(int **G, int *len, int v); // DFS from vertex v of the graph G
void CountingSort(int *idx, int *key, int n, int k);
								 // sort the idx by its key using counting sort
void freeAll(); // free all allocated memory

int main(void)
{
	int i; // loop index
	int component_idx = 0; // index of strongly connected component
	double start, end; // timestamp

	readData(); // read data, construct the graph and its transposed version
	start = GetTime(); // start time
	SCC(); // find strongly connected components of a graph
	end = GetTime(); // end time
	// print out input information, number of subgroups and execution time
	printf("N = %d M = %d Subgroup = %d ", n_names, n_links, n_SCCs);
	printf("CPU time = %.5e\n", end - start);
	printf("Number of subgroups: %d\n", n_SCCs); // print out number of groups
	for (i = 0; SCCs[i] != -2; i++) { // print out member of groups
		if (i == 0) {
			printf("  Subgroup %d:", ++component_idx);
		}
		else if (SCCs[i] == -1) {
			printf("\n  Subgroup %d:", ++component_idx);
		}
		else {
			printf(" %s", names[SCCs[i]]);
		}
	}
	printf("\n");
	freeAll(); // free all allocated memory

	return 0;
}

void readData()
{
	int i, j, k, l; // indices
	int class; // class of a name determined by the hash function
	char tmp[20], tmp2[20], tmp3[20]; // temporary variable for input
	int *newptr; // temporary pointer
	Node *newNode; // temporary Node

	// input number of people and communication records
	scanf("%d %d", &n_names, &n_links);
	// allocate memory
	names = (char **)malloc(sizeof(char *) * n_names);
	adj_l = (int **)malloc(sizeof(int *) * n_names);
	adj_l_size = (int *)malloc(sizeof(int) * n_names);
	adj_l_ptr = (int *)calloc(n_names, sizeof(int));
	adj_lT = (int **)malloc(sizeof(int *) * n_names);
	adj_lT_size = (int *)malloc(sizeof(int) * n_names);
	adj_lT_ptr = (int *)calloc(n_names, sizeof(int));
	bucket = (Node **)calloc(N, sizeof(Node *));
	for (i = 0; i < n_names; i++) {
		scanf("%s", tmp); // input names
		names[i] = (char *)malloc(sizeof(char) * (3 * strlen(tmp) + 1));
		strcpy(names[i], tmp);
		class = hash(tmp) % N; // decide the name's class
		if (!bucket[class]) { // start of the linked list
			bucket[class] = (Node *)malloc(sizeof(Node));
			bucket[class]->idx = i; // store the index at this class
			bucket[class]->next = NULL;
		}
		else { // this class already has data
			newNode = (Node *)malloc(sizeof(Node));
			newNode->idx = i; // store the index at this class
			newNode->next = bucket[class];
			bucket[class] = newNode;
		}
	}
	for (i = 0; i < n_names; i++) {
		// initialize the adjacency list for the graph
		adj_l_size[i] = n_links / n_names;
		adj_l[i] = (int *)malloc(sizeof(int) * adj_l_size[i]);
		// initialize the adjacency list for the transposed graph
		adj_lT_size[i] = n_links / n_names;
		adj_lT[i] = (int *)malloc(sizeof(int) * adj_lT_size[i]);
	}
	for (i = 0; i < n_links; i++) {
		// input communication records
		scanf("%s %s %s", tmp, tmp2, tmp3);
		// convert names to array indices
		j = nameToIndex(tmp);
		k = nameToIndex(tmp3);

		// dynamically allocate memory for the adjacency list
		if (adj_l_ptr[j] >= adj_l_size[j]) {
			adj_l_size[j] *= 2; // double the size
			newptr = (int *)malloc(sizeof(int) * adj_l_size[j]);
											   // allocate memory with new size
			for (l = 0; l < adj_l_ptr[j]; l++) newptr[l] = adj_l[j][l];
												 // copy the data to new memory
			free(adj_l[j]); // free old memory block
			adj_l[j] = newptr; // assign the new pointer
		}
		adj_l[j][adj_l_ptr[j]] = k; // store the edge <j, k>
		adj_l_ptr[j]++; // update current position in the list

		// dynamically allocate memory for the adjacency list
		if (adj_lT_ptr[k] >= adj_lT_size[k]) {
			adj_lT_size[k] *= 2; // double the size
			newptr = (int *)malloc(sizeof(int) * adj_lT_size[k]);
											   // allocate memory with new size
			for (l = 0; l < adj_lT_ptr[k]; l++) newptr[l] = adj_lT[k][l];
												 // copy the data to new memory
			free(adj_lT[k]); // free old memory block
			adj_lT[k] = newptr; // assign the new pointer
		}
		adj_lT[k][adj_lT_ptr[k]] = j; // store the edge <k, j>
		adj_lT_ptr[k]++; // update current position in the list
	}
}

int nameToIndex(char *name) // convert name to array index
{
	Node *tmp; // temporary Node

	tmp = bucket[hash(name) % N];
	while (tmp) { // return the index if the names are matched
		if (!strcmp(name, names[tmp->idx])) return tmp->idx;
		tmp = tmp->next;
	}

	return -1;
}

unsigned long hash(char *str) // hash function
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}

void SCC() // find strongly connected components of a graph
{
	int i; // loop index

	// allocate memory
	f = (int *)malloc(sizeof(int) * n_names);
	idx = (int *)malloc(sizeof(int) * n_names);
	SCCs = (int *)calloc(n_names * 2 + 1, sizeof(int));
	visited = (int *)malloc(sizeof(int) * n_names);
	C = (int *)malloc(sizeof(int) * n_names);

	// initialize the index
	for (i = 0; i < n_names; i++) idx[i] = i;

	// traverse the graph in the given order
	DFS_Call(adj_l, adj_l_ptr, idx);

	// sort the index using the array f as key (decreasing order)
	CountingSort(idx, f, n_names, n_names);

	// sort the adjacency list of the transposed graph in-place
	// using the array f as key (decreasing order)
	for (i = 0; i < n_names; i++) {
		CountingSort(adj_lT[i], f, adj_lT_ptr[i] - 1, n_names);
	}

	// traverse the transposed graph in the given order
	DFS_Call(adj_lT, adj_lT_ptr, idx);
}

// initialization and recursive DFS function call
void DFS_Call(int **G, int *len, int *idx)
{
	int i, j; // loop index

	// initialize
	for (i = 0; i < n_names; i++) visited[i] = 0;
	time_DFS = 0;
	SCCs_ptr = 0;
	n_SCCs = 0;
	for (i = 0; i < n_names; i++) {
		j = idx[i]; // decide the vertex to travel according idx
		if (visited[j] == 0) {
			SCCs[SCCs_ptr++] = -1; // separate different subgroups with -1
			n_SCCs++; // number of subgroups increase by one
			DFS_d(G, len, j); // start DFS from vertex j
		}
	}
	SCCs[SCCs_ptr] = -2; // end of array
}


void DFS_d(int **G, int *len, int v) // DFS from vertex v of the graph G
{
	int i, j; // loop index

	visited[v] = 1; // vertex v has been visited
	SCCs[SCCs_ptr++] = v; // add vertex v to the current subgroup
	for (i = 0; i < len[v]; i++) {
		j = G[v][i]; // decide next vertex to travel
		if (visited[j] == 0) {
			DFS_d(G, len, j); // DFS from vertex j
		}
	}
	f[v] = time_DFS++; // record the finishing order
}

// sort the idx by its key using counting sort
void CountingSort(int *idx, int *key, int n, int k)
{
	int i; // loop index
	int tmp, tmp_idx; // temporary variable

	for (i = 0; i < k; i++) C[i] = 0; // initialize C to all 0
	for (i = 0; i < n; i++) {
		C[key[idx[i]]]++; // count # elements in C[key[idx[i]]]
	}
	for (i = 1; i < k; i++) {
		C[i] += C[i - 1]; // C[i] is the accumulate # of elements
	}
	for (i = n - 1; i >= 0; i--) {
		// store sorted order back to array idx
		tmp_idx = n - (C[key[idx[i]]] - 1) - 1; // decreasing order
		tmp = idx[tmp_idx];
		idx[tmp_idx] = idx[i];
		idx[i] = tmp;
		C[key[idx[i]]]--;
	}
}

void freeAll() // free all allocated memory
{
	int i; // loop index
	Node *tmp, *next;

	for (i = 0; i < n_names; i++) {
		free(names[i]);
		free(adj_l[i]);
		free(adj_lT[i]);
	}
	for (i = 0; i < N; i++) {
		tmp = bucket[i];
		while (tmp) {
			next = tmp->next;
			free(tmp);
			tmp = next;
		}
	} 
	free(names);
	free(adj_l);
	free(adj_l_size);
	free(adj_l_ptr);
	free(adj_lT);
	free(adj_lT_size);
	free(adj_lT_ptr);
	free(visited);
	free(f);
	free(idx);
	free(SCCs);
	free(C);
	free(bucket);
}
