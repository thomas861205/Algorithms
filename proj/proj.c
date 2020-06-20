// EE3980 Term Project Encoding utf-8 Files
// 105061110, 周柏宇
// 2020/06/20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define b2B(x) (x % 8 ? x / 8 + 1 : x / 8) // convert bits to bytes needed
#define N_BUCKET 1000 // number of buckets

typedef struct node {
	unsigned int val; // symbol hash value
	int freq; // frequency
	char *symbol; // utf-8 symbol
	struct node *l; // left child
	struct node *r; // right child
} NODE;

int n_byte = 0; // # bytes using utf-8
int n_bit = 0; // # bits using Huffman code
int n_usymb = 0; // # unique symbols
int *code; // array for Huffman codes
NODE *bucket[N_BUCKET]; // hash table
NODE **minHeap; // min heap

void readText(void); // read utf-8 encoded paragraph
int det_nB(char ch); // determine n-byte symbol
unsigned int hash(char *str); // hash function
NODE *find(unsigned int val); // find the node with certain value
void insert(unsigned int val, char *symbol, int n_B);
                                   // insert a new symbol to proper linked list
void hash2minHeap(NODE **bucket, int n_usymb); // transfer nodes to a min heap
void minHeapify(NODE **list, int i, int n); // enforce min heap property
NODE *minHeapRemoveMin(NODE **list, int n); // remove minimum from the min heap
void minHeapInsertion(NODE **list, int n, NODE *new_node);
                                                   // insert a node to min heap
void Tree(int n_heap); // construct the merged tree for Huffman code
void printHuffmanCode(NODE *node, int i, int bit); // print Huffman code
double GetTime(void); // get local time in seconds

int main(void)
{
	int i;
	double t0, t1; // timestamps

	t0 = GetTime();
	for (i = 0; i < N_BUCKET; i++) bucket[i] = NULL;
	readText(); // read utf-8 encoded paragraph
	hash2minHeap(bucket, n_usymb); // transfer nodes to a min heap
	Tree(n_usymb); // construct the merged tree for Huffman code
	printHuffmanCode(minHeap[0], 0, -1); // print Huffman code
	printf("Number of Chars read: %d\n", n_byte);
	printf("  Huffman Coding needs %d bits, %d bytes\n", n_bit, b2B(n_bit));
	// print the ratio of bytes needed with and without using Huffman code
	printf("  Ratio = %.4f %%\n", b2B(n_bit) * 100.0 / n_byte);
	t1 = GetTime();
	// printf("  CPU time = %.5e seconds\n", t1 - t0);
	return 0;
}

void readText(void) // read utf-8 encoded paragraph
{
	char ch;
	char symbol[5]; // utf-8 symbol
	int idx_ch;
	int type; // 0, 1, 2, 3, 4-byte symbol
	unsigned long val; // hash value
	NODE *tmp;

	symbol[0] = '\0';
	while ((ch = getchar()) != EOF) {
		n_byte++;
		type = det_nB(ch); // determine n-byte symbol
		if (type) { // skip 10xx xxxx
			if (symbol[0] != '\0') {
				val = hash(symbol);
				tmp = find(val); // find the node with the hash value
				if (tmp == NULL) { // not found
					insert(val, symbol, type);
					n_usymb++; // number of unique symbols increases by 1
				}
				else tmp->freq++;
			}
			idx_ch = 0;
			symbol[type] = '\0';
		}
		symbol[idx_ch++] = ch;
	}
	val = hash(symbol);
	tmp = find(val); // find the node with the hash value
	if (tmp == NULL) { // not found
		insert(val, symbol, type);
		n_usymb++; // number of unique symbols increases by 1
	}
	else tmp->freq++;
}

int det_nB(char ch) // determine n-byte symbol
{
	if (!(ch & 0x80)) return 1;             // 0xxx xxxx
	else {                                  // 1xxx xxxx
		if (!(ch & 0x40)) return 0;         // 10xx xxxx
		else {                              // 11xx xxxx
			if (!(ch & 0x20)) return 2;     // 110x xxxx
			else {                          // 111x xxxx
				if (!(ch & 0x10)) return 3; // 1110 xxxx
				else return 4;              // 1111 xxxx
			}
		}
	}
}

unsigned int hash(char *str) // hash function
{
	unsigned int hash = 0;
	unsigned char c;

	while ((c = *str++)) {
		hash = (hash << 8) + c;
	}

	return hash;
}

NODE *find(unsigned int val) // find the node with certain value
{
	NODE *tmp = bucket[val % N_BUCKET];

	while (tmp != NULL) {
		if (tmp->val == val) return tmp;
		tmp = tmp->l;
	}
	return NULL;
}

