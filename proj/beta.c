
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define b2B(x) (x % 8 ? x / 8 + 1 : x / 8) // convert bits to bytes needed
#define N_BUCKET 1000

typedef struct node {
	unsigned int val; // character
	int freq; // frequency
	char *symbol; // utf-8 symbol
	struct node *l; // left child
	struct node *r; // right child
} NODE;

int n_1B = 0; // # 1-byte symbols
int n_2B = 0; // # 2-byte symbols
int n_3B = 0; // # 3-byte symbols
int n_4B = 0; // # 4-byte symbols
int n_tt = 0; // total number of symbols
int n_byte = 0; // # bytes using utf-8
int n_bit = 0; // # bits using Huffman code
int n_usymb = 0; // # unique symbols
int *code;
NODE *bucket[N_BUCKET];
NODE **minHeap;

double GetTime(void); // get local time in seconds
void readText(void);
void char2binary(char ch);
int det_nB(char ch); // determine n-byte char
unsigned int hash2(char *str); // unsigned int: 4 bytes
NODE *find(unsigned int val);
void insert(unsigned int val, char *symbol, int n_B);

void hash2minHeap(NODE **bucket, int n_usymb);
void minHeapify(NODE **list, int i, int n);
NODE *minHeapRemoveMin(NODE **list, int n); // remove minimum from the min heap
void minHeapInsertion(NODE **list, int n, NODE *new_node);
void Tree(int n_heap);
void printHuffmanCode(NODE *node, int i, int bit); // print Huffman code

int main(void)
{
	int i;
	int cnt;
	double t0, t1;
	NODE *tmp;
	int clone;

	for (i = 0; i < N_BUCKET; i++) bucket[i] = NULL;
	readText();
	// for (i = 0; i < N_BUCKET; i++) {
	// 	tmp = bucket[i];
	// 	if (tmp) printf("%s\n", tmp->symbol);
	// }
	hash2minHeap(bucket, n_usymb);
	// clone = n_usymb;
	// while (clone) {
	// 	tmp = minHeapRemoveMin(minHeap, clone--);
	// 	printf("%d\n", tmp->freq);
	// }
	Tree(n_usymb);
	printHuffmanCode(minHeap[0], 0, -1); // print Huffman code
	printf("Number of Chars read: %d\n", n_byte);
	printf("# unique symbol: %d\n", n_usymb);
	printf("  Huffman Coding needs %d bits, %d bytes\n", n_bit, b2B(n_bit));
	// print the ratio of bytes needed with and without using Huffman code
	printf("  Ratio = %.4f %%\n", b2B(n_bit) * 100.0 / n_byte);
	// printf("%.5fs\n", t1 - t0);
	return 0;
}

void readText(void)
{
	char ch;
	char symbol[5];
	int idx_ch = 0;
	int type; // 0, 1, 2, 3, 4-byte symbol
	unsigned long val; // hash value
	NODE *tmp;

	symbol[0] = '\0';
	type = 4;
	while ((ch = getchar()) != EOF) {
		n_byte++;
		if (n_byte % type == 0) {
			symbol[type] = '\0';
			val = hash2(symbol);
			tmp = find(val);
			if (tmp == NULL) {
				insert(val, symbol, type);
				n_usymb++; // number of nodes increase by 1
			}
			else tmp->freq++;
			idx_ch = 0;
		}
		symbol[idx_ch++] = ch;
	}
	symbol[idx_ch] = '\0';
	val = hash2(symbol);
	tmp = find(val);
	if (tmp == NULL) {
		insert(val, symbol, type);
		n_usymb++; // number of nodes increase by one
	}
	else tmp->freq++;

	// printf("\nTotal: %d bytes\n", n_byte);
	// printf("1-byte symbol: %d\n", n_1B);
	// printf("2-byte symbol: %d\n", n_2B);
	// printf("3-byte symbol: %d\n", n_3B);
	// printf("4-byte symbol: %d\n", n_4B);
	// n_tt = n_1B + n_2B + n_3B + n_4B;
	// printf("# symbol: %d\n", n_tt);
	// printf("# unique symbol: %d\n", n_usymb);
}

void char2binary(char ch)
{
	int i;

	for (i = 0; i < 8; i++) {
		printf("%d", !!((ch << i) & 0x80)); // 0x80 = 1000 0000
	}
	printf(" ");
	// printf("\n");
}

int det_nB(char ch) // determine n-byte char
{
	if (!(ch & 0x80)) { // 0xxx xxxx
		n_1B++; return 1;
	}
	else { // 1xxx xxxx
		if (!(ch & 0x40)) { // 10xx xxxx
			return 0;
		}
		else { // 11xx xxxx
			if (!(ch & 0x20)) { // 110x xxxx
				n_2B++; return 2;
			}
			else { // 111x xxxx
				if (!(ch & 0x10)) { // 1110 xxxx
					n_3B++; return 3;
				}
				else { // 1111 xxxx
					n_4B++; return 4;
				}
			}
		}
	}
}

unsigned int hash2(char *str) // unsigned int: 4 bytes
{
	unsigned int hash = 0;
	unsigned char c;

	while ((c = *str++)) {
		hash = (hash << 8) + c;
	}

	return hash;
}

NODE *find(unsigned int val)
{
	NODE *tmp = bucket[val % N_BUCKET];

	while (tmp != NULL) {
		if (tmp->val == val) return tmp;
		tmp = tmp->l;
	}
	return NULL;
}

void insert(unsigned int val, char *symbol, int n_B)
{
	int rep = val % N_BUCKET; // representative
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

void hash2minHeap(NODE **bucket, int n_usymb)
{
	int i;
	int j = 0;
	NODE *tmp;

	minHeap = (NODE **)malloc(sizeof(NODE *) * n_usymb);
	for (i = 0; i < N_BUCKET; i++) {
		tmp = bucket[i];
		while (tmp != NULL) {
			minHeap[j++] = tmp;
			tmp = tmp->l;
		}
	}
	for (i = n_usymb / 2 - 1; i >= 0; i--)
		minHeapify(minHeap, i, n_usymb);
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
	int k;

	if (bit == -1) { // initial call
		code = (int *)malloc(sizeof(int) * (n_usymb - 1)); // allocate memory
		printf("Huffman coding:\n");
	}
	else code[i - 1] = bit; // set the bit

	if (node->val != 0) { // leaf nodes
		// print symbol
		k = 0;
		while (node->symbol[k]) {
			// char2binary(node->symbol[k]);
			printf("0x%x ", node->symbol[k] & 0xff);
			k++;
		}
		printf(": ");
		// print the code for the char
		for (j = 0; j < i; j++) printf("%d", code[j]);
		printf("\n");
		n_bit += node->freq * i; // accumulate number of bits needed
	}
	else { // non leaf nodes
		printHuffmanCode(node->l, i + 1, 0); // left child, next bit is 0
		printHuffmanCode(node->r, i + 1, 1); // right child, next bit is 1
	}
}

/*
*/