// insert a new symbol to proper linked list
void insert(unsigned int val, char *symbol, int n_B)
{
	int rep = val % N_BUCKET; // representative of the class
	NODE *new_node = (NODE *)malloc(sizeof(NODE));

	new_node->val = val;
	new_node->freq = 1;
	new_node->symbol = (char *)malloc(sizeof(char) * (n_B + 1));
	strcpy(new_node->symbol, symbol);
	if (!bucket[rep]) { // start of the class
		new_node->l = NULL;
		bucket[rep] = new_node;
	}
	else {
		new_node->l = bucket[rep];
		bucket[rep] = new_node;
	}
}

void hash2minHeap(NODE **bucket, int n_usymb) // transfer nodes to a min heap
{
	int i;
	int j = 0;
	NODE *tmp;

	minHeap = (NODE **)malloc(sizeof(NODE *) * n_usymb);
	// move the nodes from linked lists to array
	for (i = 0; i < N_BUCKET; i++) {
		tmp = bucket[i];
		while (tmp != NULL) {
			minHeap[j++] = tmp;
			tmp = tmp->l;
		}
	}
	// make the array a min heap
	for (i = n_usymb / 2 - 1; i >= 0; i--) minHeapify(minHeap, i, n_usymb);
}

void minHeapify(NODE **list, int i, int n) // enforce min heap property
{
	int j; // index
	int done; // loop flag
	NODE *tmp;

	j = 2 * (i + 1) - 1; // initialize j to be left child of i
	tmp = list[i]; // copy root element
	done = 0;
	while ((j <= n - 1) && (!done)) {
		// let list[j] to be the smaller child
		if ((j < n - 1) && (list[j]->freq > list[j + 1]->freq)) j++;
		if (tmp->freq < list[j]->freq) done = 1; // exit if root is smaller
		else {
			list[(j + 1) / 2 - 1] = list[j]; // replace j's parent with list[j]
			j = 2 * (j + 1) - 1; // move j to its left child
		}
	}
	list[(j + 1) / 2 - 1] = tmp; // move original root to proper place
}

NODE *minHeapRemoveMin(NODE **list, int n) // remove minimum from the min heap
{
	NODE *tmp;

	if (list == NULL) return NULL; // empty heap
	tmp = list[0]; // minimum is the root
	list[0] = list[n - 1]; // move last node to root
	minHeapify(list, 0, n - 1); // restore min heap property

	return tmp;
}

void minHeapInsertion(NODE **list, int n, NODE *new_node)
{
	int i; // index

	i = n - 1; // start at last node
	list[n - 1] = new_node; // put new node at last node
	while ((i > 0) && (list[(i + 1) / 2 - 1]->freq > new_node->freq)) {
		list[i] = list[(i + 1) / 2 - 1]; // overwrite list[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	list[i] = new_node; // put new node at proper place
}

void Tree(int n_heap) // construct the merged tree for Huffman code
{
	NODE *tmp, *tmp2, *new_node;

	while (n_heap >= 2) { // stop when only one node in heap
		// select the first and second smallest nodes
		tmp = minHeapRemoveMin(minHeap, n_heap--);
		tmp2 = minHeapRemoveMin(minHeap, n_heap--);
		new_node = (NODE *)malloc(sizeof(NODE)); // allocate memory
		new_node->val = 0; // non leaf node
		new_node->freq = tmp->freq + tmp2->freq; // sum up the frequency
		new_node->l = tmp; // smaller node goes to left child
		new_node->r = tmp2; // larger node goes to right child
		// insert the merged node back to heap
		minHeapInsertion(minHeap, ++n_heap, new_node);
	}
}

void printHuffmanCode(NODE *node, int i, int bit) // print Huffman code
{
	int j; // index

	if (bit == -1) { // initial call
		code = (int *)malloc(sizeof(int) * (n_usymb - 1)); // allocate memory
		printf("Huffman coding:\n");
	}
	else code[i - 1] = bit; // set the bit

	if (node->val != 0) { // leaf nodes
		// print symbol
		if (node->val == 32) printf("0x20: "); // space symbol
		else if (node->val == 10) printf("0x0D: "); // \n symbol
		else printf("%s: ", node->symbol);
		// print the code for the symbol
		for (j = 0; j < i; j++) printf("%d", code[j]);
		printf("\n");
		n_bit += node->freq * i; // accumulate number of bits needed
	}
	else { // non leaf nodes
		printHuffmanCode(node->l, i + 1, 0); // left child, next bit is 0
		printHuffmanCode(node->r, i + 1, 1); // right child, next bit is 1
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}
